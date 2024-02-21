import S3MTilesVS from '../Shaders/S3MTilesVS.js';
import S3MTilesFS from '../Shaders/S3MTilesFS.js';
import RenderEntity from './RenderEntity.js';
import FillStyle from '../FillStyle.js';
import AlphaMode from '../Enum/AlphaMode.js';
const defined = Cesium.defined;

function S3MCacheFileRenderEntity(options) {
    RenderEntity.call(this, options);
    this.vs = S3MTilesVS;
    this.fs = S3MTilesFS;
    this.edgeVA = undefined;
    this.edgeSP = undefined;
    this.regularEdgeCommand = undefined;
    this.silhouetteEdgeCommand = undefined;
    this.useLineColor = false;
}

S3MCacheFileRenderEntity.prototype = Object.create( RenderEntity.prototype );

S3MCacheFileRenderEntity.prototype.constructor = RenderEntity;

function getOpaqueRenderState() {
    return Cesium.RenderState.fromCache({
        cull : {
            enabled : false
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

function getTransparentRenderState() {
    return Cesium.RenderState.fromCache({
        cull : {
            enabled : true
        },
        depthTest : {
            enabled : true,
            func : Cesium.DepthFunction.LESS_OR_EQUAL
        },
        blending : Cesium.BlendingState.ALPHA_BLEND
    });
}

let texDimScratch = new Cesium.Cartesian4();
function getUniformMap(material, layer, ro) {
    var materialPass = ro.materialPass || {};

    let uniformMapObj =  {
        uGeoMatrix : function() {
            return ro.geoMatrix;
        },
        uTexMatrix : function() {
            return material.texMatrix;
        },
        uFillForeColor : function(){
            if(ro.useLineColor){
                return layer.style3D.lineColor;
            }

            return layer.style3D.fillForeColor;
        },
        uDiffuseColor : function() {
            return  material.diffuseColor;
        },
        uAmbientColor : function() {
            return material.ambientColor;
        },
        uInverseGeoMatrix : function() {
            return ro.invGeoMatrix;
        },
        uTexture : function() {
            let texture = material.textures[0];
            if(texture.isTexBlock){
                return texture.renderable && texture.ready ? texture : material.ancestorTexture ? material.ancestorTexture : texture;
            }

            return material.textures[0];
        },
        uTexture2 : function() {
            return material.textures[1];
        },
        uTexAtlasDim : function() {
            let texture = material.textures[0].renderable ? material.textures[0] : material.ancestorTexture ? material.ancestorTexture : material.textures[0];
            texDimScratch.x = texture.width;
            texDimScratch.y = texture.height;
            if(material.textures[1]){
                let secTexture = material.textures[1].renderable ? material.textures[1] : material.ancestorTextureBake ? material.ancestorTextureBake : material.textures[1];
                texDimScratch.z = secTexture.width;
                texDimScratch.w = secTexture.height;
            }

            return texDimScratch;
        },
       
        uTexture0Width : function(){
            return material.textures[0].width;
        },
        uTexture1Width : function(){
            return material.textures[1].width;
        },
        uSelectedColor : function(){
            return layer.selectedColor;
        },
            //颜色矫正
        uBrightness : function(){
            return layer._brightness;
        },
        uContrast : function(){
            return layer._contrast;
        },
        uHue : function(){
            return layer._hue;
        },
        uSaturation : function(){
            return layer._saturation;
        },
        uOneOverGamma : function(){
            return 1.0/layer._gamma;
        },
        decodePositionMin : function() {
           const minVerticesValue = ro.vertexPackage.minVerticesValue;
           return  minVerticesValue;
        },
        decodePositionNormConstant : function() {
            return  ro.vertexPackage.vertCompressConstant;
        },
        
    }

    let minTexCoordValue = ro.vertexPackage.minTexCoordValue;
    if(minTexCoordValue && minTexCoordValue.length > 0){
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


S3MCacheFileRenderEntity.prototype.createCommand = function() {
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

    let primitiveType = Cesium.PrimitiveType.TRIANGLES;
    switch(indexPackage.primitiveType){
        case 1 : primitiveType = Cesium.PrimitiveType.POINTS; break;
        case 2 : primitiveType = Cesium.PrimitiveType.LINES; break;
        case 4 : primitiveType = Cesium.PrimitiveType.TRIANGLES; break;
        default : break;
    }

    this.useLineColor = primitiveType === Cesium.PrimitiveType.LINES;

    let isTransparent = false;
    if((material.alphaMode === AlphaMode.MASK || material.alphaMode === AlphaMode.BLEND)){
        isTransparent = true;
    }
    else if(layer._isTransparencyOptimization && material.bTransparentSorting){
        isTransparent = true;
    }


    this.colorCommand = new Cesium.DrawCommand({
        primitiveType : primitiveType,
        modelMatrix : this.modelMatrix,
        boundingVolume : Cesium.BoundingSphere.clone(this.boundingVolume),
        pickId : this.pickColorIdentifier,
        vertexArray : this.vertexArray,
        shaderProgram : this.shaderProgram,
        pass : isTransparent ? Cesium.Pass.TRANSLUCENT : Cesium.Pass.CESIUM_3D_TILE,
        renderState : material.bTransparentSorting ? getTransparentRenderState() : getOpaqueRenderState(),
        instanceCount : vertexPackage.instanceCount
    });

    let uniformMap = getUniformMap(material, layer, this);
    if(this.batchTable){
        uniformMap = this.batchTable.getUniformMapCallback()(uniformMap);
    }

    if(material.batchTable){
        uniformMap = material.batchTable.getUniformMapCallback()(uniformMap);
    }

    this.colorCommand.uniformMap = uniformMap;

    // 跟倾斜哪里是一样的
    // this.vertexPackage = undefined;
    // this.arrIndexPackage = undefined;
    this.vs = undefined;
    this.fs = undefined;
    this.ready = true;
};




S3MCacheFileRenderEntity.prototype.transformResource = function(frameState, layer) {
    if(this.ready){
        return ;
    }

    this.createBatchTable(frameState);
    this.createPickIds();
    this.createBuffers(frameState);
    this.createShaderProgram(frameState);
    this.createCommand(frameState);
    this.initLayerSetting(layer);
};
S3MCacheFileRenderEntity.prototype.update = function(frameState, layer) {
    if(this.batchTableDirty){
        this.updateBatchTableAttributes();
        this.batchTableDirty = false;
    }
    
    if(this.batchTable){
        this.batchTable.update(frameState);
    }

    if(layer.style3D.fillStyle !== FillStyle.WireFrame){
        frameState.commandList.push(this.colorCommand);
    }

    if(layer.style3D.fillStyle !== FillStyle.Fill){
        if(this.regularEdgeCommand){
            frameState.commandList.push(this.regularEdgeCommand);
            layer._addRenderedEdge(this.regularEdgeCommand.edgeTotalLength, this.regularEdgeCommand.edgeCount);
        }

        if(this.silhouetteEdgeCommand) {
            frameState.commandList.push(this.silhouetteEdgeCommand);
            layer._addRenderedEdge(this.silhouetteEdgeCommand.edgeTotalLength, this.silhouetteEdgeCommand.edgeCount);
        }
    }

};

S3MCacheFileRenderEntity.prototype.isDestroyed = function() {
    return false;
};

S3MCacheFileRenderEntity.prototype.destroy = function() {
    this.shaderProgram = this.shaderProgram && !this.shaderProgram.isDestroyed() && this.shaderProgram.destroy();
    this.vertexArray = this.vertexArray && !this.vertexArray.isDestroyed() && this.vertexArray.destroy();
    this.material = this.material && !this.material.isDestroyed() && this.material.destroy();
    this.batchTable = this.batchTable && !this.batchTable.isDestroyed() && this.batchTable.destroy();
    this.edgeVA = this.edgeVA && !this.edgeVA.isDestroyed() && this.edgeVA.destroy();
    this.edgeSP = this.edgeSP && !this.edgeSP.isDestroyed() && this.edgeSP.destroy();
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

export default S3MCacheFileRenderEntity;