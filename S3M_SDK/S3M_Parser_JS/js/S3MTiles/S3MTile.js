define([
    './Enum/ContentState',
    './S3ModelParser',
    './S3MContentParser',
    ], function(
    ContentState,
    S3ModelParser,
    S3MContentParser
        ) {
    'use strict';

    function S3MTile(layer, parent, boundingVolume, fileName, rangeData, renderEntityMap, isLeafTile) {
        this.layer = layer;
        this.parent = parent;
        this.fileName = fileName;
        this.isLeafTile = Cesium.defaultValue(isLeafTile, false);
        this.boundingVolume = this.createBoundingVolume(boundingVolume, layer.modelMatrix);
        let baseResource = Cesium.Resource.createIfNeeded(layer._baseResource);
        if(Cesium.defined(parent)){
            this.baseUri = parent.baseUri;
        }
        else{
            let resource = new Cesium.Resource(fileName);
            this.baseUri = resource.getBaseUri();
        }

        this.contentResource = baseResource.getDerivedResource({
            url : fileName
        });

        this.serverKey = Cesium.RequestScheduler.getServerKey(this.contentResource.getUrlComponent());
        this.request = undefined;
        this.cacheNode = undefined;
        this.distanceToCamera = 0;
        this.pixel = 0;
        this.visibilityPlaneMask = 0;
        this.visible = false;
        this.children = [];
        this.lodRangeData = rangeData;
        this.renderEntityMap = renderEntityMap;
        this.contentState = ContentState.UNLOADED;
        this.readerable = false;
        this.touchedFrame = 0;
        this.requestedFrame = 0;
        this.selectedFrame = 0;
        this.priority = 0;
    }

    Object.defineProperties(S3MTile.prototype, {
        renderable : {
            get : function() {
                return Cesium.defined(this.renderEntityMap);
            }
        }
    });

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

    function createBoundingBox(box, transform) {
        let min = new Cesium.Cartesian3(box.min.x, box.min.y, box.min.z);
        Cesium.Matrix4.multiplyByPoint(transform, min, min);
        let max = new Cesium.Cartesian3(box.max.x, box.max.y, box.max.z);
        Cesium.Matrix4.multiplyByPoint(transform, max, max);
        let sphere = Cesium.BoundingSphere.fromCornerPoints(min, max, new Cesium.BoundingSphere());
        let center = sphere.center;
        let radius = sphere.radius;
        let scale = Cesium.Matrix4.getScale(transform, scratchScale);
        let maxScale = Cesium.Cartesian3.maximumComponent(scale);
        radius *= maxScale;
        return new Cesium.TileBoundingSphere(center, radius);
    }

    S3MTile.prototype.createBoundingVolume = function(parameter, transform) {
        if(this.isLeafTile) {
            return new Cesium.TileBoundingSphere(parameter.center, parameter.radius);
        }

        if (Cesium.defined(parameter.sphere)) {
            return createSphere(parameter.sphere, transform);
        }
        else if(Cesium.defined(parameter.box)) {
            return createBoundingBox(parameter.box, transform);
        }

        return undefined;
    };

    S3MTile.prototype.canTraverse = function() {
        if (this.children.length === 0 || this.isLeafTile) {
            return false;
        }

        if(!Cesium.defined(this.lodRangeData)) {
            return true;
        }

        return this.pixel > this.lodRangeData;
    };

    function getBoundingVolume (tile, frameState) {
        return tile.boundingVolume;
    }

    S3MTile.prototype.getPixel = function(frameState) {
        let boundingVolume = this.boundingVolume;
        let radius = boundingVolume.radius;
        let center = boundingVolume.center;
        let distance = Cesium.Cartesian3.distance(frameState.camera.positionWC, center);
        let height = frameState.context.drawingBufferHeight;
        let theta = frameState.camera.frustum._fovy * 0.5;
        let screenYPix = height * 0.5;
        let lamat = screenYPix / Math.tan(theta);
        return lamat * radius / distance;
    };

    S3MTile.prototype.distanceToTile = function(frameState) {
        let boundingVolume = getBoundingVolume(this, frameState);
        return boundingVolume.distanceToCamera(frameState);
    };

    S3MTile.prototype.visibility = function(frameState, parentVisibilityPlaneMask) {
        let boundingVolume = getBoundingVolume(this, frameState);
        return frameState.cullingVolume.computeVisibilityWithPlaneMask(boundingVolume, parentVisibilityPlaneMask);
    };

    S3MTile.prototype.updateVisibility = function(frameState) {
        let parent = this.parent;
        let parentVisibilityPlaneMask = Cesium.defined(parent) ? parent.visibilityPlaneMask : Cesium.CullingVolume.MASK_INDETERMINATE;
        this.distanceToCamera = this.distanceToTile(frameState);
        this.pixel = this.getPixel(frameState);
        this.visibilityPlaneMask = this.visibility(frameState, parentVisibilityPlaneMask);
        this.visible = this.visibilityPlaneMask !== Cesium.CullingVolume.MASK_OUTSIDE;
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
            let data = datas[i];
            let boundingVolume = data.boundingVolume;
            let fileName = data.rangeDataList;
            fileName = parent.baseUri + fileName;
            let rangeData = data.rangeList;
            let renderEntitieMap = data.geoMap;
            let tile = new S3MTile(layer, parent, boundingVolume, fileName, rangeData, renderEntitieMap, data.isLeafTile);
            parent.children.push(tile);
            parent.layer._cache.add(tile);
        }
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
        this.contentReadyPromise = Cesium.when.defer();
        let contentFailedFunction = getContentFailedFunction(this);

        promise.then(function(arrayBuffer) {
            if (that.isDestroyed()) {
                contentFailedFunction();
                return;
            }

            let content = S3ModelParser.parseBuffer(arrayBuffer);
            let data = S3MContentParser.parse(that.layer, content);
            createChildren(that, data);
            that.selectedFrame = 0;
            that.contentState = ContentState.READY;
            //that.renderable = true;
            that.contentReadyPromise.resolve(content);
        }).otherwise(function(error) {
            if (request.state === Cesium.RequestState.CANCELLED) {
                that.contentState = ContentState.UNLOADED;
                return;
            }

            contentFailedFunction(error);
        });

        return true;
    };

    S3MTile.prototype.update = function(frameState) {
        let renderEntityMap = this.renderEntityMap;

        for(let key in  renderEntityMap) {
            if(renderEntityMap.hasOwnProperty(key)) {
                renderEntityMap[key].update(frameState);
            }
        }
    };

    S3MTile.prototype.free = function() {
        for(let i = 0,j = this.children.length;i < j;i++) {
            let child = this.children[i];
            child.destroy();
        }

        this.children.length = 0;
        this.contentState = ContentState.UNLOADED;
    };

    S3MTile.prototype.isDestroyed = function() {
        return false;
    };

    S3MTile.prototype.destroy = function() {
        for(let key in this.renderEntityMap) {
            if(this.renderEntityMap.hasOwnProperty(key)) {
                this.renderEntityMap[key].destroy();
            }
        }

        this.renderEntityMap = undefined;

        for(let i = 0,j = this.children.length;i < j;i++) {
            let child = this.children[i];
            child.destroy();
        }

        this.children.length = 0;

        return Cesium.destroyObject(this);
    };

    return S3MTile;
});
