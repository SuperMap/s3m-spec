function MaterialPass(){
    this.ambientColor = new Cesium.Color();
    this.diffuseColor = new Cesium.Color();
    this.specularColor = new Cesium.Color(0.0, 0.0, 0.0, 0.0);
    this.shininess = 50.0;
    this.bTransparentSorting = false;
    this.alphaMode = undefined;
    this.texMatrix = Cesium.Matrix4.clone(Cesium.Matrix4.IDENTITY, new Cesium.Matrix4());
    this.textures = [];
    this._RGBTOBGR = false;
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


MaterialPass.prototype.createWebp = async function(keyword, context, textureInfo) {
    let mipmapEnabled = Cesium.defaultValue(textureInfo.mipmapEnabled, true);
    let imageBuffer = textureInfo.arrayBufferView;
    let wrapS = textureInfo.wrapS;
    let wrapT = textureInfo.wrapT;
    var totalLength = imageBuffer.length;
    var offset = imageBuffer.byteOffset;
    var view = new DataView(imageBuffer.buffer);
    var mipmapLen = 0;
    var promises = [];
    while (mipmapLen < totalLength) {
        var len = view.getUint32(offset, true);
        offset += Uint32Array.BYTES_PER_ELEMENT;
        mipmapLen += Uint32Array.BYTES_PER_ELEMENT;
        var webpBuffer = imageBuffer.subarray(mipmapLen, mipmapLen + len);
        offset += len;
        mipmapLen += len;
        var promise = Cesium.loadImageFromTypedArray({
            uint8Array: webpBuffer,
            format: 'image/webp'
        });
        promises.push(promise);
    }

    var that = this;
    return Promise.all(promises).then((imgs)=>{
        var img = imgs[0];
        if(Cesium.defined(img)){
            var mipMap = true;
            if(!Cesium.Math.isPowerOfTwo(img.width) || !Cesium.Math.isPowerOfTwo(img.height)){
                wrapS = Cesium.TextureWrap.CLAMP_TO_EDGE;
                wrapT = Cesium.TextureWrap.CLAMP_TO_EDGE;
                mipMap = false;
            }
            mipMap = mipMap;
            var texture = new Cesium.Texture({
                context : context,
                source : img,
                pixelFormat : Cesium.PixelFormat.RGBA,
                flipY : false,
                sampler : new Cesium.Sampler({
                    wrapS : wrapS,
                    wrapT : wrapT,
                    minificationFilter : (imgs.length === 0 || !mipMap) ? Cesium.TextureMinificationFilter.LINEAR : Cesium.TextureMinificationFilter.LINEAR_MIPMAP_LINEAR,
                    magnificationFilter : Cesium.TextureMagnificationFilter.LINEAR
                })
            });
            context.textureCache.addTexture(keyword, texture);
            that.textures.push(texture);
        }

        var gl = context._gl;
        var pixelFormat = Cesium.PixelFormat.RGBA;
        var pixelDatatype = Cesium.PixelDatatype.UNSIGNED_BYTE;
        if(imgs.length > 1 && mipmapEnabled){
            gl.activeTexture(gl.TEXTURE0);
            gl.bindTexture(gl.TEXTURE_2D, texture._texture);
            for(var i = 1,j = imgs.length;i < j;i++){
                var source = imgs[i];
                gl.texImage2D(gl.TEXTURE_2D, i, pixelFormat, pixelFormat, pixelDatatype, source);
            }
            gl.bindTexture(gl.TEXTURE_2D, null);
        }
    })
};

export default MaterialPass;