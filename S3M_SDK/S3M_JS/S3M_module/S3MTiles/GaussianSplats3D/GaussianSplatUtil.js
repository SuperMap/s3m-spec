
const Matrix4 = Cesium.Matrix4;
const defined = Cesium.defined;

export function computeTotalDrawCount(totalTextureSize){
    const e = Math.log2(totalTextureSize);
    return ((e + 1) * e) / 2;
}

export function computeDimension(texelsPerElement, splatCount, result){
    const totalTexelSize = texelsPerElement * splatCount;
    result.x = 2;
    result.y = 2;
    for(; result.x * result.y < totalTexelSize; ){
      if(result.y > result.x){
        result.x *= 2;
      }
      else {
        result.y *= 2;
      }
    }
    return result
}

/**
 * @private
 * 高斯点排序
 * @param  {Float32Array} buffer 高斯点位置
 * @param  {Matrix4} viewProj 模型视图矩阵
 * @param  {Number} vertexCount 高斯点数量
 * @returns  {Uint32Array} 排序后高斯点的索引
 */
export function runSort(buffer, viewProj, vertexCount) {
  if (!buffer) return;
  const f_buffer = defined(buffer.buffer) ? new Float32Array(buffer.buffer, buffer.byteOffset, buffer.byteLength / 4) : new Float32Array(buffer);
  let maxDepth = -Infinity;
  let minDepth = Infinity;
  let sizeList = new Int32Array(vertexCount);
  for (let i = 0; i < vertexCount; i++) {
    let depth =
      ((viewProj[2] * f_buffer[3 * i + 0] +
        viewProj[6] * f_buffer[3 * i + 1] +
        viewProj[10] * f_buffer[3 * i + 2]) *
        4096) |
      0;
    sizeList[i] = depth;
    if (depth > maxDepth) {
      maxDepth = depth
    };
    if (depth < minDepth) {
      minDepth = depth
    };
  }

  // This is a 16 bit single-pass counting sort
  let depthInv = (256 * 256 - 1) / (maxDepth - minDepth);
  let counts0 = new Uint32Array(256 * 256);
  for (let i = 0; i < vertexCount; i++) {
    sizeList[i] = ((sizeList[i] - minDepth) * depthInv) | 0;
    counts0[sizeList[i]]++;
  }
  let starts0 = new Uint32Array(256 * 256);
  for (let i = 1; i < 256 * 256; i++) {
    starts0[i] = starts0[i - 1] + counts0[i - 1];
  }
  let depthIndex = new Uint32Array(vertexCount);
  for (let i = 0; i < vertexCount; i++) {
    depthIndex[starts0[sizeList[i]]++] = i;
  }
  return depthIndex;
}

function GaussianSplatUtil(){
}

export default GaussianSplatUtil;