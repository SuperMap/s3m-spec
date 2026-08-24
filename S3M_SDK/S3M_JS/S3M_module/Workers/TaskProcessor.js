const defaultValue =(value, defaultValue)=>value ?? defaultValue;
const defined = Cesium.defined;
const destroyObject = Cesium.destroyObject;
const DeveloperError = Cesium.DeveloperError;
const Event = Cesium.Event;
const RuntimeError = Cesium.RuntimeError;

const taskCompletedEvent = new Event();

/**
 * 将 worker 相对路径解析为基于本文件的绝对 URL。
 * 直接使用 import.meta.url，避免依赖 Cesium 的 buildModuleUrl。
 * @private
 * @param {string} workerPath 相对于本文件的 worker 路径，例如 "GaussianSplating" 或 "GaussianSplating.js"。
 * @returns {string} 可直接传给 Worker 构造器的 URL。
 */
function getWorkerUrl(workerPath) {
  // 兼容不带扩展名的写法，例如 'GaussianSplating'
  const path = /\.js$/.test(workerPath) ? workerPath : `${workerPath}.js`;
  const baseUrl = new URL(".", import.meta.url);
  return new URL(path, baseUrl).href;
}

function createWorker(processor) {
  // worker 均为 ES module（内部使用 import 语法），必须以 module 方式创建
  const workerUrl = getWorkerUrl(processor._workerPath);
  return new Worker(workerUrl, { type: "module" });
}

/**
 * A wrapper around a web worker that allows scheduling tasks for a given worker,
 * returning results asynchronously via a promise.
 *
 * The Worker is not constructed until a task is scheduled.
 *
 * @alias TaskProcessor
 * @constructor
 *
 * @param {string} workerPath The Url to the worker. 相对于本文件的 worker 名称，
 *                            例如 'GaussianSplating' 或 'GaussianSplatingSort'。
 * @param {number} [maximumActiveTasks=Number.POSITIVE_INFINITY] The maximum number of active tasks.  Once exceeded,
 *                                        scheduleTask will not queue any more tasks, allowing
 *                                        work to be rescheduled in future frames.
 */
function TaskProcessor(workerPath, maximumActiveTasks) {
  this._workerPath = workerPath;
  this._maximumActiveTasks = defaultValue(
    maximumActiveTasks,
    Number.POSITIVE_INFINITY
  );
  this._activeTasks = 0;
  this._nextID = 0;
}

const createOnmessageHandler = (worker, id, resolve, reject) => {
  const listener = ({ data }) => {
    if (data.id !== id) {
      return;
    }

    if (defined(data.error)) {
      let error = data.error;
      if (error.name === "RuntimeError") {
        error = new RuntimeError(data.error.message);
        error.stack = data.error.stack;
      } else if (error.name === "DeveloperError") {
        error = new DeveloperError(data.error.message);
        error.stack = data.error.stack;
      } else if (error.name === "Error") {
        error = new Error(data.error.message);
        error.stack = data.error.stack;
      }
      taskCompletedEvent.raiseEvent(error);
      reject(error);
    } else {
      taskCompletedEvent.raiseEvent();
      resolve(data.result);
    }

    worker.removeEventListener("message", listener);
  };

  return listener;
};

const emptyTransferableObjectArray = [];
async function runTask(processor, parameters, transferableObjects) {
  // 本插件要求 ES module worker（隐含 Chrome 80+ / Firefox 85+ / Safari 15+），
  // 这些浏览器对 transferable objects 支持完备，无需再做能力探测
  const canTransfer = true;
  if (!defined(transferableObjects)) {
    transferableObjects = emptyTransferableObjectArray;
  } else if (!canTransfer) {
    transferableObjects.length = 0;
  }

  const id = processor._nextID++;
  const promise = new Promise((resolve, reject) => {
    processor._worker.addEventListener(
      "message",
      createOnmessageHandler(processor._worker, id, resolve, reject)
    );
  });

  processor._worker.postMessage(
    {
      id: id,
      parameters: parameters,
      canTransferArrayBuffer: canTransfer,
    },
    transferableObjects
  );

  return promise;
}

async function scheduleTask(processor, parameters, transferableObjects) {
  ++processor._activeTasks;

  try {
    const result = await runTask(processor, parameters, transferableObjects);
    --processor._activeTasks;
    return result;
  } catch (error) {
    --processor._activeTasks;
    throw error;
  }
}

/**
 * Schedule a task to be processed by the web worker asynchronously.  If there are currently more
 * tasks active than the maximum set by the constructor, will immediately return undefined.
 * Otherwise, returns a promise that will resolve to the result posted back by the worker when
 * finished.
 *
 * @param {object} parameters Any input data that will be posted to the worker.
 * @param {Object[]} [transferableObjects] An array of objects contained in parameters that should be
 *                                      transferred to the worker instead of copied.
 * @returns {Promise<object>|undefined} Either a promise that will resolve to the result when available, or undefined
 *                    if there are too many active tasks,
 *
 * @example
 * const taskProcessor = new TaskProcessor('GaussianSplating');
 * const promise = taskProcessor.scheduleTask({
 *     someParameter : true,
 *     another : 'hello'
 * });
 * if (!Cesium.defined(promise)) {
 *   // too many active tasks - try again later
 * } else {
 *   promise.then(function(result) {
 *     // use the result of the task
 *   });
 * }
 */
TaskProcessor.prototype.scheduleTask = function (
  parameters,
  transferableObjects
) {
  if (!defined(this._worker)) {
    this._worker = createWorker(this);
  }

  if (this._activeTasks >= this._maximumActiveTasks) {
    return undefined;
  }

  return scheduleTask(this, parameters, transferableObjects);
};

/**
 * Returns true if this object was destroyed; otherwise, false.
 * <br /><br />
 * If this object was destroyed, it should not be used; calling any function other than
 * <code>isDestroyed</code> will result in a {@link DeveloperError} exception.
 *
 * @returns {boolean} True if this object was destroyed; otherwise, false.
 *
 * @see TaskProcessor#destroy
 */
TaskProcessor.prototype.isDestroyed = function () {
  return false;
};

/**
 * Destroys this object.  This will immediately terminate the Worker.
 * <br /><br />
 * Once an object is destroyed, it should not be used; calling any function other than
 * <code>isDestroyed</code> will result in a {@link DeveloperError} exception.
 */
TaskProcessor.prototype.destroy = function () {
  if (defined(this._worker)) {
    this._worker.terminate();
  }
  return destroyObject(this);
};

/**
 * An event that's raised when a task is completed successfully.  Event handlers are passed
 * the error object is a task fails.
 *
 * @type {Event}
 *
 * @private
 */
TaskProcessor.taskCompletedEvent = taskCompletedEvent;

export default TaskProcessor;
