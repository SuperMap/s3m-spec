import createTaskProcessorWorker from "./createTaskProcessorWorker.js";

var _floatView = new Float32Array(1);
var _int32View = new Int32Array(_floatView.buffer);

function floatToHalf(float) {
  _floatView[0] = float;
  var f = _int32View[0];

  var sign = (f >> 31) & 0x0001;
  var exp = (f >> 23) & 0x00ff;
  var frac = f & 0x007fffff;

  var newExp;
  if (exp == 0) {
    newExp = 0;
  } else if (exp < 113) {
    newExp = 0;
    frac |= 0x00800000;
    frac = frac >> (113 - exp);
    if (frac & 0x01000000) {
      newExp = 1;
      frac = 0;
    }
  } else if (exp < 142) {
    newExp = exp - 112;
  } else {
    newExp = 31;
    frac = 0;
  }

  return (sign << 15) | (newExp << 10) | (frac >> 13);
}

function createSplateTextureData(splatCount, attributes) {
  let { positions, scales, rotations, colors, batchIds } = attributes;

  var texdata = new Uint32Array(splatCount * 8); // 4 components per pixel (RGBA)
  var texdata_c = new Uint8Array(texdata.buffer);
  var texdata_f = new Float32Array(texdata.buffer);

  // Here we convert from a .splat file buffer into a texture
  // With a little bit more foresight perhaps this texture file
  // should have been the native format as it'd be very easy to
  // load it into webgl.
  for (let i = 0; i < splatCount; i++) {
    // x, y, z
    texdata_f[8 * i + 0] = positions[i * 3 + 0];
    texdata_f[8 * i + 1] = positions[i * 3 + 1];
    texdata_f[8 * i + 2] = positions[i * 3 + 2];
    texdata_f[8 * i + 3] = batchIds[i];

    // r, g, b, a
    texdata_c[4 * (8 * i + 7) + 0] = colors[i * 4 + 0];
    texdata_c[4 * (8 * i + 7) + 1] = colors[i * 4 + 1];
    texdata_c[4 * (8 * i + 7) + 2] = colors[i * 4 + 2];
    texdata_c[4 * (8 * i + 7) + 3] = colors[i * 4 + 3];

    // quaternions
    let scale = [
      scales[i * 3 + 0],
      scales[i * 3 + 1],
      scales[i * 3 + 2],
    ];
    let rot = [
      rotations[i * 4 + 0],
      rotations[i * 4 + 1],
      rotations[i * 4 + 2],
      rotations[i * 4 + 3],
    ];

    // Compute the matrix product of S and R (M = S * R)
    const M = [
      1.0 - 2.0 * (rot[2] * rot[2] + rot[3] * rot[3]),
      2.0 * (rot[1] * rot[2] + rot[0] * rot[3]),
      2.0 * (rot[1] * rot[3] - rot[0] * rot[2]),

      2.0 * (rot[1] * rot[2] - rot[0] * rot[3]),
      1.0 - 2.0 * (rot[1] * rot[1] + rot[3] * rot[3]),
      2.0 * (rot[2] * rot[3] + rot[0] * rot[1]),

      2.0 * (rot[1] * rot[3] + rot[0] * rot[2]),
      2.0 * (rot[2] * rot[3] - rot[0] * rot[1]),
      1.0 - 2.0 * (rot[1] * rot[1] + rot[2] * rot[2]),
    ].map((k, i) => k * scale[Math.floor(i / 3)]);

    const sigma = [
      M[0] * M[0] + M[3] * M[3] + M[6] * M[6],
      M[0] * M[1] + M[3] * M[4] + M[6] * M[7],
      M[0] * M[2] + M[3] * M[5] + M[6] * M[8],
      M[1] * M[1] + M[4] * M[4] + M[7] * M[7],
      M[1] * M[2] + M[4] * M[5] + M[7] * M[8],
      M[2] * M[2] + M[5] * M[5] + M[8] * M[8],
    ];

    texdata[8 * i + 4] = packHalf2x16(4 * sigma[0], 4 * sigma[1]);
    texdata[8 * i + 5] = packHalf2x16(4 * sigma[2], 4 * sigma[3]);
    texdata[8 * i + 6] = packHalf2x16(4 * sigma[4], 4 * sigma[5]);
  }

  return texdata;
}

function packHalf2x16(x, y) {
  return (floatToHalf(x) | (floatToHalf(y) << 16)) >>> 0;
}

/**
 *  解析顶点位置
 * @param {Uint8Array} vertexPos 顶点位置
 * @param {Boolean} isCompress  是否压缩 
 * @param {Quaternion} minVerticesValue  压缩参数
 * @param {Number} vertCompressConstant  压缩参数
 * @param {Number} splatCount  顶点数
 * @param {Number} dimension  顶点维度
 * @returns {Float32Array} 
 */
function parseVertexPosition(vertexPos, isCompress, minVerticesValue, vertCompressConstant, splatCount, dimension = 4) {
  let buffer = new Float32Array(splatCount * 3);
  if (isCompress) {
    let bufferPos = new Int16Array(vertexPos.buffer, vertexPos.byteOffset, vertexPos.byteLength / 2);
    for (let i = 0, j = bufferPos.length / dimension; i < j; i++) {
      buffer[i * 3] = bufferPos[i * dimension] * vertCompressConstant + minVerticesValue.x;
      buffer[i * 3 + 1] = bufferPos[i * dimension + 1] * vertCompressConstant + minVerticesValue.y;
      buffer[i * 3 + 2] = bufferPos[i * dimension + 2] * vertCompressConstant + minVerticesValue.z;
    }
  } else {
    let bufferPos = new Float32Array(vertexPos.buffer, vertexPos.byteOffset, vertexPos.byteLength / 4);
    if (dimension === 3) {
      return bufferPos;
    }
    for (let i = 0, j = bufferPos.length / 4; i < j; i++) {
      buffer[i * 3] = bufferPos[i * 4];
      buffer[i * 3 + 1] = bufferPos[i * 4 + 1];
      buffer[i * 3 + 2] = bufferPos[i * 4 + 2];
    }
  }
  return buffer;
}

/**
 *  解析顶点颜色
 * @param {Uint8Array} colArray 顶点颜色buffer
 * @returns {Uint8Array} 
 */
function parseVertexColor(colArray) {
  return new Uint8Array(colArray.buffer, colArray.byteOffset, colArray.byteLength);
}

/**
 *  解码SPZ压缩的顶点缩放
 * @param {Uint8Array} 顶点缩放buffer 
 * @returns {Float32Array} 
 */
function decodeScaleVertices(arrayBuffer) {
  let newArrayBuffer = new Float32Array(arrayBuffer.length / 4 * 3);
  for (let i = 0, j = arrayBuffer.length / 4; i < j; i++) {
    newArrayBuffer[i * 3] = Math.exp(arrayBuffer[i * 4] / 16.0 - 10.0);
    newArrayBuffer[i * 3 + 1] = Math.exp(arrayBuffer[i * 4 + 1] / 16.0 - 10.0);
    newArrayBuffer[i * 3 + 2] = Math.exp(arrayBuffer[i * 4 + 2] / 16.0 - 10.0);
  }
  return newArrayBuffer;
}

/**
 *  解析顶点缩放
 * @param {Uint8Array} arrayBuffer 
 * @param {Boolean} isSPZCompress  是否是spz压缩 
 * @returns {Float32Array} arrayBuffer
 */
function parseVertexScale(arrayBuffer, isSPZCompress) {
  if (!isSPZCompress) {
    return new Float32Array(arrayBuffer.buffer, arrayBuffer.byteOffset, arrayBuffer.byteLength / 4);
  }
  return decodeScaleVertices(arrayBuffer);
}

/**
 * 解码SPZ压缩的顶点旋转
 * @param {Uint8Array} arrayBuffer 
 * @returns {Float32Array} 
 */
function decodeRotationSpz(arrayBuffer) {
  let newArrayBuffer = new Float32Array(arrayBuffer.length);
  for (let i = 0, j = arrayBuffer.length; i < j; i += 4) {
    let x = arrayBuffer[i] / 127.5 - 1.0;
    let y = arrayBuffer[i + 1] / 127.5 - 1.0;
    let z = arrayBuffer[i + 2] / 127.5 - 1.0;
    let w = Math.sqrt(Math.max(0.0, 1.0 - (x * x + y * y + z * z)));
    newArrayBuffer[i] = w;
    newArrayBuffer[i + 1] = x;
    newArrayBuffer[i + 2] = y;
    newArrayBuffer[i + 3] = z;
  }
  return newArrayBuffer;
}

/**
* 解码SPZ2压缩的四元数旋转数据 
* @param {Uint8Array} pDecodeVertices - 压缩后的顶点数据（每个顶点4字节）
* @param {number} nVertexCount - 顶点数量
* @param {Float32Array} pVertexAttData - 解量化后存储的数据
*/
function decodeRotationSpz2(pDecodeVertices, nVertexCount) {
  const pVertexAttData = new Float32Array(nVertexCount * 4);
  const c_mask = (1 << 9) - 1; // 9位掩码，值为511
  const sqrt1_2 = Math.sqrt(0.5); // 预计算1/√2

  for (let k = 0; k < nVertexCount; k++) {
    // 从4个字节组装32位无符号整数（小端字节序）
    const byteOffset = 4 * k;
    const comp =
      pDecodeVertices[byteOffset] |
      (pDecodeVertices[byteOffset + 1] << 8) |
      (pDecodeVertices[byteOffset + 2] << 16) |
      (pDecodeVertices[byteOffset + 3] << 24);

    // 获取最大分量的索引（2位）
    const i_largest = (comp >> 30) & 0x3; // 确保只取低2位
    let sum_squares = 0;
    let tempComp = comp; // 使用临时变量避免修改原始值

    // 逆序处理3个非最大分量
    for (let i = 3; i >= 0; i--) {
      if (i !== i_largest) {
        // 提取9位幅度值
        const mag = tempComp & c_mask;
        // 提取符号位
        const negbit = (tempComp >> 9) & 0x1;
        // 右移10位（9位幅度+1位符号）
        tempComp = tempComp >> 10;

        // 计算归一化后的分量值
        let value = sqrt1_2 * mag / c_mask;
        if (negbit) {
          value = -value;
        }

        pVertexAttData[4 * k + i] = value;
        sum_squares += value * value;
      }
    }
    // 计算最大分量（保证四元数单位长度）
    pVertexAttData[4 * k + i_largest] = Math.sqrt(Math.max(1.0 - sum_squares, 0));
  }
  return pVertexAttData;
}

// 解析顶点旋转
function parseVertexRotation(arrayBuffer, isSPZCompress, spzVersion) {
  if (!isSPZCompress) {
    return new Float32Array(arrayBuffer.buffer, arrayBuffer.byteOffset, arrayBuffer.byteLength / 4);
  }
  if (spzVersion == 1.1) {
    return decodeRotationSpz2(arrayBuffer, arrayBuffer.length / 4);
  }
  return decodeRotationSpz(arrayBuffer);
}

// 准备创建高斯纹理的数据：包括顶点位置，颜色，旋转和缩放合并到一张纹理
function prepareTextureData(options, transferableObjects) {
  let isSPZCompress = options.isSPZCompress;
  let { posArray, colArray, scaArray, rotArray, isCompress, splatCount, positionDimension, minVerticesValue, vertCompressConstant, batchIdArray, version } = options;
  let bufferPos = parseVertexPosition(posArray, isCompress, minVerticesValue, vertCompressConstant, splatCount, positionDimension);
  let bufferColor = parseVertexColor(colArray);
  let bufferScale = parseVertexScale(scaArray, isSPZCompress);
  let bufferRot = parseVertexRotation(rotArray, isSPZCompress, version);
  batchIdArray = batchIdArray ??new Float32Array(splatCount);

  const attributes = {
    positions: bufferPos,
    scales: bufferScale,
    rotations: bufferRot,
    colors: bufferColor,
    batchIds: batchIdArray
  };

  let texData = createSplateTextureData(splatCount, attributes);
  if (transferableObjects) {
    transferableObjects.push(bufferPos.buffer, texData.buffer);
  }

  return { bufferPos, texData };
}

function generateGaussianTextureWorker(parameters, transferableObjects) {
  if (parameters.type === 'textureData') {
    return prepareTextureData(parameters, transferableObjects)
  }
}

export default createTaskProcessorWorker(generateGaussianTextureWorker);
