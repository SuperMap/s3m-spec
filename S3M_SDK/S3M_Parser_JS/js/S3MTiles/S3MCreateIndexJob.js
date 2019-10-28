define([
],function(
){
    "use strict";

    function S3MCreateIndexBufferJob(){
        this.model = undefined;
        this.context = undefined;
        this.index = 0;
    }

    S3MCreateIndexBufferJob.prototype.set = function(context, model, index) {
        this.model = model;
        this.context = context;
        this.index = index;
    };

    S3MCreateIndexBufferJob.prototype.execute = function(){
        let context = this.context;
        let indexPackage = this.model.arrIndexPackage[this.index];
        let verticesCount = this.model.vertexPackage.verticesCount;
        if(!Cesium.defined(indexPackage)){
            throw new Cesium.DeveloperError('index package is null');
        }

        if(Cesium.defined(indexPackage.indexBuffer)){
            return ;
        }

        if(!Cesium.defined(indexPackage.indicesTypedArray)){
            throw new Cesium.DeveloperError('index buffer is null');
        }

        let indexDataType = Cesium.IndexDatatype.UNSIGNED_SHORT;
        if((indexPackage.indexType === 1 || verticesCount >= Cesium.Math.SIXTY_FOUR_KILOBYTES) && context.elementIndexUint) {
            indexDataType = Cesium.IndexDatatype.UNSIGNED_INT;
        }

        indexPackage.indexBuffer = Cesium.Buffer.createIndexBuffer({
            context : context,
            typedArray : indexPackage.indicesTypedArray,
            usage : Cesium.BufferUsage.STATIC_DRAW,
            indexDatatype : indexDataType
        });

        indexPackage.indicesTypedArray = null;
        delete indexPackage.indicesTypedArray;
    };

    return S3MCreateIndexBufferJob;
});
