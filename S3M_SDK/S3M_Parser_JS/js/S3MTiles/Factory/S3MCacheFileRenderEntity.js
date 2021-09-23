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

    function getExtension(gl, names) {
        let length = names.length;
        for (let i = 0; i < length; ++i) {
            let extension = gl.getExtension(names[i]);
            if (extension) {
                return extension;
            }
        }

        return undefined;
    }

    function createShaderProgram(context, attributeLocations, material, vertexPackage) {
        if(context.texturelod === undefined){
            context.texturelod = Cesium.defaultValue(getExtension(context._gl, ['EXT_shader_texture_lod']), false);
        }

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

        if(vertexPackage.instanceIndex > -1 && vertexPackage.instanceMode === 17){
            vp.defines.push('Instance');
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

    function getUniformMap(material,layer) {
        return {
            uTexture : function() {
                if(!Cesium.defined(material.textures[0])){
                    return layer.context.defaultTexture;
                }
                return material.textures[0];
            },
            uTextureWidth : function() {
                if(!Cesium.defined(material.textures[0])){
                    return 1.0;
                }
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
            },
            uTexture0Width : function(){
                if(!Cesium.defined(material.textures[0])){
                    return 1.0;
                }
                return material.textures[0]._width;
            },
            uTexture1Width : function(){
                if(!Cesium.defined(material.textures[1])){
                    return 1.0;
                }
                return material.textures[1]._width;
            },
        };
    }

    S3MCacheFileRenderEntity.prototype.createCommand = function() {
        if(Cesium.defined(this.colorCommand)) {
            return ;
        }

        let layer = this.layer;
        let context = layer.context;
        let vertexPackage = this.vertexPackage;
        this._attributeLocations = vertexPackage.attrLocation;
        this._instanceCount = vertexPackage.instanceCount;
        let arrIndexPackage = this.arrIndexPackage;
        let attributes = vertexPackage.vertexAttributes;
        let attributeLocations = vertexPackage.attrLocation;
        if(arrIndexPackage.length < 1) {
            return ;
        }

        let indexPackage = arrIndexPackage[0];
        let material = this.material;

        this.shaderProgram = createShaderProgram(context, attributeLocations, material,vertexPackage);

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
            renderState : material.bTransparentSorting ? getTransparentRenderState() : getOpaqueRenderState(),
            instanceCount : vertexPackage.instanceCount
        });

        this.colorCommand.uniformMap = getUniformMap(material,layer);
        this.vertexPackage = null;
        this.arrIndexPackage = null;
        this.ready = true;
    };

    function createSelectionMap(ro){
        let pickInfo = ro._pickInfo;
        if(!Cesium.defined(pickInfo) ){
            return;
        }
        ro._selectionInfoMap = new Cesium.AssociativeArray();
        for(let id in pickInfo){
            if(!pickInfo.hasOwnProperty(id)){
                continue;
            }

            ro._selectionInfoMap.set(id, pickInfo[id]);
        }
    }

    let scratchPntCenter = new Cesium.Cartesian3();
    function createBoundingBoxForInstance(ro){
         let vertexPackage = ro.vertexPackage;
         if(!Cesium.defined(vertexPackage) || vertexPackage.instanceIndex === -1 || !Cesium.defined(vertexPackage.instanceBounds)){
             return;
         }
         let instanceBounds = vertexPackage.instanceBounds;
         let pntLU = new Cesium.Cartesian3(instanceBounds[0], instanceBounds[1], instanceBounds[2]);
         let pntRD = new Cesium.Cartesian3(instanceBounds[3], instanceBounds[4], instanceBounds[5]);
         let pntCenter = Cesium.Cartesian3.lerp(pntLU, pntRD, 0.5, scratchPntCenter);
         let dRadius = Cesium.Cartesian3.distance(pntCenter, pntLU);
         let vecCenter = new Cesium.Cartesian3();
         Cesium.Matrix4.multiplyByPoint(ro.modelMatrix, pntCenter, vecCenter);
         ro.boundingVolume.center = vecCenter;
         ro.boundingVolume.radius = dRadius > ro.boundingVolume.radius ? dRadius : ro.boundingVolume.radius;
         vertexPackage.instanceBounds = undefined;
    }

    function createBatchTable(context, ro){
        let attributes = [];
        attributes.push({
            functionName : 'batchTable_pickColor',
            componentDatatype : Cesium.ComponentDatatype.UNSIGNED_BYTE,
            componentsPerAttribute : 4,
            normalize : true
        })
        let pickInfo = ro._pickInfo;
        let pickIds = Object.keys(pickInfo);
        let numberOfInstances = ro._instanceCount > 0 ? ro._instanceCount : pickIds.length;
        let batchTable = new Cesium.BatchTable(context,attributes,numberOfInstances);
        ro._batchTable = batchTable;
    }

    var cartesian4Scratch = new Cesium.Cartesian4();
    function setPickIdValues(context, ro, layer){
        let batchTable = ro._batchTable;
        let selectionInfoMap = ro._selectionInfoMap;
        let hash = selectionInfoMap._hash;
        for(let id in hash){
            if(hash.hasOwnProperty(id)){
                let selInfo = selectionInfoMap.get(id);
                let pickId;
                if(!Cesium.defined(pickId)){
                    pickId = context.createPickId({
                       primitive:layer,
                       id : id
                    })
                }
                let pickColor = pickId.color;
                cartesian4Scratch.x = Cesium.Color.floatToByte(pickColor.red);
                cartesian4Scratch.y = Cesium.Color.floatToByte(pickColor.green);
                cartesian4Scratch.z = Cesium.Color.floatToByte(pickColor.blue);
                cartesian4Scratch.w = Cesium.Color.floatToByte(pickColor.alpha);
                let instanceIds = selInfo.instanceIds;
                if(ro._instanceCount > 0){
                    instanceIds.map(function(instanceId){
                        batchTable.setBatchedAttribute(instanceId, 0, cartesian4Scratch);
                    });
                }else{
                    let batchId = selInfo[0].batchId;
                    batchTable.setBatchedAttribute(batchId,0,cartesian4Scratch);
                }
            }
        }
    }

    function updateShaderProgram(context, ro){
        let vp = ro.shaderProgram.vertexShaderSource;
        let fp = ro.shaderProgram.fragmentShaderSource;
        let vs = vp.sources[0];
        let attributeLocations = ro._attributeLocations;
        vs = ro._batchTable.getVertexShaderCallback()(vs);

        vp = new Cesium.ShaderSource({
            defines : vp.defines,
            sources : [vs]
        });

        if(vp.defines.indexOf('BatchTable') === -1){
            vp.defines.push('BatchTable');
        }

        ro.shaderProgram = ro.shaderProgram && ro.shaderProgram.destroy();
        ro.shaderProgram = Cesium.ShaderProgram.fromCache({
            context : context,
            vertexShaderSource : vp,
            fragmentShaderSource : fp,
            attributeLocations : attributeLocations
        });
        ro.colorCommand.shaderProgram = ro.shaderProgram;

    }

    function combineUniforms(ro){
        let batchTable = ro._batchTable;
        let command = ro.colorCommand;
        command.uniformMap = batchTable.getUniformMapCallback()(ro.colorCommand.uniformMap);
    }

    S3MCacheFileRenderEntity.prototype.update = function(frameState, layer) {
        if(!this.ready) {
            createSelectionMap(this);
            createBoundingBoxForInstance(this);
            this.createBuffers(frameState);
            this.createCommand(frameState);
            return ;
        }
        if(!Cesium.defined(this._batchTable)){
            createBatchTable(frameState.context, this);
            updateShaderProgram(frameState.context, this);
            combineUniforms(this);
            setPickIdValues(frameState.context, this, layer);
        }

        frameState.commandList.push(this.colorCommand);
        this._batchTable.update(frameState);
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
