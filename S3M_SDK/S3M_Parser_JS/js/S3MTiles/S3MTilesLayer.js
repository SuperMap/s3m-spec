define([
    './XmlParser',
    './S3MTile',
    './S3MLayerScheduler',
    './S3MLayerCache'
    ], function(
    XmlParser,
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
                if(resource.extension === 'scp') {
                    return resource.fetchXML();
                }

                if (resource.extension === 'json') {
                    return resource.fetchJson();
                }
            })
            .then(function(doc) {
                let rootNode = doc.firstChild;
                let namespace = rootNode.namespace;
                that.fileType = XmlParser.queryStringValue(rootNode, 'FileType', namespace);
                let positionNode = XmlParser.queryFirstNode(rootNode, 'Position', namespace);
                let lon = XmlParser.queryNumericValue(positionNode, 'X', namespace);
                let lat = XmlParser.queryNumericValue(positionNode, 'Y',namespace);
                let height = XmlParser.queryNumericValue(positionNode, 'Z', namespace);
                that._position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
                that.modelMatrix = Cesium.Transforms.eastNorthUpToFixedFrame(that._position);
                let boundsNode = XmlParser.queryFirstNode(rootNode, 'Bounds', namespace);
                if(Cesium.defined(boundsNode)){
                    let left = XmlParser.queryNumericValue(boundsNode, 'Left', namespace);
                    let top = XmlParser.queryNumericValue(boundsNode, 'Top', namespace);
                    let right = XmlParser.queryNumericValue(boundsNode, 'Right', namespace);
                    let bottom = XmlParser.queryNumericValue(boundsNode, 'Bottom', namespace);
                    that._rectangle = Cesium.Rectangle.fromDegrees(left, bottom, right, top);
                }

                let osgFilesNode = XmlParser.queryFirstNode(rootNode, 'OSGFiles', namespace);
                let filesNodes = XmlParser.queryNodes(osgFilesNode, 'Files', namespace);
                for(let i = 0,j = filesNodes.length;i < j;i++){
                    let fileNode = filesNodes[i];
                    let fileName = XmlParser.queryStringValue(fileNode, 'FileName', namespace);
                    fileName = fileName.replace(/\\/gm, '\/');
                    let sphereNode = XmlParser.queryFirstNode(fileNode, 'BoundingSphere', namespace);
                    let boundingVolume;
                    if(Cesium.defined(sphereNode) && Cesium.defined(sphereNode.childNodes) && sphereNode.childNodes.length){
                        let centerX = XmlParser.queryNumericValue(sphereNode, 'CenterX', namespace);
                        let centerY = XmlParser.queryNumericValue(sphereNode, 'CenterY', namespace);
                        let centerZ = XmlParser.queryNumericValue(sphereNode, 'CenterZ', namespace);
                        let radius = XmlParser.queryNumericValue(sphereNode, 'Radius', namespace);
                        let center = new Cesium.Cartesian3(centerX, centerY, centerZ);
                        boundingVolume = {
                            sphere : {
                                center : center,
                                radius : radius
                            }
                        };
                    }

                    let tile = new S3MTile(that, undefined, boundingVolume, fileName);
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
            requestTiles[i].requestContent();
        }
    }

    function updateTiles(layer, frameState) {
        let selectedTiles = layer._selectedTiles;
        let length = selectedTiles.length;
        for(let i = 0;i < length;i++){
            selectedTiles[i].update(frameState);
        }
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
    };

    return S3MTilesLayer;
});
