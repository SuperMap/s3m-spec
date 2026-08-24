import GaussianRenderVS from '../Shaders/gaussianSplatVS.js';
import GaussianRenderFS from '../Shaders/gaussianSplatFS.js';
import RenderEntity from '../Factory/RenderEntity.js';
import GaussianSplatMesh from './GaussianSplatMesh.js';

function S3MGaussianRenderEntity(options) {
    RenderEntity.call(this, options);
    this.vs = GaussianRenderVS;
    this.fs = GaussianRenderFS;
    this.gaussianSplatMesh = new GaussianSplatMesh(this);
    this.layer._hasGaussianSplatting = true;
    this.extensions = options.extensions ?? {};
}

S3MGaussianRenderEntity.prototype = Object.create( RenderEntity.prototype );

S3MGaussianRenderEntity.prototype.constructor = S3MGaussianRenderEntity;

// ready 必须在 prototype 替换之后定义。RenderEntity 构造函数里会赋 this.ready = false，
// getter-only 属性在严格模式（ES module）下赋值会抛 TypeError，因此补一个 no-op setter：
// 既允许基类赋值（不产生实例自有属性遮蔽 getter），ready 读取时始终实时反映 mesh 状态
Object.defineProperties(S3MGaussianRenderEntity.prototype, {
    ready : {
        get : function() {
            return this.gaussianSplatMesh.ready;
        },
        set : function() {
            // 忽略基类对 ready 的赋值
        }
    }
});

S3MGaussianRenderEntity.prototype.createCommand = function() {
    if(Cesium.defined(this.colorCommand) || this.vertexBufferToCreate.length !== 0 || this.indexBufferToCreate.length !== 0 || this.shaderProgramToCreate.length !== 0) {
        return ;
    }
    this.gaussianSplatMesh.createCommand();
};
S3MGaussianRenderEntity.prototype.transformResource = function(frameState, layer) {
    if(this.ready){
        return ;
    }
    this.createBatchTable(frameState);
    this.createPickIds();
    this.gaussianSplatMesh.createResources(frameState, layer);
    this.initLayerSetting(layer);
};

S3MGaussianRenderEntity.prototype.update = function(frameState, layer) {
    this.gaussianSplatMesh.update();
    if(this.batchTableDirty){
        this.updateBatchTableAttributes();
        this.batchTableDirty = false;
    }
    if(this.batchTable){
        this.batchTable.update(frameState);
    }
    frameState.commandList.push(this.colorCommand);
};

S3MGaussianRenderEntity.prototype.isGaussianSplatting = function() {
    return this.extensions && Cesium.defined(this.extensions.GaussianSplatting) ? this.extensions.GaussianSplatting.isGaussianSplatting : false;
};

S3MGaussianRenderEntity.prototype.getGaussianSplattingVersion = function() {
    if(this.isGaussianSplatting()){
        return this.extensions.GaussianSplatting.version;
    }
    return undefined;
};

S3MGaussianRenderEntity.prototype.isDestroyed = function() {
    return false;
};

S3MGaussianRenderEntity.prototype.destroy = function() {
    this.gaussianSplatMesh = this.gaussianSplatMesh && this.gaussianSplatMesh.destroy();
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

export default S3MGaussianRenderEntity;
