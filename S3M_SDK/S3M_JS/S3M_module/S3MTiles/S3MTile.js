import ContentState from './Enum/ContentState.js';
import S3ModelParser from '../S3MParser/S3ModelParser.js';
import S3MContentParser from './S3MContentParser.js';
import defer from './defer.js';
import S3MPageLod from "./S3MPageLod.js";

const defined = Cesium.defined; 

function S3MTile(layer, parent, boundingVolume, fileName) {
    this.layer = layer;
    this.parent = parent;
    let path = fileName.replace(/\\/g,'/');
    this.fileExtension = Cesium.getExtensionFromUri(fileName);
    this.relativePath = getUrl(path, layer);
    this.fileName = fileName;
    this.isLeafTile = false;
    this.isRootTile = false;
    this.boundingVolume = this.createBoundingVolume(boundingVolume, layer.modelMatrix);
    let baseResource = Cesium.Resource.createIfNeeded(layer._baseResource);
    if(Cesium.defined(parent)){
        this.baseUri = parent.baseUri;
    }
    else{
        let resource = new Cesium.Resource(path);
        this.baseUri = resource.getBaseUri();
    }

    this.contentResource = baseResource.getDerivedResource({
        url : this.relativePath
    });

    this.serverKey = Cesium.RequestScheduler.getServerKey(this.contentResource.getUrlComponent());
    this.request = undefined;
    this.cacheNode = undefined;
    this.distanceToCamera = 0;
    this.centerZDepth = 0;
    this.depth = parent ? parent.depth + 1 : 0;
    this.visibilityPlaneMask = 0;
    this.visible = false;
    this.pageLods = [];
    this.renderEntities = [];
    this.contentState = ContentState.UNLOADED;
    this.touchedFrame = 0;
    this.requestedFrame = 0;
    this.processFrame = 0;
    this.updatedVisibilityFrame = 0;
    this.foveatedFactor = 0;
    this.priority = 0;
}

let scratchScale = new Cesium.Cartesian3();

function createSphere(sphere, transform) {
    let center = Cesium.Cartesian3.clone(sphere.center);
    let radius = sphere.radius;
    center = Cesium.Matrix4.multiplyByPoint(transform, center, center);
    let scale = Cesium.Matrix4.getScale(transform, scratchScale);
    let maxScale = Cesium.Cartesian3.maximumComponent(scale);
    radius *= maxScale;
    return new Cesium.TileBoundingSphere(center, radius);
}

function getUrl(fileName, layer){
    fileName = fileName.replace(/\+/g, '%2B');
    let url = layer._basePath;
    let isRealspace = layer._basePath.indexOf("realspace") > -1;
    if (!isRealspace) {
        return fileName;
    }

    let afterRealspace  = url.replace(/(.*realspace)/, "");
    let lastUrl = url.replace(/\/rest\/realspace/g,"").replace(afterRealspace,"");
    lastUrl += '/rest/realspace'+afterRealspace+'data/path/'+ fileName.replace(/^\.*/, "").replace(/^\//, "").replace(/\/$/, "");
    return lastUrl;
}

function createBoundingBox(box, transform) {
    if(box.center){
        const halfAxes = new Cesium.Matrix3();
        const center = new Cesium.Cartesian3(box.center.x, box.center.y, box.center.z);
        Cesium.Matrix4.multiplyByPoint(transform, center, center);

        const vx = new Cesium.Cartesian4(box.xExtent.x, box.xExtent.y, box.xExtent.z, 0);
        const vy = new Cesium.Cartesian4(box.yExtent.x, box.yExtent.y, box.yExtent.z, 0);
        const vz = new Cesium.Cartesian4(box.zExtent.x, box.zExtent.y, box.zExtent.z, 0);

        Cesium.Matrix4.multiplyByVector(transform, vx, vx);
        Cesium.Matrix4.multiplyByVector(transform, vy, vy);
        Cesium.Matrix4.multiplyByVector(transform, vz, vz);

        Cesium.Matrix3.setColumn(halfAxes, 0, vx, halfAxes);
        Cesium.Matrix3.setColumn(halfAxes, 1, vy, halfAxes);
        Cesium.Matrix3.setColumn(halfAxes, 2, vz, halfAxes);
        return new Cesium.TileOrientedBoundingBox(center, halfAxes);
    }

    const points = [];
    points.push(new Cesium.Cartesian3(box.min.x, box.min.y, box.min.z));
    points.push(new Cesium.Cartesian3(box.min.x, box.min.y, box.max.z));
    points.push(new Cesium.Cartesian3(box.min.x, box.max.y, box.min.z));
    points.push(new Cesium.Cartesian3(box.min.x, box.max.y, box.max.z));
    points.push(new Cesium.Cartesian3(box.max.x, box.min.y, box.min.z));
    points.push(new Cesium.Cartesian3(box.max.x, box.min.y, box.max.z));
    points.push(new Cesium.Cartesian3(box.max.x, box.max.y, box.min.z));
    points.push(new Cesium.Cartesian3(box.max.x, box.max.y, box.max.z));

    for(let i = 0; i < 8; i++){
        points[i] = Cesium.Matrix4.multiplyByPoint(transform, points[i], points[i]);
    }

    const orientedBoundingBox = Cesium.OrientedBoundingBox.fromPoints(points);
    return new Cesium.TileOrientedBoundingBox(orientedBoundingBox.center, orientedBoundingBox.halfAxes);
}

S3MTile.prototype.createBoundingVolume = function(parameter, transform) {
    if (Cesium.defined(parameter.sphere)) {
        return createSphere(parameter.sphere, transform);
    }

    if(Cesium.defined(parameter.box)) {
        return createBoundingBox(parameter.box, transform);
    }

    if(parameter.boundingVolume) {
        return parameter;
    }

    return undefined;
};

S3MTile.prototype.distanceToTile = function(frameState) {
    let boundingVolume = this.boundingVolume;
    return boundingVolume.distanceToCamera(frameState);
};

let scratchToTileCenter = new Cesium.Cartesian3();

S3MTile.prototype.distanceToTileCenter = function (frameState) {
    const tileBoundingVolume = this.boundingVolume;
    const boundingVolume = tileBoundingVolume.boundingVolume;
    const toCenter = Cesium.Cartesian3.subtract(boundingVolume.center, frameState.camera.positionWC, scratchToTileCenter);
    return Cesium.Cartesian3.dot(frameState.camera.directionWC, toCenter);
};

S3MTile.prototype.visibility = function(frameState, parentVisibilityPlaneMask) {
    let boundingVolume = this.boundingVolume;
    return frameState.cullingVolume.computeVisibilityWithPlaneMask(boundingVolume, parentVisibilityPlaneMask);
};

let scratchCartesian = new Cesium.Cartesian3();
function priorityDeferred(tile, frameState) {
    const camera = frameState.camera;
    const tileBoundingVolume = tile.boundingVolume;
    const boundingVolume = tileBoundingVolume.boundingVolume;
    const radius = boundingVolume.radius;
    const scaledCameraDirection = Cesium.Cartesian3.multiplyByScalar(camera.directionWC, tile.centerZDepth, scratchCartesian);
    const closestPointOnLine = Cesium.Cartesian3.add(camera.positionWC, scaledCameraDirection, scratchCartesian);
    const toLine = Cesium.Cartesian3.subtract(closestPointOnLine, boundingVolume.center, scratchCartesian);
    const distanceToCenterLine = Cesium.Cartesian3.magnitude(toLine);
    const notTouchingSphere = distanceToCenterLine > radius;
    if (notTouchingSphere) {
        const toLineNormalized = Cesium.Cartesian3.normalize(toLine, scratchCartesian);
        const scaledToLine = Cesium.Cartesian3.multiplyByScalar(toLineNormalized, radius, scratchCartesian);
        const closestOnSphere = Cesium.Cartesian3.add(boundingVolume.center, scaledToLine, scratchCartesian);
        const toClosestOnSphere = Cesium.Cartesian3.subtract(closestOnSphere, camera.positionWC, scratchCartesian);
        const toClosestOnSphereNormalize = Cesium.Cartesian3.normalize(toClosestOnSphere, scratchCartesian);
        tile.foveatedFactor = 1.0 - Math.abs(Cesium.Cartesian3.dot(camera.directionWC, toClosestOnSphereNormalize));
    } else {
        tile.foveatedFactor = 0.0;
    }
}


S3MTile.prototype.updateVisibility = function(frameState, layer) {
    let parent = this.parent;
    let parentVisibilityPlaneMask = Cesium.defined(parent) ? parent.visibilityPlaneMask : Cesium.CullingVolume.MASK_INDETERMINATE;
    this.distanceToCamera = this.distanceToTile(frameState);
    this.centerZDepth = this.distanceToTileCenter(frameState);
    this.visibilityPlaneMask = this.visibility(frameState, parentVisibilityPlaneMask);
    this.visible = this.visibilityPlaneMask !== Cesium.CullingVolume.MASK_OUTSIDE && this.distanceToCamera >= layer.visibleDistanceMin && this.distanceToCamera <=  layer.visibleDistanceMax;
    priorityDeferred(this, frameState);
};

function createPriorityFunction(tile) {
    return function() {
        return tile.priority;
    };
}

function getContentFailedFunction(tile) {
    return function(error) {
        tile.contentState = ContentState.FAILED;
        tile.contentReadyPromise.reject(error);
    };
}

function createChildren(parent, datas) {
    let layer = parent.layer;
    let length = datas.length;
    for(let i = 0;i < length;i++){
        const pageLod = new S3MPageLod();
        let data = datas[i];
        let boundingVolume = data.boundingVolume;
        let fileName = data.childTile;
        pageLod.fileName = parent.baseUri + fileName;
        pageLod.rangeData = data.rangeList;
        pageLod.rangeMode = data.rangeMode;
        if(boundingVolume.obb){
            pageLod.boundingVolume = createBoundingBox(boundingVolume.obb, layer.modelMatrix);
        }
        else if(boundingVolume.box) {
            pageLod.boundingVolume = createBoundingBox(boundingVolume.box, layer.modelMatrix);
        }
        else if(boundingVolume.sphere) {
            if(boundingVolume.isCalc){
                pageLod.boundingVolume = new Cesium.TileBoundingSphere(boundingVolume.sphere.center, boundingVolume.sphere.radius);
            }
            else{
                pageLod.boundingVolume = createSphere(boundingVolume.sphere, layer.modelMatrix);
            }
        }

        pageLod.renderEntities = data.renderEntities;
        pageLod.isLeafTile = data.isLeafTile;
        parent.pageLods.push(pageLod);
    }
}



function contentReadyFunction(layer, tile, arrayBuffer) {
    layer._cache.add(tile);

    let content;
    if(tile.fileExtension === 's3mb'){
        content = S3ModelParser.parseBuffer(arrayBuffer);
    }

    if(!content){
        tile.contentState = ContentState.FAILED;
        tile.contentReadyPromise.reject();
        return ;
    }

    let data = S3MContentParser.parse(layer, content, tile);

    createChildren(tile, data);
    tile.contentState = ContentState.LOADED;
    tile.contentReadyPromise && tile.contentReadyPromise.resolve(content);
}

S3MTile.prototype.requestContent = function() {
    let that = this;
    let layer = this.layer;
    let resource = this.contentResource.clone();
    let request = new Cesium.Request({
        throttle : true,
        throttleByServer : true,
        type : Cesium.RequestType.TILES3D,
        priorityFunction : createPriorityFunction(this),
        serverKey : this.serverKey
    });

    this.request = request;
    resource.request = request;

    let promise = resource.fetchArrayBuffer();

    if (!Cesium.defined(promise)) {
        return false;
    }

    this.contentState = ContentState.LOADING;
    this.contentReadyPromise = defer();
    let contentFailedFunction = getContentFailedFunction(this);

    promise.then(function(arrayBuffer) {
        if (that.isDestroyed()) {
            contentFailedFunction();
            return;
        }

        contentReadyFunction(layer, that, arrayBuffer);
    },(error)=>{
        if (request.state === Cesium.RequestState.CANCELLED) {
            that.contentState = ContentState.UNLOADED;
            return;
        }

        contentFailedFunction(error);
    });

    return true;
};

function priorityNormalizeAndClamp(value, minimum, maximum) {
    return Math.max(Cesium.Math.normalize(value, minimum, maximum) - Cesium.Math.EPSILON7, 0.0);
}

function isolateDigits(normalizedValue, numberOfDigits, leftShift) {
    let scaled = normalizedValue * Math.pow(10, numberOfDigits);
    let integer = parseInt(scaled);
    return integer * Math.pow(10, leftShift);
}

S3MTile.prototype.updatePriority = function(layer, frameState) {
    let minimumPriority = layer._minimumPriority;
    let maximumPriority = layer._maximumPriority;
    let leftShift = 4;
    let digitsCount = 4;

    let normalizedFoveatedFactor = priorityNormalizeAndClamp(this.foveatedFactor, minimumPriority.foveatedFactor, maximumPriority.foveatedFactor);
    let foveatedDigits = isolateDigits(normalizedFoveatedFactor, digitsCount, leftShift);

    leftShift = 8;
    let normalizedPixel = priorityNormalizeAndClamp(this.pixel, minimumPriority.pixel, maximumPriority.pixel);
    let pixelDigits = isolateDigits(1.0 - normalizedPixel, digitsCount, leftShift);

    leftShift = 0;
    let distancePriority = priorityNormalizeAndClamp(this.distanceToCamera, minimumPriority.distance, maximumPriority.distance);
    let distanceDigit = isolateDigits(distancePriority, digitsCount, leftShift);
    this.priority = foveatedDigits + pixelDigits + distanceDigit;
};

S3MTile.prototype.transformResource = function(frameState, layer) {
    const context = frameState.context;
    let allRenderable = true;
    for(let i = 0,j = this.pageLods.length;i < j;i++){
        const pagelod = this.pageLods[i];
        var isReady = true;
        for(let m = 0,n = pagelod.renderEntities.length;m < n;m++){
            const ro = pagelod.renderEntities[m];
            if(!ro.ready){
                allRenderable = false;
                isReady = false;
                try{
                    ro.transformResource(frameState, layer);
                }
                catch (e) {
                    console.log(e);
                }
            }
        }
        pagelod.ready = isReady;
    }

    if(allRenderable){
        this.contentState = ContentState.READY;
    }
};

S3MTile.prototype.update = function(frameState, layer) {
};

S3MTile.prototype.free = function() {
    this.contentState = ContentState.UNLOADED;
    this.request = undefined;
    this.cacheNode = undefined;
    this.contentReadyPromise = undefined;
    for(let i = 0,j = this.pageLods.length;i < j;i++){
        const pageLod = this.pageLods[i];
        pageLod.destroy();
    }

    this.pageLods.length = 0;
};

S3MTile.prototype.isDestroyed = function() {
    return false;
};

S3MTile.prototype.destroy = function() {
    this.free();
    return Cesium.destroyObject(this);
};

export default S3MTile;