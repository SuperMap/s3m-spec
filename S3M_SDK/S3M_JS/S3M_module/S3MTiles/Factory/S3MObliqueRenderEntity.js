import S3MTilesNoLightVS from '../Shaders/S3MTilesNoLightVS.js';
import S3MTilesNoLightFS from '../Shaders/S3MTilesNoLightFS.js';
import RenderEntity from './RenderEntity.js';

function S3MObliqueRenderEntity(options) {
    RenderEntity.call(this, options);
    this.vs = S3MTilesNoLightVS;
    this.fs = S3MTilesNoLightFS;
}

S3MObliqueRenderEntity.prototype = Object.create( RenderEntity.prototype );

S3MObliqueRenderEntity.prototype.constructor = RenderEntity;

function getOpaqueRenderState() {
    return Cesium.RenderState.fromCache({
        cull : {
            enabled : true
        },
        depthTest : {
            enabled : true,
            func : Cesium.DepthFunction.LESS_OR_EQUAL
        },
        blending : Cesium.BlendingState.ALPHA_BLEND,
        stencilTest: {  // 采用3d tiles模板测试值
            backFunction: Cesium.StencilFunction.ALWAYS,
            backOperation: {
                fail: Cesium.StencilOperation.KEEP, 
                zFail: Cesium.StencilOperation.KEEP, 
                zPass: Cesium.StencilOperation.REPLACE
            },
            enabled: true,
            frontFunction: Cesium.StencilFunction.ALWAYS,
            frontOperation: {
                fail: Cesium.StencilOperation.KEEP, 
                zFail: Cesium.StencilOperation.KEEP, 
                zPass: Cesium.StencilOperation.REPLACE
            },
            mask: Cesium.StencilConstants.CESIUM_3D_TILE_MASK,
            reference: Cesium.StencilConstants.CESIUM_3D_TILE_MASK
        }
    });
}

let hypMinMaxValueScratch = new Cesium.Cartesian4();
let hypOpacityIntervalFillModeScratch = new Cesium.Cartesian4();
let swipRegionScratch = new Cesium.Cartesian4();
function getUniformMap(material, layer, ro) {
    let uniformMapObj = {
        uDiffuseColor : function() {
            return  material.diffuseColor;
        },
        uGeoMatrix : function() {
            return ro.geoMatrix;
        },
        uInverseGeoMatrix : function() {
            return ro.invGeoMatrix;
        },
        uTexture : function() {
            return material.textures[0];
        },
        uTexture0Width : function(){
            return material.textures[0].width;
        },
        decodePositionMin : function() {
            const minVerticesValue = ro.vertexPackage.minVerticesValue
           return  minVerticesValue;
        },
        decodePositionNormConstant : function() {
            return  ro.vertexPackage.vertCompressConstant;
        }
    }
    if(layer._vertexCompressionType == "MESHOPT") {
        uniformMapObj.decodeTexCoord0vNormConstant = function() {
            return  ro.vertexPackage.texCoordCompressConstant[0];
        }

        uniformMapObj.decodeTexCoord1vNormConstant = function() {
            return  ro.vertexPackage.texCoordCompressConstant[1];
        }
    
        uniformMapObj.decodeTexCoord0Min = function() {
            return  ro.vertexPackage.minTexCoordValue[0];
        }
    }
    return uniformMapObj;
}

S3MObliqueRenderEntity.prototype.createCommand = function() {
    if(Cesium.defined(this.colorCommand) || this.vertexBufferToCreate.length !== 0 || this.indexBufferToCreate.length !== 0 || this.shaderProgramToCreate.length !== 0) {
        return ;
    }

    let layer = this.layer;
    let context = layer.context;
    let vertexPackage = this.vertexPackage;
    let arrIndexPackage = this.arrIndexPackage;
    let attributes = vertexPackage.vertexAttributes;
    if(arrIndexPackage.length < 1) {
        return ;
    }

    let indexPackage = arrIndexPackage[0];
    let material = this.material;

    this.vertexArray = new Cesium.VertexArray({
        context : context,
        attributes : attributes,
        indexBuffer : indexPackage.indexBuffer
    });
    
    this.colorCommand = new Cesium.DrawCommand({
        primitiveType : indexPackage.primitiveType,
        modelMatrix : this.modelMatrix,
        boundingVolume : Cesium.BoundingSphere.clone(this.boundingVolume),
        vertexArray : this.vertexArray,
        shaderProgram : this.shaderProgram,
        pass : material.bTransparentSorting ? Cesium.Pass.TRANSLUCENT : Cesium.Pass.CESIUM_3D_TILE,
        renderState : getOpaqueRenderState(),
        instanceCount : vertexPackage.instanceCount
    });

    this.colorCommand.uniformMap = getUniformMap(material, layer, this);

    this.vs = undefined;
    this.fs = undefined;
    this.ready = true;
};
S3MObliqueRenderEntity.prototype.transformResource = function(frameState, layer) {
    if(this.ready){
        return ;
    }

    this.createBuffers(frameState);
    this.createShaderProgram(frameState);
    this.createCommand(frameState);
    this.initLayerSetting(layer);
};

S3MObliqueRenderEntity.prototype.update = function(frameState, layer) {
    frameState.commandList.push(this.colorCommand);
};

S3MObliqueRenderEntity.prototype.isDestroyed = function() {
    return false;
};

S3MObliqueRenderEntity.prototype.destroy = function() {
    this.shaderProgram = this.shaderProgram && !this.shaderProgram.isDestroyed() && this.shaderProgram.destroy();
    this.vertexArray = this.vertexArray && !this.vertexArray.isDestroyed() && this.vertexArray.destroy();
    this.material = this.material && !this.material.isDestroyed() && this.material.destroy();
    this.colorCommand = undefined;
    this.vertexPackage = null;
    this.arrIndexPackage = null;
    this.modelMatrix = undefined;
    this.pickInfo = undefined;
    this.selectionInfoMap = undefined;
    this.vs = undefined;
    this.fs = undefined;
    return Cesium.destroyObject(this);
};

export default S3MObliqueRenderEntity;