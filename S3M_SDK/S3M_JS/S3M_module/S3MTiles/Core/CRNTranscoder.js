function CRNTranscoder() {}

CRNTranscoder.wasmReady = false;
CRNTranscoder.crunch = undefined;
CRNTranscoder.loading = false;

CRNTranscoder.init = function() {
    if(CRNTranscoder.loading) {
        return ;
    }
    import('../ThirdParty/crunch.js').then((module) => {
        CRNTranscoder.loading = false;
        CRNTranscoder.crunch = module.default;
        if(Object.keys(CRNTranscoder.crunch.asm).length > 0){
            CRNTranscoder.wasmReady = true;
        }
        CRNTranscoder.crunch.onRuntimeInitialized = function() {
            CRNTranscoder.wasmReady = true;
        };
    });

    CRNTranscoder.loading = true;
};

const CRN_FORMAT = {
    cCRNFmtInvalid: -1,
    cCRNFmtDXT1: 0,
    cCRNFmtDXT3: 1,
    cCRNFmtDXT5: 2
};

const DXT_FORMAT_MAP = {};
DXT_FORMAT_MAP[CRN_FORMAT.cCRNFmtDXT1] = Cesium.PixelFormat.RGB_DXT1;
DXT_FORMAT_MAP[CRN_FORMAT.cCRNFmtDXT3] = Cesium.PixelFormat.RGBA_DXT3;
DXT_FORMAT_MAP[CRN_FORMAT.cCRNFmtDXT5] = Cesium.PixelFormat.RGBA_DXT5;

var dst;
var dxtData;
var cachedDstSize = 0;

function arrayBufferCopy(src, dst, dstByteOffset, numBytes) {
    var i;
    var dst32Offset = dstByteOffset / 4;
    var tail = (numBytes % 4);
    var src32 = new Uint32Array(src.buffer, 0, (numBytes - tail) / 4);
    var dst32 = new Uint32Array(dst.buffer);
    for (i = 0; i < src32.length; i++) {
        dst32[dst32Offset + i] = src32[i];
    }
    for (i = numBytes - tail; i < numBytes; i++) {
        dst[dstByteOffset + i] = src[i];
    }
}


CRNTranscoder.transcode = function(parameters) {
    const crunch = CRNTranscoder.crunch;
    // Copy the contents of the arrayBuffer into emscriptens heap.
    var arrayBuffer = parameters.data;
    var srcSize = arrayBuffer.byteLength;
    var bytes = new Uint8Array(arrayBuffer);
    var src = crunch._malloc(srcSize);
    arrayBufferCopy(bytes, crunch.HEAPU8, src, srcSize);

    // Determine what type of compressed data the file contains.
    var crnFormat = crunch._crn_get_dxt_format(src, srcSize);
    var format = DXT_FORMAT_MAP[crnFormat];
    if (!Cesium.defined(format)) {
        throw new Cesium.RuntimeError('Unsupported compressed format.');
    }

    // Gather basic metrics about the DXT data.
    var levels = crunch._crn_get_levels(src, srcSize);
    var width = crunch._crn_get_width(src, srcSize);
    var height = crunch._crn_get_height(src, srcSize);

    // Determine the size of the decoded DXT data.
    var dstSize = 0;
    var i;
    for (i = 0; i < levels; ++i) {
        dstSize += Cesium.PixelFormat.compressedTextureSizeInBytes(format, width >> i, height >> i);
    }

    // Allocate enough space on the emscripten heap to hold the decoded DXT data
    // or reuse the existing allocation if a previous call to this function has
    // already acquired a large enough buffer.
    if(cachedDstSize < dstSize) {
        if(Cesium.defined(dst)) {
            crunch._free(dst);
        }
        dst = crunch._malloc(dstSize);
        dxtData = new Uint8Array(crunch.HEAPU8.buffer, dst, dstSize);
        cachedDstSize = dstSize;
    }

    // Decompress the DXT data from the Crunch file into the allocated space.
    crunch._crn_decompress(src, srcSize, dst, dstSize, 0, levels);

    // Release the crunch file data from the emscripten heap.
    crunch._free(src);

    var bOutMipMapData = Cesium.defaultValue(parameters.bMipMap, false);
    if(bOutMipMapData){
        var dXTDataMipMap = dxtData.slice(0, dstSize);
        if(Cesium.CompressedTextureBuffer.length === 4){
            return new Cesium.CompressedTextureBuffer(format, width, height, dXTDataMipMap);
        }
        return new Cesium.CompressedTextureBuffer(format, Cesium.PixelDatatype.UNSIGNED_BYTE, width, height, dXTDataMipMap);
    }
    else {
        // Mipmaps are unsupported, so copy the level 0 texture
        // When mipmaps are supported, a copy will still be necessary as dxtData is a view on the heap.
        var length = Cesium.PixelFormat.compressedTextureSizeInBytes(format, width, height);

        // Get a copy of the 0th mip level. dxtData will exceed length when there are more mip levels.
        // Equivalent to dxtData.slice(0, length), which is not supported in IE11
        var level0DXTDataView = dxtData.subarray(0, length);
        var level0DXTData = new Uint8Array(length);
        level0DXTData.set(level0DXTDataView, 0);
        if(Cesium.CompressedTextureBuffer.length === 4){
            return new Cesium.CompressedTextureBuffer(format, width, height, level0DXTData);
        }
        return new Cesium.CompressedTextureBuffer(format, Cesium.PixelDatatype.UNSIGNED_BYTE, width, height, level0DXTData);
    }
};
export default CRNTranscoder;