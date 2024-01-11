import RangeMode from "./Enum/RangeMode.js";
import S3MTile from "./S3MTile.js";

function S3MPageLod() {
   this.fileName = undefined;
   this.rangeData = undefined;
   this.rangeMode = undefined;
   this.boundingVolume = undefined;
   this.isLeafTile = false;
   this.renderEntities = [];
   this.childTile = undefined;
   this.canRefine = false;
   this.ready = false;
}

S3MPageLod.prototype.update = function(frameState, layer) {
   const context = frameState.context;
   const boundingVolume = this.boundingVolume;
   const rangeMode = this.rangeMode;
   const rangeData = this.rangeData;
   const lodRangeScale = layer.lodRangeScale;
   const distanceToCamera = boundingVolume.distanceToCamera(frameState);
   if(rangeMode === RangeMode.Distance) {
      this.canRefine = distanceToCamera * lodRangeScale < rangeData;
   }
   else if(rangeMode === RangeMode.Pixel){
      const radius = boundingVolume.radius;
      const center = boundingVolume.center;
      const height = frameState.context.drawingBufferHeight;
      const theta = frameState.camera.frustum._fovy * 0.5;
      const screenYPix = height * 0.5;
      const lamat = screenYPix / Math.tan(theta);
      const pixel = lamat * radius / distanceToCamera;
      this.canRefine = pixel > rangeData * lodRangeScale;
   }
   else if(rangeMode === RangeMode.GeometryError) {
      const resolutionScale = context.uniformState.pixelRatio;
      const height = context.drawingBufferHeight;
      const geometricError = rangeData;
      const pixelSize = (geometricError * height) / (distanceToCamera * frameState.camera.frustum.sseDenominator) / resolutionScale;
      // this.canRefine = (pixelSize > geometricError * lodRangeScale);
      this.canRefine = (pixelSize > 16 * lodRangeScale);
   }
};

S3MPageLod.prototype.createChildTile = function(layer, parentTile) {
   const boundingVolume = this.boundingVolume;
   const tile = new S3MTile(layer, parentTile, boundingVolume, this.fileName);
   this.childTile = tile;
};

S3MPageLod.prototype.isDestroyed = function() {
   return false;
};
S3MPageLod.prototype.destroy = function(){
   for(let i = 0,j = this.renderEntities.length;i < j;i++){
      const ro = this.renderEntities[i];
      ro.destroy();
   }

   this.renderEntities.length = 0;
   this.childTile = undefined;
   return Cesium.destroyObject(this);
};

export default S3MPageLod;