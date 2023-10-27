import S3MCreateVertexJob from '../S3MCreateVertexJob.js';
import S3MCreateIndexJob from '../S3MCreateIndexJob.js';
import S3MCreateShaderProgramJob from '../S3MCreateShaderProgramJob.js';
import OperationType from '../Enum/OperationType.js';
import ProgramDefines from '../Enum/ProgramDefines.js';
import MaterialPass from '../MaterialPass.js';

function RenderEntity(options) {
    this.layer = options.layer;
    this.vertexPackage = options.vertexPackage;
    this.arrIndexPackage = options.arrIndexPackage;
    this.vertexBufferToCreate = new Cesium.Queue();
    this.indexBufferToCreate = new Cesium.Queue();
    this.shaderProgramToCreate = new Cesium.Queue();
    let i, j;
    for(i = 0, j = this.vertexPackage.vertexAttributes.length;i < j;i++){
        this.vertexBufferToCreate.enqueue(i);
    }

    for(i = 0, j = this.arrIndexPackage.length;i < j;i++){
        this.indexBufferToCreate.enqueue(i);
    }

    this.shaderProgramToCreate.enqueue(0);

    this.boundingVolume = options.boundingVolume;
    this.material = Cesium.defaultValue(options.material, new MaterialPass());
    this.geoName = options.geoName;
    this.modelMatrix = options.modelMatrix;
    this.geoMatrix = options.geoMatrix;
    this.invGeoMatrix = Cesium.Matrix4.inverse(this.geoMatrix, new Cesium.Matrix4());
    this.instanceCount = options.vertexPackage.instanceCount;
    this.attributeLocations = options.vertexPackage.attrLocation;
    this.shaderProgram = undefined;
    this.vertexArray = undefined;
    this.colorCommand = undefined;
    this.pickInfo = options.pickInfo;
    this.selectionInfoMap = new Cesium.AssociativeArray();
    this.batchTable = undefined;
    this.batchTableDirty = false;
    this.pickColorIdentifier = 'vSecondColor';

    this.idsOperationMap = new Cesium.AssociativeArray();
    this.idsColorMap = new Cesium.AssociativeArray();

    this.useWValue = options.vertexPackage.vertexAttributes[0].componentsPerAttribute === 4;

    this.edgeGeometry = options.edgeGeometry;

    this.createBoundingBoxForInstance();
    this.ready = false;
}


const _vertexBufferJob = new S3MCreateVertexJob();
const _indexBufferJob = new S3MCreateIndexJob();
const _shaderProgramJob = new S3MCreateShaderProgramJob();

function createVertexBuffers(renderEntity, frameState) {
    let context = renderEntity.layer.context;
    let queue = renderEntity.vertexBufferToCreate;
    while(queue.length){
        let index = queue.peek();
        _vertexBufferJob.set(context, renderEntity, index);
        if (!frameState.jobScheduler.execute(_vertexBufferJob, Cesium.JobType.BUFFER)) {
            break;
        }

        queue.dequeue();
    }
}

function createIndexBuffers(renderEntity, frameState) {
    let context = renderEntity.layer.context;
    let queue = renderEntity.indexBufferToCreate;
    while(queue.length){
        let index = queue.peek();
        _indexBufferJob.set(context, renderEntity, index);
        if (!frameState.jobScheduler.execute(_indexBufferJob, Cesium.JobType.BUFFER)) {
            break;
        }

        queue.dequeue();
    }
}

function createShaderProgram(renderEntity, frameState) {
    let context = renderEntity.layer.context;
    let queue = renderEntity.shaderProgramToCreate;
    while(queue.length){
        let index = queue.peek();
        _shaderProgramJob.set(context, renderEntity);
        if (!frameState.jobScheduler.execute(_shaderProgramJob, Cesium.JobType.PROGRAM)) {
            break;
        }

        queue.dequeue();
    }
}

function createBatchTable(renderEntity, frameState) {
    if(Cesium.defined(renderEntity.batchTable) || !renderEntity.pickInfo){
        return ;
    }

    const context = renderEntity.layer.context;
    let attributes = [];
    attributes.push({
        functionName : 's3m_batchTable_color',
        componentDatatype : Cesium.ComponentDatatype.UNSIGNED_BYTE,
        componentsPerAttribute : 4,
        normalize : true
    },{
        functionName : 's3m_batchTable_operation',
        componentDatatype : Cesium.ComponentDatatype.UNSIGNED_BYTE,
        componentsPerAttribute : 4
    },{
        functionName : 's3m_batchTable_pickColor',
        componentDatatype : Cesium.ComponentDatatype.UNSIGNED_BYTE,
        componentsPerAttribute : 4,
        normalize : true
    });

    let pickInfo = renderEntity.pickInfo;
    let pickIds = Object.keys(pickInfo);
    let numberOfInstances = renderEntity.instanceCount > 0 ? renderEntity.instanceCount : pickIds.length;
    renderEntity.batchTable = new Cesium.BatchTable(context, attributes, numberOfInstances);
}

RenderEntity.prototype.createBuffers = function(frameState) {
    createVertexBuffers(this, frameState);
    createIndexBuffers(this, frameState);
};

RenderEntity.prototype.createShaderProgram = function(frameState) {
    createShaderProgram(this, frameState);
};

RenderEntity.prototype.createBatchTable = function(frameState) {
    createBatchTable(this, frameState);
};

let scratchPntCenter = new Cesium.Cartesian3();
RenderEntity.prototype.createBoundingBoxForInstance = function() {
    const vertexPackage = this.vertexPackage;
    if(!Cesium.defined(vertexPackage) || vertexPackage.instanceIndex === -1 || !Cesium.defined(vertexPackage.instanceBounds)){
        return;
    }

    let instanceBounds = vertexPackage.instanceBounds;
    let pntLU = new Cesium.Cartesian3(instanceBounds[0], instanceBounds[1], instanceBounds[2]);
    let pntRD = new Cesium.Cartesian3(instanceBounds[3], instanceBounds[4], instanceBounds[5]);
    let pntCenter = Cesium.Cartesian3.lerp(pntLU, pntRD, 0.5, scratchPntCenter);
    let dRadius = Cesium.Cartesian3.distance(pntCenter, pntLU);
    let vecCenter = new Cesium.Cartesian3();
    Cesium.Matrix4.multiplyByPoint(this.modelMatrix, pntCenter, vecCenter);
    this.boundingVolume.center = vecCenter;
    this.boundingVolume.radius = dRadius;
    vertexPackage.instanceBounds = undefined;
};

let cartesian4Scratch = new Cesium.Cartesian4();

RenderEntity.prototype.updateBatchTableAttributes = function(){
    let ro = this;
    let idsColorMap = this.idsColorMap;
    let byte = [];
    for(let i = 0,j = idsColorMap.length;i < j;i++){
        let obj = idsColorMap.values[i];
        if(!obj.dirty){
            continue ;
        }

        obj.dirty = false;
        byte = obj.color.toBytes();
        cartesian4Scratch.x = byte[0];
        cartesian4Scratch.y = byte[1];
        cartesian4Scratch.z = byte[2];
        cartesian4Scratch.w = byte[3];
        if(Cesium.defined(obj.batchId)){
            this.batchTable.setBatchedAttribute(obj.batchId, 0, cartesian4Scratch);
        }
        else if(Array.isArray(obj.instanceIds)){
            obj.instanceIds.map(function(instanceId){
                ro.batchTable.setBatchedAttribute(instanceId, 0, cartesian4Scratch);
            });
        }

    }

    let idsOperationMap = this.idsOperationMap;
    for(let i = 0,j = idsOperationMap.length;i < j;i++){
        let obj = idsOperationMap.values[i];
        if(!obj.dirty) {
            continue ;
        }

        obj.dirty = false;
        if(this.instanceCount > 0){
            if(Array.isArray(obj.instanceIds)){
                obj.instanceIds.map(function(instanceId){
                    ro.batchTable.setBatchedAttribute(instanceId, 1, obj.operationValue);
                });
            }
        }
        else{
            if(Cesium.defined(obj.batchId)){
                this.batchTable.setBatchedAttribute(obj.batchId, 1, obj.operationValue);
            }
        }
    }
};


RenderEntity.prototype.transformResource = Cesium.DeveloperError.throwInstantiationError;

RenderEntity.prototype.createCommand = Cesium.DeveloperError.throwInstantiationError;

RenderEntity.prototype.update = Cesium.DeveloperError.throwInstantiationError;

RenderEntity.prototype.isDestroyed = Cesium.DeveloperError.throwInstantiationError;

RenderEntity.prototype.destroy = Cesium.DeveloperError.throwInstantiationError;

export default RenderEntity;