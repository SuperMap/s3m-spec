import { computeDimension, runSort } from './GaussianSplatUtil.js';
const Matrix4 = Cesium.Matrix4;
const Texture = Cesium.Texture;
const Sampler = Cesium.Sampler;
const PixelFormat = Cesium.PixelFormat;
const PixelDatatype = Cesium.PixelDatatype;
const TextureMinificationFilter = Cesium.TextureMinificationFilter;
const TextureMagnificationFilter = Cesium.TextureMagnificationFilter;
const Cartesian2 = Cesium.Cartesian2;
const Cartesian3 = Cesium.Cartesian3;
const defined = Cesium.defined;



const GaussianSplatSortingState = {
  IDLE: 0,
  WAITING: 1,
  SORTING: 2,
  SORTED: 3,
  ERROR: 4,
};

function createTexture(context, textureSize, buffer) {
  return new Texture({
    context,
    source: {
      width: textureSize.x,
      height: textureSize.y,
      arrayBufferView: buffer,
    },
    preMultiplyAlpha: false,
    skipColorSpaceConversion: true,
    pixelFormat: PixelFormat.RED_INTEGER,
    pixelDatatype: PixelDatatype.UNSIGNED_INT,  //FLOAT,
    flipY: false,
    sampler: new Sampler({
      minificationFilter: TextureMinificationFilter.NEAREST,
      magnificationFilter: TextureMagnificationFilter.NEAREST,
    })
  });
}

function GaussianSplatCPUSorter(context, splatCount) {
  this._indexTextureSize = computeDimension(1, splatCount, new Cartesian2());
  this._indexTexBuffer = new Uint32Array(this._indexTextureSize.x * this._indexTextureSize.y);
  this._splatIndexTexture = createTexture(context, this._indexTextureSize, this._indexTexBuffer);
  this._splatCount = splatCount;

  // 相机发生多大幅度的变化后触发排序
  this._cameraPosChangedSortThreshold = .3;
  this._cameraAngleChangedSortThreshold = Math.cos(20 * Math.PI / 180);
  this._lastCameraPos = new Cartesian3();
  this._lastCameraDirection = new Cartesian3();
  this._sorterState = GaussianSplatSortingState.IDLE;
  this._sorterPromise = undefined;
  this._sorterError = undefined;
  this._sorterReady = false;  //第一次排序完成准备
  this._splatMatrix = new Matrix4();
}

// 合并精细层排序更新参数
GaussianSplatCPUSorter.prototype.updatePamerter = function (positionBuffer, frameState, splatCount) {
  this._splatCount = splatCount;
  this._sorterReady = true;
  Cartesian3.clone(frameState.camera.position, this._lastCameraPos);
  Cartesian3.clone(frameState.camera.direction, this._lastCameraDirection);
  this._positionBuffer = positionBuffer;
}

// 初始化排序
GaussianSplatCPUSorter.prototype.init = function (positionBuffer, frameState, matModel, splatCount) {
  if (defined(splatCount)) {
    this._splatCount = splatCount;
  }
  Matrix4.multiply(frameState.camera.viewMatrix, matModel, this._splatMatrix)
  let sortedData = runSort(positionBuffer, this._splatMatrix, this._splatCount);
  this._updateSort(sortedData);
  this._sorterReady = true;
  Cartesian3.clone(frameState.camera.position, this._lastCameraPos);
  Cartesian3.clone(frameState.camera.direction, this._lastCameraDirection);
  this._positionBuffer = positionBuffer;
}

GaussianSplatCPUSorter.prototype.update = function (frameState, matModel) {
  let isCopyBuffer = frameState.gaussianSplatController.isDynamicCombine;
  if (this._sorterState === GaussianSplatSortingState.IDLE) {
    if (!this._checkNeedSort(frameState.camera)) {
      return;
    }
    const transferableObjects = isCopyBuffer ? [] : [this._positionBuffer.buffer];
    if (!defined(this._sorterPromise)) {
      this._sorterPromise = frameState.gaussianSplatController._gaussianSplatSortProcessor.scheduleTask({
        positions: this._positionBuffer,
        modelView: Matrix4.multiply(frameState.camera.viewMatrix, matModel, this._splatMatrix),
        count: this._splatCount,
        type: 'sortIndex'
      }, transferableObjects);
    }

    if (!defined(this._sorterPromise)) {
      this._sorterState = GaussianSplatSortingState.WAITING;
      return;
    }
    this._sorterPromise.catch((err) => {
      this._sorterState = GaussianSplatSortingState.ERROR;
      this._sorterError = err;
    });
    this._sorterPromise.then((sortedData) => {
      if (this.isDestroyed()) {
        return;
      }
      this._updateSort(sortedData.indexs);
      this._positionBuffer = sortedData.positions;
      this._sorterState = GaussianSplatSortingState.SORTED;
    });

    Cartesian3.clone(frameState.camera.position, this._lastCameraPos);
    Cartesian3.clone(frameState.camera.direction, this._lastCameraDirection);
    this._sorterState = GaussianSplatSortingState.SORTING;
  } else if (this._sorterState === GaussianSplatSortingState.WAITING) {
    const transferableObjects = isCopyBuffer ? [] : [this._positionBuffer.buffer];
    if (!defined(this._sorterPromise)) {
      this._sorterPromise = frameState.gaussianSplatController._gaussianSplatSortProcessor.scheduleTask({
        positions: this._positionBuffer,
        modelView: Matrix4.multiply(frameState.camera.viewMatrix, matModel, this._splatMatrix),
        count: this._splatCount,
        type: 'sortIndex'
      }, transferableObjects);
    }
    if (!defined(this._sorterPromise)) {
      this._sorterState = GaussianSplatSortingState.WAITING;
      return;
    }
    this._sorterPromise.catch((err) => {
      this._sorterState = GaussianSplatSortingState.ERROR;
      this._sorterError = err;
    });
    this._sorterPromise.then((sortedData) => {
      if (this.isDestroyed()) {
        return;
      }
      this._updateSort(sortedData.indexs);
      this._positionBuffer = sortedData.positions;
      this._sorterState = GaussianSplatSortingState.SORTED;
    });
    this._sorterState = GaussianSplatSortingState.SORTING;
  } else if (this._sorterState === GaussianSplatSortingState.SORTING) {
    return;
  } else if (this._sorterState === GaussianSplatSortingState.SORTED) {
    this._sorterState = GaussianSplatSortingState.IDLE; //reset state for next frame
    this._sorterPromise = undefined; //reset promise for next frame
  } else if (this._sorterState === GaussianSplatSortingState.ERROR) {
    throw this._sorterError;
  }
};


GaussianSplatCPUSorter.prototype._checkNeedSort = function (camera) {
  // 水面反射阶段临时修改了相机参数，跳过排序避免误判
  if (camera.bReflect) {
    return false;
  }
  const dot = Cartesian3.dot(this._lastCameraDirection, camera.direction);

  if (Math.abs(dot) < this._cameraAngleChangedSortThreshold) {
    return true;
  }

  return false;
}

GaussianSplatCPUSorter.prototype._updateSort = function (indexBuffer) {
  if (indexBuffer) {
    this._indexTexBuffer.set(indexBuffer, 0);
    let length = this._splatIndexTexture?.copyFrom.length;
    let options = length > 1 ? {  //兼容Cesium版本
      width: this._indexTextureSize.x,
      height: this._indexTextureSize.y,
      arrayBufferView: this._indexTexBuffer,
    } : {
      source: {
        width: this._indexTextureSize.x,
        height: this._indexTextureSize.y,
        arrayBufferView: this._indexTexBuffer,
      }
    }
    this._splatIndexTexture && this._splatIndexTexture.copyFrom(options)
  }
};

GaussianSplatCPUSorter.prototype.destroyTexture = function () {
  this._splatIndexTexture = this._splatIndexTexture && !this._splatIndexTexture.isDestroyed() && this._splatIndexTexture.destroy();
};

GaussianSplatCPUSorter.prototype.isDestroyed = function () {
  return false;
};

GaussianSplatCPUSorter.prototype.destroy = function () {
  this.destroyTexture();
  Cesium.destroyObject(this);
};

export default GaussianSplatCPUSorter;
