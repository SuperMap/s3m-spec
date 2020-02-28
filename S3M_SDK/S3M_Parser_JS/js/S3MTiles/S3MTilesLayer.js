define([
    './S3MTile',
    './S3MLayerScheduler',
    './S3MLayerCache'
    ], function(
    S3MTile,
    S3MLayerScheduler,
    S3MLayerCache
        ) {
    'use strict';

    function S3MTilesLayer(options) {
        options = Cesium.defaultValue(options, Cesium.defaultValue.EMPTY_OBJECT);
        Cesium.Check.defined('options.url', options.url);
        Cesium.Check.defined('options.context', options.context);
        this.context = options.context;
        this._url = undefined;
        this._basePath = undefined;
        this._baseResource = undefined;
        this.modelMatrix = undefined;
        this.fileType = undefined;
        this._position = undefined;
        this._rectangle = undefined;
        this._rootTiles = [];
        this._schuduler = new S3MLayerScheduler();
        this._requestTiles = [];
        this._selectedTiles = [];
        this._cache = new S3MLayerCache();
        this._maximumMemoryUsage = -1;
        this._totalMemoryUsageInBytes = 0;
        this._readyPromise = Cesium.when.defer();
        this.loadConfig(options.url);
    }

    Object.defineProperties(S3MTilesLayer.prototype, {
        ready : {
            get : function() {
                return this._rootTiles.length > 0;
            }
        },
        readyPromise : {
            get : function() {
                return this._readyPromise;
            }
        },
        rectangle : {
            get : function() {
                return this._rectangle;
            }
        },
        totalMemoryUsageInBytes : {
            get : function() {
                return this._totalMemoryUsageInBytes;
            },
            set : function(value) {
                this._totalMemoryUsageInBytes = value;
            }
        },
        maximumMemoryUsage : {
            get : function() {
                return this._maximumMemoryUsage;
            },
            set : function(value) {
                this._maximumMemoryUsage = value;
            }
        }
    });

    S3MTilesLayer.prototype.loadConfig = function(url) {
        let that = this;
        Cesium.when(url)
            .then(function(url) {
                let basePath;
                let resource = Cesium.Resource.createIfNeeded(url);
                basePath = resource.getBaseUri(true);
                that._url = resource.url;
                that._basePath = basePath;
                that._baseResource = resource;
                return resource.fetchJson();
            })
            .then(function(config) {
                let extensions = config.extensions;
                that.fileType = extensions["s3m:FileType"];
                let lon = config.position.x;
                let lat = config.position.y;
                let height = config.position.z;
                that._position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
                that.modelMatrix = Cesium.Transforms.eastNorthUpToFixedFrame(that._position);
                that._rectangle = Cesium.Rectangle.fromDegrees(config.geoBounds.left, config.geoBounds.bottom, config.geoBounds.right, config.geoBounds.top);

                for (let i = 0, len = config.tiles.length; i < len; i++) {
                    let fileName = config.tiles[i].url;
                    let boundingVolume = {
                        box : config.tiles[i].boundingbox
                    };

                    let tile = new S3MTile(that, undefined, boundingVolume, fileName);
                    that._cache.add(tile);
                    that._rootTiles.push(tile);
                }

                that._readyPromise.resolve(that);
            }).otherwise(function(error) {
                that._readyPromise.reject(error);
        });
    };

    function sortRequestByPriority(a, b) {
        return a.priority - b.priority;
    }

    function requestTiles(layer) {
        let requestTiles = layer._requestTiles;
        let length = requestTiles.length;
        requestTiles.sort(sortRequestByPriority);
        for (let i = 0; i < length; ++i) {
            let tile = requestTiles[i];
            tile.requestContent();
        }
    }

    function updateTiles(layer, frameState) {
        let selectedTiles = layer._selectedTiles;
        let length = selectedTiles.length;
        for(let i = 0;i < length;i++){
            selectedTiles[i].update(frameState);
        }
    }

    function unloadTile(layer, tile) {
        //tile.free();
    }

    function freeResource(layer) {
        layer._cache.unloadTiles(layer, unloadTile);
    }

    S3MTilesLayer.prototype.prePassesUpdate = function(frameState) {
        if (!this.ready) {
            return;
        }

        if (frameState.newFrame) {
            this._cache.reset();
        }
    };

    S3MTilesLayer.prototype.update = function(frameState) {
        if(!this.ready) {
            return ;
        }

        this._schuduler.scheduler(this, frameState);
        requestTiles(this);
        updateTiles(this, frameState);
        freeResource(this);
    };

    return S3MTilesLayer;
});
