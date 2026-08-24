import TaskProcessor from '../../Workers/TaskProcessor.js';
const Cartesian3 = Cesium.Cartesian3;
const defined = Cesium.defined;
const destroyObject = Cesium.destroyObject;
const mergeSort = Cesium.mergeSort;

const GAUSSIAN_SPLAT_WORKER = 'GaussianSplating';
const GAUSSIAN_SPLAT_SORT_WORKER = 'GaussianSplatingSort';
// 限制高斯属性纹理初始化任务并发，避免大量瓦片同时加载时造成内存峰值过高。
const MAX_ACTIVE_PROCESS_TASKS = 16;
const MAX_ACTIVE_SORT_TASKS = 16;

function compareSplatsBackToFront(a, b) {
  return b.gaussianSplatMesh._distanceSquaredToCenter - a.gaussianSplatMesh._distanceSquaredToCenter;
}

class GaussianSplatController {
  constructor() {
    this._gaussianSplatProcessor = new TaskProcessor(GAUSSIAN_SPLAT_WORKER, MAX_ACTIVE_PROCESS_TASKS);
    this._gaussianSplatSortProcessor = new TaskProcessor(GAUSSIAN_SPLAT_SORT_WORKER, MAX_ACTIVE_SORT_TASKS);
    
    // this._gaussianSplatProcessor = new Worker(GAUSSIAN_SPLAT_WORKER);
    // this._gaussianSplatSortProcessor = new Worker(GAUSSIAN_SPLAT_SORT_WORKER);
    
    this._currentLoadedSplatCount = 0;
    this._totalGaussianSplatCount = 0;
  }

  /**
   * 计算高斯渲染对象到相机的距离。
   * @private
   * @param {FrameState} frameState 帧状态。
   * @param {RenderEntityPagedLOD[]} renderQueue 当前需要渲染的高斯 RenderObject 队列。
   */
  _computeDepth(frameState, renderQueue) {
    const cameraPosition = frameState.camera.positionWC;
    let loadedSplatCount = 0;

    for (let i = 0, length = renderQueue.length; i < length; i++) {
      const renderObject = renderQueue[i];
      const mesh = renderObject.gaussianSplatMesh;
      mesh._distanceSquaredToCenter = Cartesian3.distanceSquared(renderObject.boundingVolume.center, cameraPosition);
      loadedSplatCount += mesh._splatCount;
    }

    this._currentLoadedSplatCount = loadedSplatCount;
  }

  /**
   * 按距离由近到远更新tile内部的高斯点排序器。
   * 合并 RO 的排序器由合并类内部驱动（updateSorter），此处跳过避免重复提交排序。
   * @private
   * @param {FrameState} frameState 帧状态。
   * @param {RenderEntityPagedLOD[]} renderQueue 当前需要渲染的高斯 RenderObject 队列（按远到近排序后）。
   */
  _sort(frameState, renderQueue) {
    for (let i = renderQueue.length - 1; i >= 0; i--) {
      const renderObject = renderQueue[i];
      if (renderObject._isGSMergedRo) {
        continue;
      }
      const mesh = renderObject.gaussianSplatMesh;
      mesh.updateSorter(frameState);
    }
  }

  /**
   * 提交高斯渲染对象的绘制命令。
   * @private
   * @param {FrameState} frameState 帧状态。
   * @param {RenderEntityPagedLOD[]} renderQueue 当前需要渲染的高斯 RenderObject 队列。
   */
  _render(frameState, renderQueue) {
    for (let i = 0, length = renderQueue.length; i < length; i++) {
      const renderObject = renderQueue[i];
      if (!renderObject.isDestroyed()) {
        // 与 S3MGaussianRenderEntity.update(frameState, layer) 签名对齐
        renderObject.update(frameState, renderObject.layer);
      }
    }
  }

  /**
   * 更新高斯渲染队列。
   * @private
   * @param {FrameState} frameState 帧状态。
   * @param {RenderEntityPagedLOD[]} renderQueue 当前需要渲染的 RenderObject 队列。
   */
  update(frameState, renderQueue) {
    if (renderQueue.length === 0) {
      return;
    }

    try {
      this._computeDepth(frameState, renderQueue);
      mergeSort(renderQueue, compareSplatsBackToFront);
      this._sort(frameState, renderQueue);
      this._render(frameState, renderQueue);
    } finally {
      renderQueue.length = 0;
    }
  }


  destroy() {
    this._gaussianSplatProcessor.destroy();
    this._gaussianSplatSortProcessor.destroy();
    return destroyObject(this);
  }
}

export default GaussianSplatController;
