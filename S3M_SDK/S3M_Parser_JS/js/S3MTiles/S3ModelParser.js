define([
    './Enum/S3MPixelFormat',
    './Enum/S3MCompressType',
    '../pako_inflate'
    ], function(
    S3MPixelFormat,
    S3MCompressType,
    pako
        ) {
    'use strict';

    function S3ModelParser() {

    }

    function unZip(buffer) {
        let dataZip = new Uint8Array(buffer, 4);
        return pako.inflate(dataZip).buffer;
    }

    function parseDoc(buffer, view, bytesOffset) {
        let xmlSize = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let xmlBuffer = new Uint8Array(buffer, bytesOffset, xmlSize);
        let nAlign = xmlSize % 4;
        if(nAlign){
            nAlign = 4 - nAlign;
        }

        bytesOffset += xmlSize + nAlign;

        let xmlDoc = Cesium.getStringFromTypedArray(xmlBuffer);

        return {
            xmlDoc : xmlDoc,
            bytesOffset : bytesOffset
        };
    }

    function parseVertex(arrayBuffer, view, bytesOffset, vertexPackage) {
        let nVertexOptions = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let vertexDimension = view.getUint16(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let normalDimension = vertexDimension;
        if (vertexDimension > 4) {
            normalDimension = vertexDimension >> 8;
            vertexDimension = vertexDimension & 0x0f;
        }

        let verticesCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let vertexStride = view.getUint16(bytesOffset, true);
        vertexStride = vertexDimension * Float32Array.BYTES_PER_ELEMENT;
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let byteLength = verticesCount * vertexStride;
        vertexPackage.attrLocation['aPosition'] = vertexPackage.index++;
        let attr = {
            index: vertexPackage.attrLocation['aPosition'],
            typedArray: new Uint8Array(arrayBuffer, bytesOffset, byteLength),
            componentsPerAttribute: vertexDimension,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            offsetInBytes: 0,
            strideInBytes: vertexStride,
            normalize: false
        };

        bytesOffset += byteLength;
        vertexPackage.vertexAttributes.push(attr);
        vertexPackage.verticesCount = verticesCount;
        vertexPackage.vertexDemension = vertexDimension;
        vertexPackage.normalDimension = normalDimension;
        return bytesOffset;
    }

    function parseNormal(arrayBuffer, view, bytesOffset, vertexPackage) {
        let normalDimension = vertexPackage.normalDimension;
        let normalCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let normalStride = view.getUint16(bytesOffset, true);
        normalStride = normalDimension * Float32Array.BYTES_PER_ELEMENT;
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        if(normalCount > 0){
            let byteLength = normalCount * normalStride;
            vertexPackage.attrLocation['aNormal'] = vertexPackage.index++;
            let attr = {
                index: vertexPackage.attrLocation['aNormal'],
                typedArray: new Uint8Array(arrayBuffer, bytesOffset, byteLength),
                componentsPerAttribute: normalDimension,
                componentDatatype: Cesium.ComponentDatatype.FLOAT,
                offsetInBytes: 0,
                strideInBytes: normalStride,
                normalize: false
            };

            bytesOffset += byteLength;
            vertexPackage.vertexAttributes.push(attr);
        }

        return bytesOffset;
    }

    function parseVertexColor(arrayBuffer, view, bytesOffset, vertexPackage) {
        let verticesCount = vertexPackage.verticesCount;
        let vertexColorCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let vertexColor = new Uint8Array(4 * verticesCount);
        if (vertexColorCount > 0) {
            let vertexColorStride = view.getUint32(bytesOffset, true);
            vertexColorStride = 4 * Float32Array.BYTES_PER_ELEMENT;
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let byteLength = vertexColorCount * vertexColorStride;
            let oriVertexColor = new Float32Array(arrayBuffer, bytesOffset, verticesCount * 4);
            for (let m = 0; m < verticesCount; m++) {
                vertexColor[4 * m] = oriVertexColor[4 * m] * 255;
                vertexColor[4 * m + 1] = oriVertexColor[4 * m + 1] * 255;
                vertexColor[4 * m + 2] = oriVertexColor[4 * m + 2] * 255;
                vertexColor[4 * m + 3] = oriVertexColor[4 * m + 3] * 255;
            }
            bytesOffset += byteLength;
        }
        else {
            for (let m = 0; m < verticesCount; m++) {
                vertexColor[4 * m] = 255;
                vertexColor[4 * m + 1] = 255;
                vertexColor[4 * m + 2] = 255;
                vertexColor[4 * m + 3] = 255;
            }
        }

        if(verticesCount > 0){
            vertexPackage.attrLocation['aColor'] = vertexPackage.index++;
            let attr = {
                index: vertexPackage.attrLocation['aColor'],
                typedArray: vertexColor,
                componentsPerAttribute: 4,
                componentDatatype: Cesium.ComponentDatatype.UNSIGNED_BYTE,
                offsetInBytes: 0,
                strideInBytes: 4,
                normalize: true
            };
            vertexPackage.vertexColor = vertexColor;
            vertexPackage.vertexAttributes.push(attr);
        }

        return bytesOffset;
    }

    function parseSecondColor(arrayBuffer, view, bytesOffset, vertexPackage) {
        let secondColorCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        if (secondColorCount > 0) {
            let byteLength = secondColorCount * 16;
            let oriSecColor = new Float32Array(arrayBuffer, bytesOffset, secondColorCount * 4);
            let secColor = new Uint8Array(4 * secondColorCount);
            for (let m = 0; m < secondColorCount; m++) {
                secColor[4 * m] = oriSecColor[4 * m] * 255;
                secColor[4 * m + 1] = oriSecColor[4 * m + 1] * 255;
                secColor[4 * m + 2] = oriSecColor[4 * m + 2] * 255;
                secColor[4 * m + 3] = oriSecColor[4 * m + 3] * 255;
            }

            vertexPackage.attrLocation['aSecondColor'] = vertexPackage.index++;
            let attr = {
                index: vertexPackage.attrLocation['aSecondColor'],
                typedArray: secColor,
                componentsPerAttribute: 4,
                componentDatatype: Cesium.ComponentDatatype.UNSIGNED_BYTE,
                offsetInBytes: 0,
                strideInBytes: 4,
                normalize: true
            };
            bytesOffset += byteLength;
            vertexPackage.vertexAttributes.push(attr);
        }

        return bytesOffset;
    }

    function parseInstanceInfoBim(vertexPackage) {
        let byteStride = Float32Array.BYTES_PER_ELEMENT * 20;
        vertexPackage.attrLocation['uv2'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv2'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 0,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv3'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv3'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 4 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv4'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv4'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 8 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['secondary_colour'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['secondary_colour'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 12 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv6'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv6'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 16 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });
    }

    function parseInstanceInfoPipeline(vertexPackage) {
        let byteStride = Float32Array.BYTES_PER_ELEMENT * 35;
        vertexPackage.attrLocation['uv1'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv1'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 0,
            strideInBytes: byteStride,
            instanceDivisor: 1,
            byteLength: byteLength
        });

        vertexPackage.attrLocation['uv2'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv2'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 4 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv3'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv3'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 8 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv4'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv4'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 12 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv5'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv5'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 16 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv6'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv6'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 20 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv7'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv7'],
            componentsPerAttribute: 3,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 24 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['secondary_colour'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['secondary_colour'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 27 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });

        vertexPackage.attrLocation['uv9'] = vertexPackage.index++;
        vertexPackage.vertexAttributes.push({
            index: vertexPackage.attrLocation['uv9'],
            componentsPerAttribute: 4,
            componentDatatype: Cesium.ComponentDatatype.FLOAT,
            normalize: false,
            offsetInBytes: 31 * Float32Array.BYTES_PER_ELEMENT,
            strideInBytes: byteStride,
            instanceDivisor: 1
        });
    }

    function parseUV(arrayBuffer, view, bytesOffset, vertexPackage, index) {
        let count = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let instanceIndex = -1;
        for (let j = 0; j < count; j++) {
            let texCoordCount = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let texCoordDim = view.getUint16(bytesOffset, true);
            bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
            let texCoordStride = view.getUint16(bytesOffset, true);
            bytesOffset += Uint16Array.BYTES_PER_ELEMENT;
            let byteLength = texCoordCount * texCoordDim * Float32Array.BYTES_PER_ELEMENT;
            let texBuffer = new Uint8Array(arrayBuffer, bytesOffset, byteLength);
            if (instanceIndex == -1 && (texCoordDim == 20 || texCoordDim == 35)) {
                instanceIndex = j;
                vertexPackage.instanceCount = texCoordCount;
                vertexPackage.instanceMode = texCoordDim;
                vertexPackage.instanceBuffer = texBuffer;
                let len = texCoordDim * texCoordCount;
                let oriInstanceVertexColor = new Float32Array(typedArray.buffer, bytesOffset, len);
                let vertexColorInstance = oriInstanceVertexColor.slice(0, len);
                vertexPackage.vertexColorInstance = vertexColorInstance;
                if (texCoordDim === 20) {
                    parseInstanceInfoBim(vertexPackage);
                }
                else if (texCoordDim === 35) {
                    parseInstanceInfoPipeline(vertexPackage);
                }
            }
            else {
                if (instanceIndex !== -1) {
                    vertexPackage.instanceBounds = new Float32Array(arrayBuffer, bytesOffset, texCoordCount * texCoordDim);
                }
                else {
                    let str = 'aTexCoord' + j;
                    vertexPackage.attrLocation[str] = vertexPackage.index++;
                    vertexPackage.vertexAttributes.push({
                        index: vertexPackage.attrLocation[str],
                        typedArray: texBuffer,
                        componentsPerAttribute: texCoordDim,
                        componentDatatype: Cesium.ComponentDatatype.FLOAT,
                        offsetInBytes: 0,
                        strideInBytes: texCoordDim * Float32Array.BYTES_PER_ELEMENT,
                        normalize: false
                    });
                }
            }

            bytesOffset += byteLength;
        }

        vertexPackage.instanceIndex = instanceIndex;

        return bytesOffset;
    }

    function parseIndices(arrayBuffer, view, bytesOffset, arrIndexPackage) {
        let indexPackageSize = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;

        for (let j = 0; j < indexPackageSize; j++) {
            let indexPackage = {};
            let indicesCount = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let indexType = view.getUint8(bytesOffset, true);
            bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
            let useIndex = view.getUint8(bytesOffset, true);
            bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
            let primitiveType = view.getUint8(bytesOffset, true);
            bytesOffset += Uint8Array.BYTES_PER_ELEMENT;
            bytesOffset += 1;
            indexPackage.indicesCount = indicesCount;
            indexPackage.indexType = indexType;
            indexPackage.primitiveType = primitiveType;
            let indexOffset = bytesOffset;
            let byteLength = 0;
            if (indicesCount > 0) {
                if (indexType == 0) {
                    byteLength = indicesCount * Uint16Array.BYTES_PER_ELEMENT;
                    bytesOffset += byteLength;
                    if (indicesCount % 2 == 1) {
                        bytesOffset += 2;
                    }
                }
                else {
                    byteLength = indicesCount * 4;
                    bytesOffset += byteLength;
                }
            }

            indexPackage.indicesTypedArray = new Uint8Array(arrayBuffer, indexOffset, byteLength);
            let passNameCount = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let materialCode = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT * passNameCount;
            indexPackage.materialCode = materialCode;
            arrIndexPackage.push(indexPackage);
        }

        return bytesOffset;
    }

    function parseGeometry(arrayBuffer, view, bytesOffset, geoPackage) {
        let geoName = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let vertexPackage = {
            vertexAttributes : [],
            attrLocation : {},
            instanceCount : 0,
            instanceMode : 0,
            index : 0
        };

        let arrIndexPackage = [];

        bytesOffset = parseVertex(arrayBuffer, view, bytesOffset, vertexPackage);

        bytesOffset = parseNormal(arrayBuffer, view, bytesOffset, vertexPackage);

        bytesOffset = parseVertexColor(arrayBuffer, view, bytesOffset, vertexPackage);

        bytesOffset = parseSecondColor(arrayBuffer, view, bytesOffset, vertexPackage);

        bytesOffset = parseUV(arrayBuffer, view, bytesOffset, vertexPackage);

        bytesOffset = parseIndices(arrayBuffer, view, bytesOffset, arrIndexPackage);

        geoPackage[geoName] = {
            vertexPackage : vertexPackage,
            arrIndexPackage : arrIndexPackage
        };

        return bytesOffset;
    }

    function parseGeoPackage(arrayBuffer, view, bytesOffset, geoPackage) {
        let packageSize = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let geoCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        for (let i = 0; i < geoCount; i++) {
            bytesOffset = parseGeometry(arrayBuffer, view, bytesOffset, geoPackage);
        }

        return bytesOffset;
    }

    function parsePickInfo(arrayBuffer, view, bytesOffset, geoPackage) {
        let pickInfoPackageCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let pickColorCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        for (let i = 0; i < pickColorCount; i++) {
            let geoName = view.getUint32(bytesOffset, true);
            let vertexPackage = geoPackage[geoName].vertexPackage;
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let pickIdsCount = view.getUint32(bytesOffset, true);
            bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
            let pickInfo = {};
            let bInstanced = vertexPackage.instanceIndex;
            for (let j = 0; j < pickIdsCount; j++) {
                let pickId = view.getUint32(bytesOffset, true);
                bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                let size = view.getUint32(bytesOffset, true);
                bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                let infos = [];
                for (let k = 0; k < size; k++) {
                    let vertexColorOffset = view.getUint32(bytesOffset, true);
                    bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    let vertexCount = 1;
                    if(bInstanced === -1) {
                        vertexCount = view.getUint32(bytesOffset, true);
                        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
                    }

                    infos.push({
                        vertexColorOffset: vertexColorOffset,
                        vertexColorCount: vertexCount
                    });
                }

                pickInfo[pickId] = infos;
            }

            vertexPackage.pickInfo = pickInfo;
        }

        return bytesOffset;
    }

    function parseTexture(arrayBuffer, view, bytesOffset, texturePackage) {
        let texturePackageCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        let imageCount = view.getUint32(bytesOffset, true);
        bytesOffset += Uint32Array.BYTES_PER_ELEMENT;
        for (let i = 0; i < imageCount; i++) {
            let id = view.getUint32(bytesOffset, true);
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
            let imageTypedArray = new Uint8Array(arrayBuffer, bytesOffset, size);
            let internalFormat = compressType === S3MCompressType.enrS3TCDXTN && pixelFormat === S3MPixelFormat.ABGR ? Cesium.PixelFormat.RGBA_DXT5 : Cesium.PixelFormat.RGBA_DXT1;

            texturePackage[id] = {
                id: id,
                width: width,
                height: height,
                compressType: compressType,
                nFormat: pixelFormat,
                internalFormat : internalFormat,
                imageBuffer: imageTypedArray
            };

            bytesOffset += size;
        }
    }

    S3ModelParser.parseBuffer = function(buffer) {
        let bytesOffset = 0;
        let header = new Uint8Array(buffer, 0, 4);
        if (header[0] !== 115 || header[1] !== 51 || header[2] !== 109) {
            return ;
        }

        let version = header[3];
        let arrayBuffer = unZip(buffer);
        let view = new DataView(arrayBuffer);
        let result = parseDoc(arrayBuffer, view, bytesOffset);
        let xmlDoc = result.xmlDoc;
        bytesOffset = result.bytesOffset;
        let geoPackage = {};
        let texturePackage = {};
        bytesOffset = parseGeoPackage(arrayBuffer, view, bytesOffset, geoPackage);
        bytesOffset = parsePickInfo(arrayBuffer, view, bytesOffset, geoPackage);
        parseTexture(arrayBuffer, view, bytesOffset, texturePackage);

        return {
            version: version,
            xmlDoc: xmlDoc,
            geoPackage: geoPackage,
            texturePackage: texturePackage
        };
    };

    return S3ModelParser;
});
