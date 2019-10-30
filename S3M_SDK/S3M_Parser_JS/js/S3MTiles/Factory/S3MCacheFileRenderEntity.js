define([
    '../Shaders/S3MTilesVS',
    '../Shaders/S3MTilesFS',
    './RenderEntity'
    ], function(
    S3MTilesVS,
    S3MTilesFS,
    RenderEntity
        ) {
    'use strict';


    function S3MCacheFileRenderEntity(options) {
        RenderEntity.call(this, options);
    }

    S3MCacheFileRenderEntity.prototype = Object.create( RenderEntity.prototype );

    S3MCacheFileRenderEntity.prototype.constructor = RenderEntity;

    function createShaderProgram(context, attributeLocations, material) {
        let vp = new Cesium.ShaderSource({
            sources : [S3MTilesVS]
        });

        let fp = new Cesium.ShaderSource({
            sources : [S3MTilesFS]
        });

        if(Cesium.defined(attributeLocations['aNormal'])) {
            vp.defines.push('VertexNormal');
            fp.defines.push('VertexNormal');
        }

        if(material.textures.length > 0) {
            vp.defines.push('TexCoord');
            fp.defines.push('TexCoord');
        }

        if(material.textures.length == 2) {
            vp.defines.push('TexCoord2');
            fp.defines.push('TexCoord2');
        }

        return Cesium.ShaderProgram.fromCache({
            context : context,
            vertexShaderSource : vp,
            fragmentShaderSource : fp,
            attributeLocations : attributeLocations
        });

    }

    function getOpaqueRenderState() {
        return Cesium.RenderState.fromCache({
            cull : {
                enabled : true
            },
            depthTest : {
                enabled : true,
                func : Cesium.DepthFunction.LESS_OR_EQUAL
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

    function getUniformMap(material) {
        return {
            uTexture : function() {
                return material.textures[0];
            },
            uTextureWidth : function() {
                return material.textures[0]._width;
            },
            uTexture2 : function() {
                return material.textures[1];
            },
            uTexture2Width : function() {
                return material.textures[1]._width;
            },
            uDiffuseColor : function() {
                return material.diffuseColor;
            },
            uAmbientColor : function() {
                return material.ambientColor;
            }
        };
    }

    S3MCacheFileRenderEntity.prototype.createCommand = function() {
        if(Cesium.defined(this.colorCommand)) {
            return ;
        }

        let layer = this.layer;
        let context = layer.context;
        let vertexPackage = this.vertexPackage;
        let arrIndexPackage = this.arrIndexPackage;
        let attributes = vertexPackage.vertexAttributes;
        let attributeLocations = vertexPackage.attrLocation;
        if(arrIndexPackage.length < 1) {
            return ;
        }

        let indexPackage = arrIndexPackage[0];
        let material = this.material;

        this.shaderProgram = createShaderProgram(context, attributeLocations, material);

        this.vertexArray = new Cesium.VertexArray({
            context : context,
            attributes : attributes,
            indexBuffer : indexPackage.indexBuffer
        });

        this.colorCommand = new Cesium.DrawCommand({
            primitiveType : indexPackage.primitiveType,
            modelMatrix : this.modelMatrix,
            boundingVolume : Cesium.BoundingSphere.clone(this.boundingVolume),
            pickId : 'vSecondColor',
            vertexArray : this.vertexArray,
            shaderProgram : this.shaderProgram,
            pass : material.bTransparentSorting ? Cesium.Pass.TRANSLUCENT : Cesium.Pass.CESIUM_3D_TILE,
            renderState : material.bTransparentSorting ? getTransparentRenderState() : getOpaqueRenderState()
        });

        this.colorCommand.uniformMap = getUniformMap(material);
        this.vertexPackage = null;
        this.arrIndexPackage = null;
        this.ready = true;
    };

    S3MCacheFileRenderEntity.prototype.update = function(frameState) {
        if(!this.ready) {
            this.createBuffers(frameState);
            this.createCommand(frameState);
            return ;
        }

        frameState.commandList.push(this.colorCommand);
    };

    S3MCacheFileRenderEntity.prototype.isDestroyed = function() {
        return false;
    };

    S3MCacheFileRenderEntity.prototype.destroy = function() {
        this.shaderProgram = this.shaderProgram && !this.shaderProgram.isDestroyed() && this.shaderProgram.destroy();
        this.vertexArray = this.vertexArray && !this.vertexArray.isDestroyed() && this.vertexArray.destroy();
        this.colorCommand = undefined;
        this.vertexPackage = null;
        this.arrIndexPackage = null;
        return Cesium.destroyObject(this);
    };

    return S3MCacheFileRenderEntity;
});
