define([
],function(
    ){
    "use strict";

    function DDSTexture(context, id, source) {
        let gl = context._gl;
        this.id = id;
        this._size = source.imageBuffer.length;
        this._context = context;
        this._width = source.width;
        this._height = source.height;
        this._texture = null;
        this._internalFormat = source.internalFormat;
        this._wrapS = Cesium.defaultValue(source.wrapS, Cesium.TextureWrap.CLAMP_TO_EDGE);
        this._wrapT = Cesium.defaultValue(source.wrapT, Cesium.TextureWrap.CLAMP_TO_EDGE);
        this.id = id;
        this._target = gl.TEXTURE_2D;
        this._texture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, this._texture);

        let offset = 0;
        let i = 0;
        let bMipMap = validateMipmap(source.imageBuffer, this._internalFormat, this._width, this._height);
        let texWidth = this._width;
        let texHeight = this._height;
        do{
            let levelSize = Cesium.PixelFormat.compressedTextureSizeInBytes(this._internalFormat, texWidth, texHeight);
            let subArrayBuffer = new Uint8Array(source.imageBuffer.buffer, source.imageBuffer.byteOffset + offset, levelSize);
            gl.compressedTexImage2D(gl.TEXTURE_2D, i++, this._internalFormat, texWidth, texHeight, 0, subArrayBuffer);
            texWidth = Math.max(texWidth >> 1,1);
            texHeight = Math.max(texHeight >> 1,1);
            offset += levelSize;
        }
        while (offset < source.imageBuffer.length && bMipMap);

        if (i > 1) {
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
        }
        else {
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        }

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, this._wrapS);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, this._wrapT);
        gl.bindTexture(gl.TEXTURE_2D, null);
    }

    function validateMipmap(buffer, pixelFormat, width, height){
        let len = buffer.length;
        let w = width, h = height;
        let totalBytes = 0;
        while(1){
            let sizeInBytes = Cesium.PixelFormat.compressedTextureSizeInBytes(pixelFormat, w, h);
            totalBytes += sizeInBytes;
            w = w >> 1;
            h = h >> 1;
            if(w == 0 && h == 0){
                break;
            }

            w = Math.max(w,1);
            h = Math.max(h,1);
        }

        return totalBytes === len;
    }

    DDSTexture.prototype.enable = function(){
        let gl = this._context._gl;
        gl.activeTexture(this._gl.TEXTURE0);
        gl.bindTexture(this._target, this._texture);
    };

    DDSTexture.prototype.disable = function() {
        let gl = this._context._gl;
        gl.bindTexture(this._target, null);
    };

    DDSTexture.prototype.isDestroyed = function() {
        return false;
    };

    DDSTexture.prototype.destroy = function() {
        let gl = this._context._gl;
        gl.deleteTexture(this._texture);
        this._texture = null;
        this.id = 0;
        Cesium.destroyObject(this);
    };

    return DDSTexture;
});