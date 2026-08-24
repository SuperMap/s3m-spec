import createTaskProcessorWorker from "./createTaskProcessorWorker.js";
/**
 * 高斯点排序
 * @param  {Float32Array} buffer 高斯点位置
 * @param  {Matrix4} viewProj 模型视图矩阵
 * @param  {Number} vertexCount 高斯点数量
 * @returns  {Uint32Array} 排序后高斯点的索引
 */
function runSort(buffer, viewProj, vertexCount) {
  if (!buffer) return;
  const f_buffer = buffer.buffer ? new Float32Array(buffer.buffer, buffer.byteOffset, buffer.byteLength / 4) : new Float32Array(buffer);
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

function generateGaussianSortWorker(parameters, transferableObjects) {
  const { positions, modelView, count, type } = parameters;
  if(type === 'sortIndex'){
    let indexs = runSort(positions, modelView, count);
    if(transferableObjects) {
      transferableObjects.push(positions.buffer, indexs.buffer);
    }
    return { indexs, positions , count};
  }
}

export default createTaskProcessorWorker(generateGaussianSortWorker);
