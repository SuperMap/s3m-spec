import pako from './pako_inflate.js';
import DXTTextureDecode from './DXTTextureDecode.js';
import { parseMeshOptSkeleton, parseMeshOpIndexPackage } from './ParseMeshOpt.js'
import dracoDecoderModule from './draco_decode.module.js';
import { parseDracoSkeleton } from './ParseDraco.js'

function S3ModelParser() {

}

function defer() {
    let resolve;
    let reject;
    const promise = new Promise(function (res, rej) {
      resolve = res;
      reject = rej;
    });
  
    return {
      resolve: resolve,
      reject: reject,
      promise: promise,
    };
  }

function loadArrayBuffer(url) {
    let readyPromise = defer();
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'arraybuffer';
    xhr.onload = function() {
        if (xhr.status < 200 || xhr.status >= 300) {
            readyPromise.reject(xhr.response)
            throw new Error(xhr.response)
        }
        readyPromise.resolve(xhr.response)
    };
    xhr.onerror = function(e) {
        readyPromise.reject(new Error(e));
    };
    xhr.send();
    return readyPromise.promise;
}

let dracoLib;
function initDracoLib() {
    if(dracoLib) return;
    loadArrayBuffer('S3M_module/S3MParser/draco_decoder_new.wasm').then(function (arrayBuffer) {
        dracoDecoderModule({wasmBinary: arrayBuffer}).then(function (compiledModule) {
            dracoLib = compiledModule;
        })
    })
}
initDracoLib();

S3ModelParser.s3tc = true;
S3ModelParser.pvrtc = false;
S3ModelParser.etc1 = false;

const S3MBVertexTag = {
    SV_Unkown : 0,
    SV_Standard : 1,
    SV_Compressed : 2,
    SV_DracoCompressed: 3
};

const S3MBVertexTagV3 = {
    Standard : 0,
    Draco : 1,
    MeshOpt : 2
};

const AttrTypeMap = {
    0 : Uint32Array.BYTES_PER_ELEMENT,
    1 : Float32Array.BYTES_PER_ELEMENT,
    2 : Float64Array.BYTES_PER_ELEMENT,
};

const S3MPixelFormat = {
    NONE: 0,
    LUMINANCE_8 : 1,
    LUMINANCE_16 : 2,
    ALPHA : 3,
    ALPHA_4_LUMINANCE_4 : 4,
    LUMINANCE_ALPHA : 5,
    RGB_565 : 6,
    BGR565 : 7,
    RGB : 10,
    BGR : 11,
    ARGB : 12,
    ABGR : 13,
    BGRA : 14,
    WEBP : 25,
    RGBA : 28,
    DXT1 : 17,
    DXT2 : 18,
    DXT3 : 19,
    DXT4 : 20,
    DXT5 : 21,
    CRN_DXT5 : 26,
    STANDARD_CRN : 27,
    KTX2 :31
};

const VertexCompressOption = {
    SVC_Vertex : 1,
    SVC_Normal : 2,
    SVC_VertexColor : 4,
    SVC_SecondColor	: 8,
    SVC_TexutreCoord : 16,
    SVC_TexutreCoordIsW	: 32
};

const AttributeType = {
    Invalid : 0,
    Position : 1,
    PositionW : 2,
    Normal : 4,
    Tangent : 8,
    FirstTexcoord : 16,
    SecondTexcoord : 32,
    Color : 64,
    SecondColor : 128,
    Custom0 : 512,
    Custom1 : 1024
};

function unZip(buffer, bytesOffset) {
    let dataZip = new Uint8Array(buffer, bytesOffset);
    return pako.inflate(dataZip).buffer;
}

function getStringFromTypedArray(uint8Array, byteOffset, byteLength) {
    var byteOffset = 0;
    var byteLength = uint8Array.byteLength;
    var codeType = 'utf-8';

    uint8Array = uint8Array.subarray(byteOffset, byteOffset + byteLength);

    var decoder = new TextDecoder(codeType);
    return decoder.decode(uint8Array);
}

function parseString(buffer, view, bytesOffset) {
    let length = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let stringBufferView = new Uint8Array(buffer, bytesOffset, length);
    let string = getStringFromTypedArray(stringBufferView);
    bytesOffset += length;

    return {
        string : string,
        bytesOffset : bytesOffset,
        length : length
    }
}

function parseGeode(buffer, view, bytesOffset, geodes) {
    let geode = {};
    let skeletonNames = [];
    let geoMatrix = new Array(16);
    for(let i = 0; i < 16; i++){
        geoMatrix[i] = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
    }

    geode.matrix = geoMatrix;
    geode.skeletonNames = skeletonNames;
    let skeletonCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < skeletonCount; i++){
        let res = parseString(buffer, view, bytesOffset);
        skeletonNames.push(res.string);
        bytesOffset = res.bytesOffset;
    }

    geodes.push(geode);

    return bytesOffset;
}

function parsePageLOD(buffer, view, bytesOffset, pageLods, version) {
    let pageLOD = {};
    pageLOD.rangeList = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;
    pageLOD.rangeMode = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let center = {};
    center.x = view.getFloat64(bytesOffset, true);
    bytesOffset += Float64Array.BYTES_PER_ELEMENT;
    center.y = view.getFloat64(bytesOffset, true);
    bytesOffset += Float64Array.BYTES_PER_ELEMENT;
    center.z = view.getFloat64(bytesOffset, true);
    bytesOffset += Float64Array.BYTES_PER_ELEMENT;
    let radius = view.getFloat64(bytesOffset, true);
    bytesOffset += Float64Array.BYTES_PER_ELEMENT;
    pageLOD.boundingSphere = {
        center : center,
        radius : radius
    };

    if(version >= 3){
        const obbCenter = {};
        obbCenter.x = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        obbCenter.y = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        obbCenter.z = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;

        const xExtent = {};
        xExtent.x = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        xExtent.y = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        xExtent.z = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;

        const yExtent = {};
        yExtent.x = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        yExtent.y = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        yExtent.z = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;

        const zExtent = {};
        zExtent.x = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        zExtent.y = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        zExtent.z = view.getFloat64(bytesOffset, true);
        bytesOffset += Float64Array.BYTES_PER_ELEMENT;

        pageLOD.obb = {
            xExtent: xExtent,
            yExtent: yExtent,
            zExtent: zExtent,
            center: obbCenter
        };
    }

    let res = parseString(buffer, view, bytesOffset);
    let strChildTile = res.string;
    bytesOffset = res.bytesOffset;
    let index = strChildTile.indexOf('Geometry');
    if(index !== -1){
        let ignoreString = strChildTile.substring(index);
        strChildTile = strChildTile.replace(ignoreString, '');
    }

    pageLOD.childTile = strChildTile;
    pageLOD.geodes = [];
    let geodeCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < geodeCount; i++){
        bytesOffset = parseGeode(buffer, view, bytesOffset, pageLOD.geodes);
    }

    pageLods.push(pageLOD);

    //animations
    if(version >= 3){
        let resAnimations = parseString(buffer, view, bytesOffset);
        bytesOffset = resAnimations.bytesOffset;
    }

    return bytesOffset;
}

function parseGroupNode(buffer, view, bytesOffset, result) {
    let groupNode = {};
    let geodes = [];
    let pageLods = [];
    let size = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let count = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < count; i++){
        bytesOffset = parsePageLOD(buffer, view, bytesOffset, pageLods, result.version);
    }
    groupNode.pageLods = pageLods;
    let align = bytesOffset % 4;
    if(align !== 0){
        bytesOffset += (4 - align);
    }

    result.groupNode = groupNode;

    return bytesOffset;
}

function parseVertex(buffer, view, bytesOffset, vertexPackage) {
    let verticesCount = view.getUint32(bytesOffset, true);
    vertexPackage.verticesCount = verticesCount;
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if(bytesOffset <= 0){
        return bytesOffset;
    }

    let vertexDimension = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let vertexStride = view.getUint16(bytesOffset, true);
    vertexStride = vertexDimension * Float32Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let byteLength = verticesCount * vertexDimension * Float32Array.BYTES_PER_ELEMENT;
    let vertexBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    bytesOffset += byteLength;
    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aPosition'] = attributes.length;
    attributes.push({
        index: attrLocation['aPosition'],
        typedArray: vertexBuffer,
        componentsPerAttribute: vertexDimension,
        componentDatatype: 5126,
        offsetInBytes: 0,
        strideInBytes: vertexStride,
        normalize: false
    });

    return bytesOffset;
}

function parseNormal(buffer, view, bytesOffset, vertexPackage) {
    let normalCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if(normalCount <= 0){
        return bytesOffset;
    }

    let normalDimension = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let normalStride = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let byteLength = normalCount * normalDimension * Float32Array.BYTES_PER_ELEMENT;
    let normalBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    bytesOffset += byteLength;
    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aNormal'] = attributes.length;
    attributes.push({
        index: attrLocation['aNormal'],
        typedArray: normalBuffer,
        componentsPerAttribute : normalDimension,
        componentDatatype : 5126,
        offsetInBytes: 0,
        strideInBytes: normalStride,
        normalize: false
    });

    return bytesOffset;
}

function parseVertexColor(buffer, view, bytesOffset, vertexPackage) {
    let colorCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let verticesCount = vertexPackage.verticesCount;
    let vertexColor;
    if (colorCount > 0){
        let colorStride = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT * 2;
        let byteLength = colorCount * Uint8Array.BYTES_PER_ELEMENT * 4;
        let typedArray = new Uint8Array(buffer, bytesOffset, byteLength);
        vertexColor = typedArray.slice(0, byteLength);
        bytesOffset += byteLength;
    }
    else{
        vertexColor = new Uint8Array(4 * verticesCount);
        for (let m = 0; m < verticesCount; m++) {
            vertexColor[m * 4] = 255;
            vertexColor[m * 4 + 1] = 255;
            vertexColor[m * 4 + 2] = 255;
            vertexColor[m * 4 + 3] = 255;
        }
    }

    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aColor'] = attributes.length;
    attributes.push({
        index: attrLocation['aColor'],
        typedArray: vertexColor,
        componentsPerAttribute: 4,
        componentDatatype: 5121,
        offsetInBytes: 0,
        strideInBytes: 4,
        normalize: true
    });

    vertexPackage.vertexColor = vertexColor;

    return bytesOffset;
}

function parseSecondColor(buffer, view, bytesOffset, vertexPackage) {
    let secondColorCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if (secondColorCount <= 0){
        return bytesOffset;
    }

    let secondColorStride = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint8Array.BYTES_PER_ELEMENT * 2;
    let byteLength = secondColorCount * Uint8Array.BYTES_PER_ELEMENT * 4;
    let secColorBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    bytesOffset += byteLength;

    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aSecondColor'] = attributes.length;
    attributes.push({
        index: attrLocation['aSecondColor'],
        typedArray: secColorBuffer,
        componentsPerAttribute: 4,
        componentDatatype: 5121,
        offsetInBytes: 0,
        strideInBytes: 4,
        normalize: true
    });

    return bytesOffset;
}

function parseTexCoord(buffer, view, bytesOffset, vertexPackage) {
    let count = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;

    for(let i = 0; i < count; i++) {
        let texCoordCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let dimension = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        let texCoordStride = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        let byteLength = texCoordCount * dimension * Float32Array.BYTES_PER_ELEMENT;
        let texCoordBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
        bytesOffset += byteLength;
        let str = 'aTexCoord' + i;
        let attributes = vertexPackage.vertexAttributes;
        let attrLocation = vertexPackage.attrLocation;
        attrLocation[str] = attributes.length;
        attributes.push({
            index: attrLocation[str],
            typedArray: texCoordBuffer,
            componentsPerAttribute: dimension,
            componentDatatype: 5126,
            offsetInBytes: 0,
            strideInBytes: dimension * Float32Array.BYTES_PER_ELEMENT,
            normalize: false
        });
    }

    return bytesOffset;
}

function parseInstanceInfo(buffer, view, bytesOffset, vertexPackage) {
    let count = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;

    for (let i=0; i < count; i++){
        let texCoordCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let texDimensions = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        let texCoordStride = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        let byteLength = texCoordCount * texDimensions * Float32Array.BYTES_PER_ELEMENT;
        if(texDimensions === 17 || texDimensions === 29){
            let instanceBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
            vertexPackage.instanceCount = texCoordCount;
            vertexPackage.instanceMode = texDimensions;
            vertexPackage.instanceBuffer = instanceBuffer;
            vertexPackage.instanceIndex = 1;
            let len = texDimensions * texCoordCount * 4;
            let vertexColorInstance = instanceBuffer.slice(0, len);
            vertexPackage.vertexColorInstance = vertexColorInstance;
            let byteStride;
            if(texDimensions === 17){
                byteStride = Float32Array.BYTES_PER_ELEMENT * 17;
                attrLocation['uv2'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv2'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 0,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv3'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv3'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 4 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv4'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv4'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 8 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });
                attrLocation['secondary_colour'] = attributes.length;
                attributes.push({
                    index:attrLocation['secondary_colour'],
                    componentsPerAttribute:4,
                    componentDatatype:5126,
                    normalize:false,
                    offsetInBytes:12*Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes:byteStride,
                    instanceDivisor:1
                });
                attrLocation['uv6'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv6'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5121,
                    normalize: true,
                    offsetInBytes: 16 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });
            }
            else if (texDimensions === 29) {
                byteStride = Float32Array.BYTES_PER_ELEMENT * 29;
                attrLocation['uv1'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv1'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 0,
                    strideInBytes: byteStride,
                    instanceDivisor: 1,
                    byteLength: byteLength
                });

                attrLocation['uv2'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv2'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 4 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv3'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv3'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 8 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv4'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv4'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 12 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv5'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv5'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 16 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv6'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv6'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 20 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });

                attrLocation['uv7'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv7'],
                    componentsPerAttribute: 3,
                    componentDatatype: 5126,
                    normalize: false,
                    offsetInBytes: 24 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });
                attrLocation['secondary_colour'] = attributes.length;
                attributes.push({
                    index: attrLocation['secondary_colour'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5121,
                    normalize: true,
                    offsetInBytes: 27 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });
                attrLocation['uv9'] = attributes.length;
                attributes.push({
                    index: attrLocation['uv9'],
                    componentsPerAttribute: 4,
                    componentDatatype: 5121,
                    normalize: true,
                    offsetInBytes: 28 * Float32Array.BYTES_PER_ELEMENT,
                    strideInBytes: byteStride,
                    instanceDivisor: 1
                });
            }
        }
        else{
            let len = texCoordCount * texDimensions;
            vertexPackage.instanceBounds = new Float32Array(len);
            for(let k = 0; k < len; k++){
                vertexPackage.instanceBounds[k] = view.getFloat32(bytesOffset + k * Float32Array.BYTES_PER_ELEMENT, true);
            }
        }

        bytesOffset += byteLength;
    }

    return bytesOffset;
}

function parseVertexAttrExtension(buffer, view, bytesOffset, vertexPackage) {
    const attrCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;

    for(let i = 0;i < attrCount;i++){
        const count = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        const dimension = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        const attrType = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        const byteLength = count * dimension * AttrTypeMap[attrType];
        const attrBuffer = new Uint8Array(buffer, bytesOffset, byteLength)
        bytesOffset += byteLength;

        const attributes = vertexPackage.vertexAttributes;
        const attrLocation = vertexPackage.attrLocation;
        const str = 'aCustom' + i;
        attrLocation[str] = attributes.length;
        attributes.push({
            index: attrLocation[str],
            typedArray: attrBuffer,
            componentsPerAttribute: dimension,
            componentDatatype: 5126,
            offsetInBytes: 0,
            strideInBytes: 0,
            normalize: false
        });
    }

    return bytesOffset;
}

function parseCompressVertex(buffer, view, bytesOffset, vertexPackage) {
    let verticesCount = view.getUint32(bytesOffset, true);
    vertexPackage.verticesCount = verticesCount;
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if(bytesOffset <= 0){
        return bytesOffset;
    }

    let vertexDimension = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let vertexStride = view.getUint16(bytesOffset, true);
    vertexStride = vertexDimension * Int16Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;

    let vertCompressConstant = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;
    let minVerticesValue = {};
    minVerticesValue.x = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;
    minVerticesValue.y = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;
    minVerticesValue.z = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;
    minVerticesValue.w = view.getFloat32(bytesOffset, true);
    bytesOffset += Float32Array.BYTES_PER_ELEMENT;

    vertexPackage.vertCompressConstant = vertCompressConstant;
    vertexPackage.minVerticesValue = minVerticesValue;

    let byteLength = verticesCount * vertexDimension * Int16Array.BYTES_PER_ELEMENT;
    let vertexBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    bytesOffset += byteLength;

    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aPosition'] = attributes.length;
    attributes.push({
        index: attrLocation['aPosition'],
        typedArray: vertexBuffer,
        componentsPerAttribute: vertexDimension,
        componentDatatype: 5122,
        offsetInBytes: 0,
        strideInBytes: vertexStride,
        normalize: false
    });

    return bytesOffset;
}

function parseCompressNormal(buffer, view, bytesOffset, vertexPackage) {
    let normalCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if(normalCount <= 0){
        return bytesOffset;
    }

    let normalDimension = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let normalStride = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    let byteLength = normalCount * 2 * Int16Array.BYTES_PER_ELEMENT;
    let normalBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    bytesOffset += byteLength;
    let attributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    attrLocation['aNormal'] = attributes.length;
    attributes.push({
        index: attrLocation['aNormal'],
        typedArray: normalBuffer,
        componentsPerAttribute : 2,
        componentDatatype : 5122,
        offsetInBytes: 0,
        strideInBytes: normalStride,
        normalize: false
    });

    return bytesOffset;
}

function parseCompressTexCoord(buffer, view, bytesOffset, vertexPackage) {
    vertexPackage.texCoordCompressConstant = [];
    vertexPackage.minTexCoordValue = [];
    let count = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < count; i++){
        let bNeedTexCoordZ = view.getUint8(bytesOffset, true);
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT * 3;
        let texCoordCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let dimension = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
        let texCoordStride = view.getUint16(bytesOffset, true);
        bytesOffset += Uint16Array.BYTES_PER_ELEMENT;

        let texCoordCompressConstant = view.getFloat32(bytesOffset, true);
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        vertexPackage.texCoordCompressConstant.push(texCoordCompressConstant);

        let minTexCoordValue = {};
        minTexCoordValue.x = view.getFloat32(bytesOffset, true);
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        minTexCoordValue.y = view.getFloat32(bytesOffset, true);
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        minTexCoordValue.z = view.getFloat32(bytesOffset, true);
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        minTexCoordValue.w = view.getFloat32(bytesOffset, true);
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        vertexPackage.minTexCoordValue.push(minTexCoordValue);

        let byteLength = texCoordCount * dimension * Int16Array.BYTES_PER_ELEMENT;
        let texCoordBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
        bytesOffset += byteLength;
        let align = bytesOffset % 4;
        if(align !== 0){
            bytesOffset += (4 - align);
        }

        let str = 'aTexCoord' + i;
        let attributes = vertexPackage.vertexAttributes;
        let attrLocation = vertexPackage.attrLocation;
        attrLocation[str] = attributes.length;
        attributes.push({
            index: attrLocation[str],
            typedArray: texCoordBuffer,
            componentsPerAttribute: dimension,
            componentDatatype: 5122,
            offsetInBytes: 0,
            strideInBytes: dimension * Int16Array.BYTES_PER_ELEMENT,
            normalize: false
        });

        if(bNeedTexCoordZ){
            byteLength = texCoordCount * Float32Array.BYTES_PER_ELEMENT;
            let texCoordZBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
            bytesOffset += byteLength;
            vertexPackage.texCoordZMatrix = true;
            str = 'aTexCoordZ' + i;
            attrLocation[str] = attributes.length;
            attributes.push({
                index: attrLocation[str],
                typedArray: texCoordZBuffer,
                componentsPerAttribute: 1,
                componentDatatype: 5126,
                offsetInBytes: 0,
                strideInBytes: Float32Array.BYTES_PER_ELEMENT,
                normalize: false
            });
        }
    }
    return bytesOffset;
}

function parseTangent(buffer, view, bytesOffset, vertexPackage) {
    const nCount = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if (nCount <= 0) {
        return bytesOffset;
    }

    const nDimension = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    const nStride = view.getUint16(bytesOffset, true);
    bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
    const byteLength = nCount * nDimension * Float32Array.BYTES_PER_ELEMENT;
    bytesOffset += byteLength;

    return bytesOffset;
}

function parseStandardSkeleton(buffer, view, bytesOffset, vertexPackage, version) {
    if(version >= 3){
        let streamSize = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    }

    bytesOffset = parseVertex(buffer, view, bytesOffset, vertexPackage);

    bytesOffset = parseNormal(buffer, view, bytesOffset, vertexPackage);

    bytesOffset = parseVertexColor(buffer, view, bytesOffset, vertexPackage);

    if(version < 3){
        bytesOffset = parseSecondColor(buffer, view, bytesOffset, vertexPackage);
    }

    bytesOffset = parseTexCoord(buffer, view, bytesOffset, vertexPackage);

    bytesOffset = parseInstanceInfo(buffer, view, bytesOffset, vertexPackage);

    if(version >= 3){
        bytesOffset = parseVertexAttrExtension(buffer, view, bytesOffset, vertexPackage);

        const describeResult = parseString(buffer, view, bytesOffset);
        bytesOffset = describeResult.bytesOffset;
        vertexPackage.customVertexAttribute = JSON.parse(describeResult.string);
        const attr = 'aCustom' + vertexPackage.customVertexAttribute['TextureCoordMatrix'];
        const attr2 = 'aCustom' + vertexPackage.customVertexAttribute['VertexWeight'];
        if(vertexPackage.attrLocation[attr] !== undefined){
            vertexPackage.attrLocation['aTextureCoordMatrix'] = vertexPackage.attrLocation[attr];
            delete vertexPackage.attrLocation[attr];
        }
        if(vertexPackage.attrLocation[attr2] !== undefined){
            vertexPackage.attrLocation['aVertexWeight'] = vertexPackage.attrLocation[attr2];
            delete vertexPackage.attrLocation[attr2];
        }

        let nAlign = bytesOffset % 4;
        if(nAlign){
            nAlign = 4 - nAlign;
        }

        bytesOffset += nAlign;

        bytesOffset = parseTangent(buffer, view, bytesOffset, vertexPackage);
    }

    return bytesOffset;
}

function parseCompressSkeleton(buffer, view, bytesOffset, vertexPackage) {
    let compressOptions = view.getUint32(bytesOffset, true);
    vertexPackage.compressOptions = compressOptions;
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    if((compressOptions & VertexCompressOption.SVC_Vertex) === VertexCompressOption.SVC_Vertex){
        bytesOffset = parseCompressVertex(buffer, view, bytesOffset, vertexPackage);
    }
    else{
        bytesOffset = parseVertex(buffer, view, bytesOffset, vertexPackage);
    }

    if((compressOptions & VertexCompressOption.SVC_Normal) === VertexCompressOption.SVC_Normal){
        bytesOffset = parseCompressNormal(buffer, view, bytesOffset, vertexPackage);
    }
    else{
        bytesOffset = parseNormal(buffer, view, bytesOffset, vertexPackage);
    }

    bytesOffset = parseVertexColor(buffer, view, bytesOffset, vertexPackage);

    bytesOffset = parseSecondColor(buffer, view, bytesOffset, vertexPackage);

    if((compressOptions & VertexCompressOption.SVC_TexutreCoord) === VertexCompressOption.SVC_TexutreCoord){
        bytesOffset = parseCompressTexCoord(buffer, view, bytesOffset, vertexPackage);
    }
    else{
        bytesOffset = parseTexCoord(buffer, view, bytesOffset, vertexPackage);
    }

    if((compressOptions & VertexCompressOption.SVC_TexutreCoordIsW) === VertexCompressOption.SVC_TexutreCoordIsW){
        vertexPackage.textureCoordIsW = true;
    }

    bytesOffset = parseInstanceInfo(buffer, view, bytesOffset, vertexPackage);

    return bytesOffset;
}

function parseIndexPackage(buffer, view, bytesOffset, arrIndexPackage, version) {
    let count = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for (let i = 0; i < count; i++){
        let indexPackage = {};
        if(version >= 3){
            const streamSize = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        }
        let indexCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let indexType = view.getUint8(bytesOffset, true);
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
        let bUseIndex = view.getUint8(bytesOffset, true);
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
        let primitiveType = view.getUint8(bytesOffset, true);
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
        bytesOffset += Uint8Array.BYTES_PER_ELEMENT;

        if(indexCount > 0){
            let indexBuffer = null;
            let byteLength;
            if(indexType === 1 || indexType === 3){
                byteLength = indexCount * Uint32Array.BYTES_PER_ELEMENT;
                indexBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
            }
            else{
                byteLength = indexCount * Uint16Array.BYTES_PER_ELEMENT;
                indexBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
                if(indexCount % 2 !== 0) {
                    byteLength += 2;
                }
            }

            indexPackage.indicesTypedArray = indexBuffer;
            bytesOffset += byteLength;
        }

        indexPackage.indicesCount = indexCount;
        indexPackage.indexType = indexType;
        indexPackage.primitiveType = primitiveType;

        let arrPassName = [];
        let passNameCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        for(let j = 0; j < passNameCount; j++) {
            let res = parseString(buffer, view, bytesOffset);
            let passName = res.string;
            bytesOffset = res.bytesOffset;
            arrPassName.push(passName);
            indexPackage.materialCode = passName;
        }

        let align = bytesOffset % 4;
        if(align !== 0){
            let nReserved = 4 - bytesOffset % 4;
            bytesOffset += nReserved;
        }

        arrIndexPackage.push(indexPackage);
    }

    return bytesOffset;
}

function parseSkeleton(buffer, view, bytesOffset, geoPackage, version) {
    let size = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let count = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < count; i++){
        if(version >= 3.0){
            let streamSize =  view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        }

        let res = parseString(buffer, view, bytesOffset);
        let geometryName = res.string;
        bytesOffset = res.bytesOffset;
        let align = res.length % 4;
        if(align !== 0){
            bytesOffset += (4 - align);
        }

        let tag = view.getUint32(bytesOffset, true);
        bytesOffset += Int32Array.BYTES_PER_ELEMENT;

        if(version >= 3){
            switch (tag){
                case S3MBVertexTagV3.Standard : tag = S3MBVertexTag.SV_Standard;break;
                case S3MBVertexTagV3.Draco : tag = S3MBVertexTag.SV_DracoCompressed;break;
                case S3MBVertexTagV3.MeshOpt : tag = S3MBVertexTag.SV_Compressed;break;
                default : break;
            }
        }

        let vertexPackage = {
            vertexAttributes : [],
            attrLocation : {},
            instanceCount : 0,
            instanceMode : 0,
            instanceIndex : -1
        };

        let arrIndexPackage = [];

        if(tag === S3MBVertexTag.SV_Standard){
            bytesOffset = parseStandardSkeleton(buffer, view, bytesOffset, vertexPackage, version);
        }
        else if(tag === S3MBVertexTag.SV_Compressed && version >= 3){
            bytesOffset = parseMeshOptSkeleton(buffer, view, bytesOffset, vertexPackage, version);
        }
        else if(tag === S3MBVertexTag.SV_Compressed){
            bytesOffset = parseCompressSkeleton(buffer, view, bytesOffset, vertexPackage);
        }
        else if(tag === S3MBVertexTag.SV_DracoCompressed){
            bytesOffset = parseDracoSkeleton(buffer, view, bytesOffset, vertexPackage, version, arrIndexPackage, dracoLib);
        }

        if(tag === S3MBVertexTag.SV_Compressed && version >= 3){
            bytesOffset = parseMeshOpIndexPackage(buffer, view, bytesOffset, arrIndexPackage, version);
        }
        else if(tag === S3MBVertexTag.SV_DracoCompressed){ }
        else{
            bytesOffset = parseIndexPackage(buffer, view, bytesOffset, arrIndexPackage, version);
        }


        let edgeGeometry = undefined;
        if(arrIndexPackage.length === 2 && arrIndexPackage[1].primitiveType === 13 && arrIndexPackage[1].indicesCount >= 3){
            edgeGeometry = S3MEdgeProcessor.createEdgeDataByIndices(vertexPackage, arrIndexPackage[1]);
        }

        geoPackage[geometryName] = {
            vertexPackage: vertexPackage,
            arrIndexPackage: arrIndexPackage,
            edgeGeometry: edgeGeometry
        };

        if(version >= 3){
            const obbCenter = {};
            obbCenter.x = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            obbCenter.y = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            obbCenter.z = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            const xExtent = {};
            xExtent.x = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            xExtent.y = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            xExtent.z = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;

            const yExtent = {};
            yExtent.x = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            yExtent.y = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            yExtent.z = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;

            const zExtent = {};
            zExtent.x = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            zExtent.y = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
            zExtent.z = view.getFloat64(bytesOffset, true);
            bytesOffset += Float64Array.BYTES_PER_ELEMENT;
        }
    }

    if(version < 3){
        let secColorSize =  view.getUint32(bytesOffset, true);
        bytesOffset += secColorSize;
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    }

    return bytesOffset;
}

function parseTexturePackage(buffer, view, bytesOffset, texturePackage, version) {
    let size = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let count = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    for(let i = 0; i < count; i++){
        let res = parseString(buffer, view, bytesOffset);
        let textureCode = res.string;
        bytesOffset = res.bytesOffset;
        let align = res.length % 4;
        if(align !== 0){
            bytesOffset += (4 - align);
        }

        let level = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let width = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let height = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let compressType = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let size = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let pixelFormat = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let textureData = new Uint8Array(buffer, bytesOffset, size);
        bytesOffset += size;
        if(version === 3.01){
            compressType = fromStandardTextureCompressType(compressType);
        }
        let internalFormat = (pixelFormat === S3MPixelFormat.RGB ||  pixelFormat === S3MPixelFormat.BGR) ? 33776 :  33779;
        if(compressType === 22){
            internalFormat = 36196;//rgb_etc1
        }

        if(!S3ModelParser.s3tc && (internalFormat === 33776 || internalFormat === 33779)) {
            let out = new Uint8Array(width * height * 4);
            DXTTextureDecode.decode(out, width, height, textureData, pixelFormat);
            textureData = out;
            compressType = 0;
            internalFormat = (pixelFormat === S3MPixelFormat.RGB || pixelFormat === S3MPixelFormat.RGB) ? 273 : 4369;
        }


        texturePackage[textureCode] = {
            id: textureCode,
            width: width,
            height: height,
            compressType: compressType,
            nFormat: pixelFormat,
            internalFormat : internalFormat,
            arrayBufferView: textureData
        };
    }

    return bytesOffset;
}

function parseMaterial(buffer, view, bytesOffset, result) {
    let byteLength = view.getUint32(bytesOffset, true);
    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    let materialBuffer = new Uint8Array(buffer, bytesOffset, byteLength);
    let strMaterials = getStringFromTypedArray(materialBuffer);
    bytesOffset += byteLength;
    result.materials = JSON.parse(strMaterials);

    return bytesOffset;
}

let colorScratch = {
    red : 0,
    green : 0,
    blue : 0,
    alpha : 0
};

function unpackColor(array, startingIndex, result) {
    result.red = array[startingIndex++];
    result.green = array[startingIndex++];
    result.blue = array[startingIndex++];
    result.alpha = array[startingIndex];
    return result;
}

let LEFT_16 = 65536;
function parsePickInfo(buffer, view, bytesOffset, nOptions, geoPackage, version) {
    if(version >= 3){
        nOptions = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
    }

    if((nOptions & 1) === 1){
        let size = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let count = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        for(let i = 0; i < count; i++){
            let res = parseString(buffer, view, bytesOffset);
            let geometryName = res.string;
            bytesOffset = res.bytesOffset;
            let selectInfoCount = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let pickInfo = {};
            geoPackage[geometryName].pickInfo = pickInfo;
            let bInstanced = geoPackage[geometryName].vertexPackage.instanceIndex;
            if(bInstanced == -1){      //非实例化信息
                let batchIds = new Float32Array(geoPackage[geometryName].vertexPackage.verticesCount);
                for(let j = 0; j < selectInfoCount; j++){
                    let nDictID = view.getUint32(bytesOffset, true);
                    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    let nSize = view.getUint32(bytesOffset, true);
                    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    let infos = [];
                    for(let k = 0; k < nSize; k++){
                        let vertexColorOffset = view.getUint32(bytesOffset, true);
                        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                        let vertexCount = view.getUint32(bytesOffset, true);
                        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                        batchIds.fill(j, vertexColorOffset, vertexColorOffset + vertexCount);
                        infos.push({
                            vertexColorOffset: vertexColorOffset,
                            vertexColorCount: vertexCount,
                            batchId:j
                        });
                    }

                    pickInfo[nDictID] = infos;
                }
                createBatchIdAttribute(geoPackage[geometryName].vertexPackage,batchIds,undefined);
            }else{
                let instanceCount = geoPackage[geometryName].vertexPackage.instanceCount;
                let instanceArray = geoPackage[geometryName].vertexPackage.instanceBuffer;
                let instanceMode = geoPackage[geometryName].vertexPackage.instanceMode;
                let instanceIds = new Float32Array(instanceCount);
                let selectionId = [];
                for(let j = 0;j < selectInfoCount;j++){
                    let nDictID = view.getUint32(bytesOffset, true);
                    selectionId.push(nDictID);
                    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    let nSize = view.getUint32(bytesOffset, true);
                    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    for(let k = 0;k < nSize; k++){
                        let instanceId = view.getUint32(bytesOffset, true);
                        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                        if(version >= 3){
                            let vertexCount = view.getUint32(bytesOffset, true);
                            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                        }
                    }
                }

                let beginOffset = instanceMode === 17 ? 16 : 28;
                beginOffset *= Float32Array.BYTES_PER_ELEMENT;
                for(let j = 0;j < instanceCount; j++){
                    instanceIds[j] = j;
                    let offset = j * instanceMode * Float32Array.BYTES_PER_ELEMENT + beginOffset;
                    unpackColor(instanceArray, offset, colorScratch);
                    let pickId = version === 2 ? selectionId[j] : colorScratch.red + colorScratch.green * 256 + colorScratch.blue * LEFT_16;
                    if(pickInfo[pickId] === undefined){
                        pickInfo[pickId] = {
                            vertexColorCount:1,
                            instanceIds:[],
                            vertexColorOffset:j
                        }
                    }
                    pickInfo[pickId].instanceIds.push(j);
                }
                createBatchIdAttribute(geoPackage[geometryName].vertexPackage,instanceIds,1);
            }

        }
    }

    return bytesOffset;
}

function createBatchIdAttribute(vertexPackage, typedArray, instanceDivisor){
    let vertexAttributes = vertexPackage.vertexAttributes;
    let attrLocation = vertexPackage.attrLocation;
    let len = vertexAttributes.length;
    let attrName = instanceDivisor === 1 ? 'instanceId' : 'batchId';
    attrLocation[attrName] = len;
    vertexAttributes.push({
        index: len,
        typedArray: typedArray,
        componentsPerAttribute: 1,
        componentDatatype: 5126,
        offsetInBytes: 0,
        strideInBytes: 0,
        instanceDivisor: instanceDivisor
    });
}
const S3MBTextureCompressType = 
{
	TC_NONE : 0,
	TC_DXT1_RGB : 33776,
	TC_DXT1_RGBA : 33777,
	TC_DXT3 : 33778,
	TC_DXT5 : 33779,
	TC_WEBP : 38000,
	TC_CRN : 38001,
	TC_KTX2 : 38002,
	//自定义扩展类型
	TC_CRN_DXT5 : 50000
};


function fromStandardTextureCompressType(nType){
    let nResType = S3MPixelFormat.NONE
    switch (nType)
    {
    case S3MBTextureCompressType.TC_DXT1_RGB:
    case S3MBTextureCompressType.TC_DXT5:
        nResType = S3MPixelFormat.BGRA;
        break;
    case S3MBTextureCompressType.TC_WEBP:
        nResType = S3MPixelFormat.WEBP;
        break;
    case S3MBTextureCompressType.TC_CRN:
        nResType = S3MPixelFormat.STANDARD_CRN;
        break;
    case S3MBTextureCompressType.TC_KTX2:
        nResType = S3MPixelFormat.KTX2;
        break;
    case S3MBTextureCompressType.TC_CRN_DXT5:
        nResType = S3MPixelFormat.CRN_DXT5;
        break;
    default:
        nResType = S3MPixelFormat.NONE;
        break;
    }
    return nResType;
}

S3ModelParser.parseBuffer = function(buffer) {
        let bytesOffset = 0;
        let result = {
            version : undefined,
            groupNode : undefined,
            geoPackage : {},
            matrials : undefined,
            texturePackage : {}
        };

        let view = new DataView(buffer);
        result.version = view.getFloat32(bytesOffset, true);
        result.version  = Number(result.version.toFixed(2));
        bytesOffset += Float32Array.BYTES_PER_ELEMENT;
        if (result.version >= 2.0) {
            let unzipSize = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        }

        let compressedType = 0;
        if(result.version >= 3) {
            compressedType = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        }

        let byteSize = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let unzipBuffer = unZip(buffer, bytesOffset);
        view = new DataView(unzipBuffer);
        bytesOffset = 0;
        let nOptions = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;

        bytesOffset = parseGroupNode(unzipBuffer, view, bytesOffset, result);

        bytesOffset = parseSkeleton(unzipBuffer, view, bytesOffset, result.geoPackage, result.version);

        bytesOffset = parseTexturePackage(unzipBuffer, view, bytesOffset, result.texturePackage, result.version);

        bytesOffset = parseMaterial(unzipBuffer, view, bytesOffset, result);

        parsePickInfo(unzipBuffer, view, bytesOffset, nOptions, result.geoPackage, result.version);

        return result;
};

export default S3ModelParser;