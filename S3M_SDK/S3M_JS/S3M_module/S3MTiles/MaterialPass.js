function MaterialPass(){
    this.ambientColor = new Cesium.Color();
    this.diffuseColor = new Cesium.Color();
    this.specularColor = new Cesium.Color(0.0, 0.0, 0.0, 0.0);
    this.shininess = 50.0;
    this.bTransparentSorting = false;
    this.alphaMode = undefined;
    this.texMatrix = Cesium.Matrix4.clone(Cesium.Matrix4.IDENTITY, new Cesium.Matrix4());
    this.textures = [];
}

MaterialPass.prototype.isDestroyed = function() {
    return false;
};

MaterialPass.prototype.destroy = function(){
    let length = this.textures.length;
    for(let i = 0;i < length;i++){
        let texture = this.textures[i];
        texture.destroy();
    }

    this.textures.length = 0;
    this.ambientColor = undefined;
    this.diffuseColor = undefined;
    this.specularColor = undefined;
    return Cesium.destroyObject(this);
};

MaterialPass.prototype.createCRN = function(textureCode,textureName,context, index, imgObj, wrapS, wrapT, isStandard, mipmapEnabled, textureTypeObj) {
    var promise;
    mipmapEnabled = defaultValue(mipmapEnabled, true);
    if (isStandard) {
        promise = loadCRN(imgObj.imageBuffer, true, true);
    } else {
        if (S3MTaskManager.CRNTaskProcessorReady) {
            promise = loadCRNForS3M(S3MTaskManager.CRNProcessor, imgObj.imageBuffer, true);
        }
    }
    if(!defined(promise)){
        return;
    }
    var that = this;
    promise.then(function(img) {
        if(that.isDestroyed()){
            return ;
        }
        textureTypeObj = defaultValue(textureTypeObj, {});
        var texture = DDSTextureManager.CreateTexture(textureCode, context,imgObj.width, imgObj.height,imgObj.nFormat, S3MCompressType.enrS3TCDXTN, img.bufferView,false, wrapS, wrapT, mipmapEnabled);
        if(textureTypeObj.isEmissiveTex){
            that.emissiveTexture = texture;
        }
        else if(textureTypeObj.isNormalTexture){
            that.normalTexture = texture;
        }
        else{
            if(index === 0 && that._textures.length > 0){
                that._textures.splice(0,0,texture);
            }
            else{
                that._textures.push(texture);
            }
        }

    });
    return promise;
};

export default MaterialPass;