import { computeDimension } from './GaussianSplatUtil.js';
import GaussianSplatCPUSorter from './GaussianSplatCPUSorter.js';
import VertexCompressOption from '../Enum/VertexCompressOption.js';
const defined = Cesium.defined;
const destroyObject = Cesium.destroyObject;
const GeometryAttribute = Cesium.GeometryAttribute;
const PrimitiveType = Cesium.PrimitiveType;
const VertexArray = Cesium.VertexArray;
const BufferUsage = Cesium.BufferUsage;
const RenderState = Cesium.RenderState;
const Pass = Cesium.Pass;
const BlendingState = Cesium.BlendingState;
const DepthFunction = Cesium.DepthFunction;
const Geometry = Cesium.Geometry;
const ComponentDatatype = Cesium.ComponentDatatype;
const Cartesian2 = Cesium.Cartesian2;
const Texture = Cesium.Texture;
const PixelFormat = Cesium.PixelFormat;
const PixelDatatype = Cesium.PixelDatatype;
const Sampler = Cesium.Sampler;
const TextureMinificationFilter = Cesium.TextureMinificationFilter;
const TextureMagnificationFilter = Cesium.TextureMagnificationFilter;
const DrawCommand = Cesium.DrawCommand;


let scartchTextureSize = new Cartesian2();
function getVertexAttributesByName(vertexPackage, name) {
  let index = vertexPackage.attrLocation[name];
  return vertexPackage.vertexAttributes[index];
}

/**
 * 判断是否为共享buffer
 * @private
 * @param {typedArray} buffer 
 * @returns {Boolean} 返回是否是共享buffer
 */
function isBufferShared(buffer) {
  if (buffer instanceof ArrayBuffer) {
    return false;
  }
  if (ArrayBuffer.isView(buffer)) {
    return buffer.byteLength !== buffer.buffer.byteLength;
  }
  return false;
}

function updateFlags(layer, ro) {
  let flag = 0;
  const hasExcavation = layer._hasExcavation;
  flag = (hasExcavation << 1);
  return flag;
}

class GaussianSplatMesh {
  constructor(ro) {
    this._ro = ro;
    this._splatCount = ro.vertexPackage.verticesCount;
    this._textureSize = computeDimension(2, this._splatCount, new Cartesian2(2, 2));
    this._distanceToCamera = 0;
    this._cpuSorter = new GaussianSplatCPUSorter(ro.layer.context, this._splatCount);
    this._textureReady = false;
    this._initTaskPending = false; // 纹理初始化任务是否在途
  }

  get ready() {
    return this._textureReady && this._cpuSorter._sorterReady;
  }

  get positionBuffer() {
    return this._cpuSorter._positionBuffer;
  }

  initSplatTexture(frameState) {
    const controller = frameState.gaussianSplatController;
    const vertexPackage = this._ro.vertexPackage;
    let gsVersion = this._ro.getGaussianSplattingVersion();
    let posAttribute = getVertexAttributesByName(vertexPackage, 'aPosition');
    let posArray = posAttribute.typedArray;
    let colArray = getVertexAttributesByName(vertexPackage, 'aColor').typedArray;
    let scaArray = getVertexAttributesByName(vertexPackage, 'aGSScale').typedArray;
    let rotArray = getVertexAttributesByName(vertexPackage, 'aGSRotation').typedArray;
    let batchIdArray = getVertexAttributesByName(vertexPackage, 'batchId')?.typedArray;

    const options = {
      type: 'textureData',
      splatCount: vertexPackage.verticesCount,
      textureSize: this._textureSize,
      isCompress: vertexPackage.compressOptions && (vertexPackage.compressOptions & VertexCompressOption.SVC_Vertex) == VertexCompressOption.SVC_Vertex,
      isSPZCompress: vertexPackage.compressOptions && (vertexPackage.compressOptions & VertexCompressOption.SVC_GaussianSplatting) == VertexCompressOption.SVC_GaussianSplatting,
      minVerticesValue: vertexPackage.minVerticesValue,
      vertCompressConstant: vertexPackage.vertCompressConstant,
      positionDimension: posAttribute.componentsPerAttribute,  //顶点位置属性的维度
      version: gsVersion, //高斯数据的版本号
      posArray: posArray,
      colArray: colArray,
      scaArray: scaArray,
      rotArray: rotArray,
      batchIdArray: batchIdArray
    }
    const transferableObjects = isBufferShared(posArray) ? [] : [posArray.buffer, colArray.buffer, scaArray.buffer, rotArray.buffer];

    // createResources 在 ready 前可能每帧触发，任务在途时跳过，
    // 避免同一批 buffer 被 transfer 两次导致 DataCloneError: already detached
    if (this._initTaskPending) {
      return;
    }
    const scheduleResult = controller._gaussianSplatProcessor.scheduleTask(options, transferableObjects);
    if (!defined(scheduleResult)) {
      // 任务队列满（并发上限），下一帧重试
      return;
    }
    this._initTaskPending = true;
    scheduleResult.then((data) => {
      this._initTaskPending = false;
      if (this.isDestroyed()) {
        return;
      }
      const { bufferPos, texData } = data;
      let texBuffer = new Uint32Array(this._textureSize.x * this._textureSize.y * 4);
      texBuffer.set(texData, 0);
      this._cpuSorter.init(bufferPos, frameState, this._ro.modelMatrix);
      this._gaussianSplatTexture = new Texture({
        context: this._ro.layer.context,
        source: {
          width: this._textureSize.x,
          height: this._textureSize.y,
          arrayBufferView: texBuffer,
        },
        preMultiplyAlpha: false,
        skipColorSpaceConversion: true,
        pixelFormat: PixelFormat.RGBA_INTEGER,
        pixelDatatype: PixelDatatype.UNSIGNED_INT,
        flipY: false,
        sampler: new Sampler({
          minificationFilter: TextureMinificationFilter.NEAREST,
          magnificationFilter: TextureMagnificationFilter.NEAREST,
        })
      });
      this._textureReady = true;
      texBuffer = null;
    }).catch((err) => {
      this._initTaskPending = false;
      throw err;
    });
  }

  createCommand(frameState) {
    const ro = this._ro;
    const vertexPackage = ro.vertexPackage;
    const context = ro.layer.context;
    const layer = ro.layer;

    if (!vertexPackage || !ro.shaderProgram) {
      return;
    }

    const command = new DrawCommand({
      primitiveType: PrimitiveType.TRIANGLE_STRIP,
      modelMatrix: ro.modelMatrix,
      boundingVolume: defined(frameState.customWorldMatrix) ? undefined : ro.boundingVolume,
      pass: Pass.GAUSSIAN_SPLATS || Pass.CESIUM_3D_TILE,
      count: 4,
      cull: true,
      occlude: false,
      ownerName: layer.name
    });
    command.pickId = 'vSecondColor';
    command.shaderProgram = ro.shaderProgram;
    command.owner = ro;
    const vertexArray = (() => {
      const geometry = new Geometry({
        attributes: {
          screenQuadPosition: new GeometryAttribute({
            componentDatatype: ComponentDatatype.FLOAT,
            componentsPerAttribute: 2,
            values: [-1, -1, 1, -1, 1, 1, -1, 1],
            name: "a_position",
            variableName: "a_position",
          })
        },
        primitiveType: PrimitiveType.TRIANGLE_STRIP,
      });

      return VertexArray.fromGeometry({
        context: frameState.context,
        geometry: geometry,
        bufferUsage: BufferUsage.STATIC_DRAW,
        interleave: false
      });
    })();

    command.vertexArray = vertexArray;
    command.instanceCount = this._splatCount;
    command.renderState = RenderState.fromCache({
      cull: {
        enabled: false
      },
      depthTest: {
        enabled: true,
        // func: DepthFunction.ALWAYS
      },
      depthMask: false,
      blending: BlendingState.PRE_MULTIPLIED_ALPHA_BLEND,
    });
    this._flag = updateFlags(layer, ro);
    let uniformMap = this.createUniformMap(layer, ro);
    ro.uniformMap = uniformMap;
    command.uniformMap = uniformMap;
    ro.colorCommand = command;
  }

  createUniformMap(layer, ro) {
    const that = this;
    let uniformMap = {};
    uniformMap.u_splatScale = function () {
      return 1.0;
    };
    uniformMap.u_textureSize = function () {
      return that._textureSize;
    };
    uniformMap.u_splatAttributeTexture = function () {
      return that._gaussianSplatTexture;
    };
    uniformMap.u_splatCount = function () {
      return that._splatCount;
    };
    uniformMap.u_splatIndexesTexture = function () {
      return that._cpuSorter._splatIndexTexture;
    };
    uniformMap.u_indexTextureSize = function () {
      return that._cpuSorter._indexTextureSize;
    };
    uniformMap.u_indexOffset = function () {
      return 0;
    };

    if (defined(ro._batchTable)) {
      uniformMap = ro._batchTable.getUniformMapCallback()(uniformMap);
    }

    if (layer._hasExcavation) {
      uniformMap.u_SignedDistanceTexture = function () {
        return layer._clippingPolygonCollection.clippingTexture;
      };

      uniformMap.u_clippingExtentsTexture = function () {
        return layer._clippingPolygonCollection.extentsTexture;
      };

      uniformMap.u_SignedDistanceTextureSize = function () {
        scartchTextureSize.x = layer._clippingPolygonCollection.clippingTexture.width;
        scartchTextureSize.y = layer._clippingPolygonCollection.clippingTexture.height;
        return scartchTextureSize;
      };

      uniformMap.u_clippingExtentsTextureSize = function () {
        scartchTextureSize.x = layer._clippingPolygonCollection.extentsTexture.width;
        scartchTextureSize.y = layer._clippingPolygonCollection.extentsTexture.height;
        return scartchTextureSize;
      };
    }
    return uniformMap;
  }

  createResources(frameState, layer, createShaderProgram) {
    const ro = this._ro;
    if (!defined(ro.shaderProgram)) {
      ro.createShaderProgram(frameState, layer, ro);
      this.createCommand(frameState);
      this.initSplatTexture(frameState);
    }
  }

  updateUniformMap() {
    const ro = this._ro;
    const layer = ro.layer;
    const flag = updateFlags(layer, ro);
    if (this._flag !== flag) {
      this._flag = flag;
      let uniformMap = this.createUniformMap(layer, ro);
      ro.uniformMap = uniformMap;
      ro.colorCommand.uniformMap = uniformMap;
    }
  }

  updateSorter(frameState) {
    const ro = this._ro;
    this._cpuSorter.update(frameState, ro.modelMatrix);
  }

  update(frameState) {
    this.updateUniformMap();
  }

  // 根据单体id获取batchid在数据中的索引数组
  getBatchIdIndexArrayById(id) {
    return this._idToBatchIdForIndexMap[id];
  }

  clear() {
    this._textureData = null;
    this._idToBatchIdForIndexMap = {};
  }

  isDestroyed() {
    return false;
  };

  destroySplatTexture() {
    this._gaussianSplatTexture = this._gaussianSplatTexture && !this._gaussianSplatTexture.isDestroyed() && this._gaussianSplatTexture.destroy();
  }

  destroy() {
    this.destroySplatTexture();
    this._cpuSorter.destroy();
    destroyObject(this);
  }

}

export default GaussianSplatMesh
