import S3MTile from './S3MTile.js';
import S3MLayerScheduler from './S3MLayerScheduler.js';
import S3MLayerCache from './S3MLayerCache.js';
import OperationType from './Enum/OperationType.js';
import XmlParser from './XmlParser.js';
import Style3D from './Style3D.js';
import defer from './defer.js';
import Uri from './Uri.js';
import CRNTranscoder from "./Core/CRNTranscoder.js";
import TextureCompressType from './Enum/TextureCompressType.js';

var transform_2d = new Cesium.Matrix4(
    0.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0);

function S3MTilesLayer(options) {
    options = Cesium.defaultValue(options, Cesium.defaultValue.EMPTY_OBJECT);
    Cesium.Check.defined('options.context', options.context);
    // Cesium.Check.defined('Authentication error', options.rss);
    this.id = Cesium.createGuid();
    this.name = options.name;
    this.context = options.context;
    this._isS3MB = Cesium.defaultValue(options.isS3MB, true);
    this._url = undefined;
    this._basePath = undefined;
    this._baseResource = undefined;
    this.modelMatrix = new Cesium.Matrix4();
    this.invModelMatrix = new Cesium.Matrix4();
    this.fileType = undefined;
    this.dataVersion = 1;
    this._position = undefined;
    this._rectangle = undefined;
    this._rootTiles = [];
    this._schuduler = new S3MLayerScheduler();
    this._requestTiles = [];
    this._processTiles = [];
    this._renderQueue = [];
    this._cache = new S3MLayerCache();
  
    // 顶点压缩类型，例如MeshOPT
    this._vertexCompressionType = options.vertexCompressionType;
    this._textureCompressType = TextureCompressType.None;
    this._maximumMemoryUsage = -1;
    this._totalMemoryUsageInBytes = 0;
    this._maximumPriority = { foveatedFactor: -Number.MAX_VALUE, depth: -Number.MAX_VALUE, distance: -Number.MAX_VALUE, pixel : -Number.MAX_VALUE};
    this._minimumPriority = { foveatedFactor: Number.MAX_VALUE, depth: Number.MAX_VALUE, distance: Number.MAX_VALUE, pixel : Number.MAX_VALUE};
    this._readyPromise = defer();
    this._selectEnabled = Cesium.defaultValue(options.selectEnabled, true);
    this._multiChoose = false;
    this._sceneMode = Cesium.defaultValue(options.sceneMode, Cesium.SceneMode.SCENE3D);
    this._selections = [];
    this._objsOperationList = {};
    this._selectedColor = new Cesium.Color(0.7, 0.7, 1.0, 1.0);
    this._minCategory = 0;
    this._maxCategory = 0;
    this._minHeight = 0;
    this._maxHeight = 0;
    this._brightness = Cesium.defaultValue(options.brightness, 1.0);//亮度
    this._contrast = Cesium.defaultValue(options.constrast, 1.0);//对比度
    this._hue = Cesium.defaultValue(options.hue, 0.0);//色调
    this._saturation = Cesium.defaultValue(options.saturation, 1.0);//饱和度
    this._gamma = Cesium.defaultValue(options.gamma, 1.0);//灰度
    this._visibleDistanceMax = Cesium.defaultValue(options.maxVisibleDistance, Number.MAX_VALUE);
    this._visibleDistanceMin = Cesium.defaultValue(options.minVisibleDistance, 0.0);
    this._minVisibleAltitude = Cesium.defaultValue(options.minVisibleAltitude, 0.0);
    this._maxVisibleAltitude = Cesium.defaultValue(options.maxVisibleAltitude, Number.MAX_VALUE);
    this._lodRangeScale = Cesium.defaultValue(options.lodRangeScale, 1.0);
    this._visible = Cesium.defaultValue(options.isVisible, true);
    this._style3D = Cesium.defaultValue(options.style3D, new Style3D());
    this._isTransparencyOptimization = false;
    this._polygonOffsetConfig = options.polygonOffset;
    this._visibleViewport = 0xFFF;
    this._waterParameters = undefined;
    this._waterPlanes = undefined;
    this._edgeDistanceFalloffFactor = 0;
    this._edgeCurrentTotalLength = 0;
    this._edgeCurrentCount = 0;
    this._materialManager = undefined;

    this.loadConfig(options.url);
}

Object.defineProperties(S3MTilesLayer.prototype, {
    ready : {
        get : function() {
            if(this.fileType === 'OSGBCacheFile_Water'){
                return this._waterParameters !== undefined && this._rootTiles.length > 0;
            }

            return this._rootTiles.length > 0;
        }
    },
    readyPromise : {
        get : function() {
            return this._readyPromise.promise;
        }
    },
    visible : {
        get : function() {
            return this._visible;
        },
        set : function(value) {
            this._visible = value;
        }
    },
    show : {
        get : function() {
            return this._visible;
        },
        set : function(value) {
            this._visible = value;
        }
    },
    style3D : {
        get : function() {
            return this._style3D;
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
    },
    lodRangeScale : {
        get : function() {
            return this._lodRangeScale;
        },
        set : function(value) {
            Cesium.Check.typeOf.number('set layer lod range scale', value);
            this._lodRangeScale = value;
        }
    },
    selectedColor : {
        get : function() {
            return this._selectedColor;
        },
        set : function(value) {
            Cesium.Color.clone(value, this._selectedColor);
        }
    },
    dataMinValue : {
        get : function() {
            return Cesium.defaultValue(this._minCategory, this._minHeight);
        }
    },
    dataMaxValue : {
        get : function() {
            return Cesium.defaultValue(this._maxCategory, this._maxHeight);
        }
    },
    visibleDistanceMax : {
        get : function(){
            return this._visibleDistanceMax;
        },
        set : function(value){
            Cesium.Check.typeOf.number('max visible distance', value);
            this._visibleDistanceMax = value;
        }
    },
    visibleDistanceMin : {
        get : function(){
            return this._visibleDistanceMin;
        },
        set : function(value){
            Cesium.Check.typeOf.number('min visible distance', value);
            this._visibleDistanceMin = value;
        }
    },
    minVisibleAltitude : {
        get : function() {
            return this._minVisibleAltitude;
        },
        set : function(value) {
            Cesium.Check.typeOf.number('min visible altitude', value);
            this._minVisibleAltitude = value;
        }
    },
    maxVisibleAltitude : {
        get : function() {
            return this._maxVisibleAltitude;
        },
        set : function(value) {
            Cesium.Check.typeOf.number('max visible altitude', value);
            this._maxVisibleAltitude = value;
        }
    },
 
    selectEnabled: {
        get : function(){
            return this._selectEnabled;
        },
        set : function(value){
            Cesium.Check.defined('value',value);
            Cesium.Check.typeOf.bool('value',value);
            this._selectEnabled = value;
        }
    },

});

function getWaterEffect(doc){
    let WaterEffect = {};
    let rootNode = doc.firstChild;
    WaterEffect.version = XmlParser.queryStringValue(rootNode,'Version');
    WaterEffect.fileType = XmlParser.queryStringValue(rootNode,'FileType');
    let waterEffectNode = XmlParser.queryFirstNode(rootNode,'WaterEffect');
    WaterEffect.averageHeight = XmlParser.queryNumericValue(waterEffectNode,'AverageHeight');
    let gpuParsNodes = XmlParser.queryNodes(waterEffectNode,'GpuProgramParameters');
    WaterEffect.gpuProgramParameters = [];
    for(let i = 0,j = gpuParsNodes.length;i < j;i++){
        let obj = {};
        obj.gpuConstants = [];
        obj.atuoConstants = [];
        let gpuParsNode = gpuParsNodes[i];
        let gpuConstantsNode = XmlParser.queryFirstNode(gpuParsNode,'GpuConstants');
        if(gpuConstantsNode){
            let gpuConstantDefinitionNodes = XmlParser.queryNodes(gpuConstantsNode,'GpuConstantDefinition');
            for(let m = 0,n = gpuConstantDefinitionNodes.length;m < n;m++){
                let tmp = {};
                let gpuConstantDefinitionNode = gpuConstantDefinitionNodes[m];
                tmp.constType = XmlParser.queryNumericValue(gpuConstantDefinitionNode,'ConstType');
                tmp.index = XmlParser.queryNumericValue(gpuConstantDefinitionNode,'Index');
                tmp.name = XmlParser.queryStringValue(gpuConstantDefinitionNode,'Name');
                tmp.arraySize = XmlParser.queryNumericValue(gpuConstantDefinitionNode,'ArraySize');
                tmp.multiple = XmlParser.queryNumericValue(gpuConstantDefinitionNode,'Multiple');
                let arrayFloatNode = XmlParser.queryFirstNode(gpuConstantDefinitionNode,'ArrayFloat');
                tmp.arrayFloat = [];
                if(arrayFloatNode){
                    let floatNodes = XmlParser.queryNodes(arrayFloatNode,'Float');
                    let len = 0;
                    while(len < tmp.arraySize){
                        let fValue = parseFloat(floatNodes[len].textContent);
                        tmp.arrayFloat.push(fValue);
                        len++;
                    }
                }
                obj.gpuConstants.push(tmp);
            }
        }

        let autoConstantsNode = XmlParser.queryFirstNode(gpuParsNode,'AutoConstants');
        if(autoConstantsNode){
            let AutoConstantEntryNodes = XmlParser.queryNodes(autoConstantsNode,'AutoConstantEntry');
            for(let m = 0,n = AutoConstantEntryNodes.length;m < n;m++){
                let tmp = {};
                let AutoConstantEntryNode = AutoConstantEntryNodes[m];
                tmp.paramType = XmlParser.queryNumericValue(AutoConstantEntryNode,'ParamType');
                tmp.name = XmlParser.queryStringValue(AutoConstantEntryNode,'Name');
                tmp.physicalIndex = XmlParser.queryNumericValue(AutoConstantEntryNode,'PhysicalIndex');
                tmp.elementCount = XmlParser.queryNumericValue(AutoConstantEntryNode,'ElementCount');
                tmp.data = XmlParser.queryNumericValue(AutoConstantEntryNode,'Data');
                tmp.fData = XmlParser.queryNumericValue(AutoConstantEntryNode,'FData');
                tmp.isReal = XmlParser.queryBooleanValue(AutoConstantEntryNode,'IsReal');
                obj.atuoConstants.push(tmp);
            }
        }

        WaterEffect.gpuProgramParameters.push(obj);
    }

    return WaterEffect;
}

function getUrl(fileName, layer){
    let url = layer._basePath;
    let isRealspace = layer._basePath.indexOf("realspace") > -1;
    if (!isRealspace) {
        return fileName;
    }

    let afterRealspace  = url.replace(/(.*realspace)/, "");
    let lastUrl = url.replace(/\/rest\/realspace/g,"").replace(afterRealspace,"");
    return lastUrl +'/rest/realspace'+afterRealspace+'data/path/'+ fileName.replace(/^\.*/, "").replace(/^\//, "").replace(/\/$/, "");
}

function parseWaterParameters(layer, config) {
    let extensions = config.extensions;
    if(!extensions || !extensions.attachFiles){
        return ;
    }

    let attachFiles = extensions.attachFiles;
    let promises = [];
    let baseUri = layer._basePath;
    let isWater = false;
    for(let i = 0,j = attachFiles.length;i < j;i++){
        let attachFileName = attachFiles[i].attachFile;
        if(attachFileName.indexOf('.water') > 0){
            let relativePath = getUrl(attachFileName, layer);
            let resource = layer._baseResource.getDerivedResource({
                url : relativePath
            });
            promises.push(resource.fetchXML());
            isWater = true;
        }
    }

    if(!promises.length){
        return ;
    }

     Promise.all(promises).then((docs)=>{
        let waterEffectArr = [];
        for(let i = 0,j = docs.length;i < j;i++){
            let doc = docs[i];
            if(!doc){
                break;
            }
            let WaterEffect = getWaterEffect(doc);
            waterEffectArr.push(WaterEffect);
        }


        layer._waterPlanes = new Cesium.AssociativeArray();
        layer._waterParameters = {};
        let waterEffect = waterEffectArr[0];
        layer._waterParameters['averageHeight'] = waterEffect.averageHeight;
        for(let i = 0;i < waterEffect.gpuProgramParameters.length; i++){
            let gpuParameter = waterEffect.gpuProgramParameters[i];
            for(let j = 0; j < gpuParameter.atuoConstants.length; j++){
                let autoContant = gpuParameter.atuoConstants[j];
                if(autoContant.name === "timeVal"){
                    layer._waterParameters[autoContant.name] = autoContant.fData;
                    break;
                }
            }

            for(let j = 0; j < gpuParameter.gpuConstants.length; j++){
                let gpuContant = gpuParameter.gpuConstants[j];
                let gpuValue = null;
                switch(gpuContant.arraySize){
                    case 1 : gpuValue = gpuContant.arrayFloat[0];break;
                    case 2 : gpuValue = new Cesium.Cartesian2(); Cesium.Cartesian2.unpack(gpuContant.arrayFloat, 0, gpuValue);break;
                    case 3 : gpuValue = new Cesium.Cartesian3(); Cesium.Cartesian3.unpack(gpuContant.arrayFloat, 0, gpuValue);break;
                    case 4 : gpuValue = new Cesium.Cartesian4(); Cesium.Cartesian4.unpack(gpuContant.arrayFloat, 0, gpuValue);break;
                    default : break;
                }

                if(!gpuValue){
                    continue;
                }

                layer._waterParameters[gpuContant.name] = gpuValue;
            }
        }
        
        layer.fileType += '_Water';
     }).catch((e)=>{
        console.log(e)
     });
}

S3MTilesLayer.prototype.loadConfig = function(url) {
    let that = this;
    Promise.resolve(url).then(function(url) {
            let basePath;
            let resource = Cesium.Resource.createIfNeeded(url);
            basePath = resource.getBaseUri(true);
            let basePathUrl = new Uri(basePath);
            basePathUrl.query = undefined;
            basePath = basePathUrl.toString();
            that._url = resource.url;
            that._basePath = basePath;
            that._baseResource = resource;
      
            if(that._isS3MB){
                return resource.fetchJson();
            }

            return resource.fetchXML();
        })
        .then(function(config) {
            if(that._isS3MB){
                let extensions = config.extensions;
                that.fileType = extensions["s3m:FileType"];
                let position = Cesium.defaultValue(config.position.point3D, config.position);
                let lon = position.x;
                let lat = position.y;
                let height = position.z;
                let positionUnits = config.position.units;
                that.lon = lon;
                that.lat = lat;
                that.height = height;
                if(that._sceneMode === Cesium.SceneMode.SCENE3D){
                    that._position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
                    that.modelMatrix = Cesium.Transforms.eastNorthUpToFixedFrame(that._position);
                    that.invModelMatrix = Cesium.Matrix4.inverse(that.modelMatrix, that.invModelMatrix);
                } else {
                    if ((lon > 180 || lon < -180 || lat > 180 || lat < -180) || positionUnits === "Meter") {
                        let posLon = lon;
                        let posLat = lat;
                        that._position = new Cesium.Cartesian3(posLon, posLat, height);
                    } else {
                        let projection = new Cesium.GeographicProjection();
                        let position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
                        let cartographic = projection.ellipsoid.cartesianToCartographic(position);
                        that._position = projection.project(cartographic);
                    }
                    Cesium.Matrix4.fromTranslation(that._position, that.modelMatrix);//得到平移矩阵
                    Cesium.Matrix4.multiply(transform_2d, that.modelMatrix, that.modelMatrix);//得到交换平移矩阵
                    that.invModelMatrix = Cesium.Matrix4.inverse(that.modelMatrix, that.invModelMatrix);
                }

                if(Cesium.defined(config.geoBounds)){
                    let left = config.geoBounds.left;
                    let top = config.geoBounds.top;
                    let right = config.geoBounds.right;
                    let bottom = config.geoBounds.bottom;
                    if (left > 180 || bottom > 180 || right > 180 || top > 180) { // 平面场景scp里存的不是经纬度
                        left /= 6378137;
                        bottom /= 6378137;
                        right /= 6378137;
                        top /= 6378137;
                        that._rectangle = new Cesium.Rectangle(left, bottom, right, top);
                    } else {
                        that._rectangle = Cesium.Rectangle.fromDegrees(left, bottom, right, top);
                    }
                }

                if(config.heightRange){
                    that._minHeight = config.heightRange.min;
                    that._maxHeight = config.heightRange.max;
                }

                if(config.wDescript){
                    let wDescript = config.wDescript.range;
                    that._minWValue = wDescript.min;
                    that._maxWValue = wDescript.max;
                }

                if(extensions.hasOwnProperty("s3m:TransparencyOptimization")){
                    that._isTransparencyOptimization = extensions["s3m:TransparencyOptimization"] === "TRUE";
                }

                if(extensions.hasOwnProperty("s3m:VertexCompressionType")){
                    that._vertexCompressionType = extensions["s3m:VertexCompressionType"];
                }

                if(extensions.hasOwnProperty("s3m:TextureCompressionType")){
                    that._textureCompressType = extensions["s3m:TextureCompressionType"];
                }

                // 3.0 为rootTiles
                const tiles = config.tiles ||  config.rootTiles;

                if(tiles.length > 0){
                    let fileExt = Cesium.getExtensionFromUri(tiles[0].url);
                }

                for (let i = 0, len = tiles.length; i < len; i++) {
                    let fileName = tiles[i].url;
                    let boundingVolume = {
                        box : tiles[i].boundingbox || tiles[i].boundingBox //兼容3.01
                    };
                    
                    let tile = new S3MTile(that, undefined, boundingVolume, fileName);
                    tile.isRootTile = true;
                    tile.blockKey = fileName;
                    that._cache.add(tile);
                    that._rootTiles.push(tile);
                }

                parseWaterParameters(that, config);
            }
            else{
                let rootNode = config.firstChild;
                let namespace = rootNode.namespace;
                that.fileType = XmlParser.queryStringValue(rootNode, 'FileType', namespace);
                let positionNode = XmlParser.queryFirstNode(rootNode, 'Position', namespace);
                let lon = XmlParser.queryNumericValue(positionNode, 'X', namespace);
                let lat = XmlParser.queryNumericValue(positionNode, 'Y',namespace);
                let height = XmlParser.queryNumericValue(positionNode, 'Z', namespace);
                that.lon = lon;
                that.lat = lat;
                that.height = height;
                that._position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
                that.modelMatrix = Cesium.Transforms.eastNorthUpToFixedFrame(that._position);
                that.invModelMatrix = Cesium.Matrix4.inverse(that.modelMatrix, that.invModelMatrix);
                let boundsNode = XmlParser.queryFirstNode(rootNode, 'Bounds', namespace);
                let boundingBoxNode = XmlParser.queryFirstNode(rootNode, 'BoundingBox', namespace);
                if(boundsNode){
                    let left = XmlParser.queryNumericValue(boundsNode, 'Left', namespace);
                    let top = XmlParser.queryNumericValue(boundsNode, 'Top', namespace);
                    let right = XmlParser.queryNumericValue(boundsNode, 'Right', namespace);
                    let bottom = XmlParser.queryNumericValue(boundsNode, 'Bottom', namespace);
                    that._rectangle = Cesium.Rectangle.fromDegrees(left, bottom, right, top);
                }
                else if(boundingBoxNode){
                    let minx = XmlParser.queryNumericValue(boundingBoxNode, 'MinX', namespace);
                    let miny = XmlParser.queryNumericValue(boundingBoxNode, 'MinY', namespace);
                    let minz = XmlParser.queryNumericValue(boundingBoxNode, 'MinZ', namespace);
                    let maxx = XmlParser.queryNumericValue(boundingBoxNode, 'MaxX', namespace);
                    let maxy = XmlParser.queryNumericValue(boundingBoxNode, 'MaxY', namespace);
                    let maxz = XmlParser.queryNumericValue(boundingBoxNode, 'MaxZ', namespace);
                    minx = 180 * Math.abs(minx) / (Math.PI*6378137);
                    miny = 180 * Math.abs(miny) / (Math.PI*6378137);
                    maxx = 180 * Math.abs(maxx) / (Math.PI*6378137);
                    maxy = 180 * Math.abs(maxy) / (Math.PI*6378137);
                    that._rectangle = Cesium.Rectangle.fromDegrees(lon - minx,lat - miny,lon + maxx,lat + maxy);
                }

                let heightRangeNode = XmlParser.queryFirstNode(rootNode, 'HeightRange', namespace);
                if(heightRangeNode){
                    that._minHeight = XmlParser.queryNumericValue(heightRangeNode, 'MinHeight', namespace);
                    that._maxHeight = XmlParser.queryNumericValue(heightRangeNode, 'MaxHeight', namespace);
                }

                let categoryRangeNode = XmlParser.queryFirstNode(rootNode, 'CategoryRange', namespace);
                if(categoryRangeNode){
                    that._maxWValue = XmlParser.queryNumericValue(categoryRangeNode, 'MaxCategory', namespace);
                    that._minWValue = XmlParser.queryNumericValue(categoryRangeNode, 'MinCategory', namespace);
                }

                let extensionsNode = XmlParser.queryFirstNode(rootNode, 'Extensions', namespace);
                that._isTransparencyOptimization = XmlParser.queryBooleanValue(extensionsNode, 'TransparencyOptimization', namespace);
                let reg = /\\+/g;
                let osgFilesNode = XmlParser.queryFirstNode(rootNode, 'OSGFiles', namespace);
                let filesNodes = XmlParser.queryNodes(osgFilesNode, 'Files', namespace);
                if(filesNodes.length > 0){
                    for(let i = 0,j = filesNodes.length;i < j;i++){
                        let fileNode = filesNodes[i];
                        let fileName = XmlParser.queryStringValue(fileNode, 'FileName', namespace);
                        fileName = fileName.replace(reg, '/');
                        fileName = fileName.replace(/(\.osgb)/gi, '.s3m');
                        let bSphereNode = XmlParser.queryFirstNode(fileNode, 'BoundingSphere', namespace);
                        let boundingVolume = {
                            sphere : {
                                center : {
                                    x : 0,
                                    y : 0,
                                    z : 0
                                },
                                radius : 6378137
                            }
                        };
                        if(bSphereNode && bSphereNode.childNodes.length){
                            let centerX = XmlParser.queryNumericValue(bSphereNode, 'CenterX', namespace);
                            let centerY = XmlParser.queryNumericValue(bSphereNode, 'CenterY', namespace);
                            let centerZ = XmlParser.queryNumericValue(bSphereNode, 'CenterZ', namespace);
                            let radius = XmlParser.queryNumericValue(bSphereNode, 'Radius', namespace);
                            boundingVolume = {
                                sphere : {
                                    center : {
                                        x : centerX,
                                        y : centerY,
                                        z : centerZ
                                    },
                                    radius : radius
                                }
                            };
                        }


                        let tile = new S3MTile(that, undefined, boundingVolume, fileName);
                        tile.isRootTile = true;
                        that._cache.add(tile);
                        that._rootTiles.push(tile);
                    }
                }
                else{
                    //老版本SCP
                    let fileNameNodes = XmlParser.queryNodes(osgFilesNode, 'FileName', namespace);
                    for(let i = 0,j = fileNameNodes.length;i < j;i++){
                        let fileName = fileNameNodes[i].textContent;
                        fileName = fileName.replace(reg, '/');
                        fileName = fileName.replace(/(\.osgb)/gi,'.s3m');
                        let boundingVolume = {
                            sphere : {
                                center : {
                                    x : 0,
                                    y : 0,
                                    z : 0
                                },
                                radius : 6378137
                            }
                        };
                        let tile = new S3MTile(that, undefined, boundingVolume, fileName);
                        tile.isRootTile = true;
                        that._cache.add(tile);
                        that._rootTiles.push(tile);
                    }
                }
            }
            that._readyPromise.resolve(that);
        },(error)=>{
            that._readyPromise.reject(error);
        })
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

function processTiles(layer, frameState) {
    let tiles = layer._processTiles;
    let length = tiles.length;
    for (let i = 0; i < length; ++i) {
        let tile = tiles[i];
        tile.transformResource(frameState, layer);
    }
}

function updateTiles(layer, frameState) {
    for(let i = 0,j = layer._renderQueue.length;i < j;i++){
        const renderEntity = layer._renderQueue[i];
        renderEntity.update(frameState, layer);
    }
}

function unloadTile(layer, tile) {
    tile.free();
}

function freeResource(layer) {
    layer._cache.unloadTiles(layer, unloadTile);
}

function updateObjsOperationCallback(renderEntity, options){
    renderEntity.updateObjsOperation(options.ids, options);
}

S3MTilesLayer.prototype._tranverseRenderEntity = function(options, callback){
    let stack = [];
    for(let i = 0,j = this._rootTiles.length;i < j;i++){
        let rootTile = this._rootTiles[i];
        stack.push(rootTile);
    }

    while(stack.length) {
        let tile = stack.pop();

        for(let i = 0, j = tile.pageLods.length;i < j;i++){
            const pageLod = tile.pageLods[i];
            const renderEntities = pageLod.renderEntities;
            for(let m = 0,n = renderEntities.length;m < n;m++){
                const ro = renderEntities[m];
                if(ro.ready){
                    callback(ro, options);
                }
            }

            if(pageLod.childTile){
                stack.push(pageLod.childTile);
            }
        }

    }
};

S3MTilesLayer.prototype._updateObjsOperation = function(ids){
    this._tranverseRenderEntity({
        ids : ids
    }, updateObjsOperationCallback);
};

S3MTilesLayer.prototype._setObjsOperationType = function(ids, operationType){
    Cesium.Check.defined('set Objs Operation ids', ids);
    Cesium.Check.defined('set Objs Operation operationType', operationType);
    if(!Array.isArray(ids)){
        ids = [ids];
    }

    let tmpArr = new Cesium.AssociativeArray();

    let id;
    for(let i = 0,j = ids.length;i < j;i++){
        id = ids[i];
        if(!Cesium.defined(id)){
            continue ;
        }
        let operation = Cesium.defaultValue(this._objsOperationList[id], 0);
        if(operation === operationType) {
            continue ;
        }

        operation = operation | operationType;
        this._objsOperationList[id] = operation;
        tmpArr.set(id, operation);
    }

    if(tmpArr.length > 0){
        this._updateObjsOperation(tmpArr._hash);
    }
};

S3MTilesLayer.prototype._removeObjsOperationType = function (ids, operationType) {
    Cesium.Check.defined('set Objs Operation ids', ids);
    if(!Array.isArray(ids)){
        ids = [ids];
    }

    let nonOperationType = OperationType.ALL ^ operationType;
    let tmpArr = new Cesium.AssociativeArray();

    let id;
    for(let i = 0,j = ids.length;i < j;i++){
        id = ids[i];
        let operation = this._objsOperationList[id];
        if(!Cesium.defined(operation)){
            continue ;
        }

        operation &= nonOperationType;
        if(operation === OperationType.RESET){
            delete this._objsOperationList[id];
        }
        else{
            this._objsOperationList[id] = operation;
        }

        tmpArr.set(id, operation);
    }

    if(tmpArr.length > 0){
        this._updateObjsOperation(tmpArr._hash);
    }
};

S3MTilesLayer.prototype.setSelection = function(ids) {
    Cesium.Check.defined('setSelection ids', ids);

    if(!Array.isArray(ids)){
        ids = [ids];
    }

    this.releaseSelection();

    this._selections = this._selections.concat(ids);
    this._setObjsOperationType(ids, OperationType.SELECTED);
};

S3MTilesLayer.prototype.releaseSelection = function(){
    if(this._selections.length < 1){
        return ;
    }

    this._removeObjsOperationType(this._selections, OperationType.SELECTED);
    this._selections.length = 0;
};

S3MTilesLayer.prototype.setLodRangeScale = function(value) {
    this._lodRangeScale = value;
};


S3MTilesLayer.prototype.setVisibleInViewport = function(index, visible){
    if(index > 8){
        throw new Cesium.DeveloperError('the index is 0~8');
    }

    if(visible){
        this._visibleViewport = (1 << index) | this._visibleViewport;
    }
    else{
        this._visibleViewport = (~(1 << index)) & this._visibleViewport;
    }
};

S3MTilesLayer.prototype.getVisibleInViewport = function(index){
    if(index > 8){
        throw new Cesium.DeveloperError('the index is 0~3');
    }

    return (1 << index) & this._visibleViewport;
};

S3MTilesLayer.prototype._updateEdgeDistanceFalloffFactor = function(){
    if(this._edgeCurrentCount === 0){
        this._edgeCurrentTotalLength = 0;
        return;
    }

    this._edgeDistanceFalloffFactor = this._edgeCurrentTotalLength / this._edgeCurrentCount * 40;
    this._edgeCurrentTotalLength = 0;
    this._edgeCurrentCount = 0;
};

S3MTilesLayer.prototype._addRenderedEdge = function(edgeTotalLength, edgeCount){
    this._edgeCurrentTotalLength += edgeTotalLength;
    this._edgeCurrentCount += edgeCount;
};

function updateWaterPlane(layer, frameState) {
    if(!layer._waterPlanes){
        return ;
    }

    let watetPlanes = layer._waterPlanes;
    if(frameState.curDis === undefined){
        frameState.curDis = 6378137;
    }

    if(frameState.heightOffset === undefined){
        frameState.heightOffset = 0;
    }

    let cameraPositionWC = frameState.camera.positionWC;

    for(let key in watetPlanes._hash){
        if(watetPlanes._hash.hasOwnProperty(key)){
            let waterPlaneObj = watetPlanes.get(key);
            let boundingVolume = waterPlaneObj.boundingVolume;
            if(frameState.cullingVolume.computeVisibility(boundingVolume) === Cesium.Intersect.OUTSIDE){
                continue;
            }

            let dis = Cesium.Cartesian3.distance(boundingVolume.center, cameraPositionWC) - boundingVolume.radius;
            dis = dis < 0.01 ? 0.01 : dis;
            if(dis < frameState.curDis){
                frameState.curDis = dis;
                frameState.heightOffset = waterPlaneObj.distance;
            }
        }

    }
}

S3MTilesLayer.prototype.addWaterPlane = function(options) {
    let index = this._waterPlanes.length;
    this._waterPlanes.set(index, options);
    return index;
};

S3MTilesLayer.prototype.removeWaterPlane = function(index){
    this._waterPlanes.remove(index);
};

S3MTilesLayer.prototype.prePassesUpdate = function(frameState) {
    if (!this.ready) {
        return;
    }

    if (frameState.newFrame) {
        this._cache.reset();
        this._requestTiles.length = 0;
        this._processTiles.length = 0;
        this._renderQueue.length = 0;
        if(Cesium.defined(frameState.brdfLutGenerator)){
            frameState.brdfLutGenerator.update(frameState);
        }
        
        updateWaterPlane(this, frameState);
    }
};

let scratchtTransposeViewMatrix = new Cesium.Matrix4();
S3MTilesLayer.prototype.postPassesUpdate = function (frameState) {
    if (!this.ready) {
        return;
    }

    freeResource(this);
    this._updateEdgeDistanceFalloffFactor();
    if(this._subTextureManager){
        this._subTextureManager.processRequests(this.id);
    }
};

function isLayerVisible(layer, frameState) {
    let camera = frameState.camera;
    if(camera.positionCartographic.height < layer._minVisibleAltitude || camera.positionCartographic.height > layer._maxVisibleAltitude){
        return false;
    }

    if(!layer.getVisibleInViewport(frameState.multiViewportIndex)){
        return false;
    }

    return layer.visible;
}



/**
 * 根据插入点和底部高程计算模型矩阵
 * @param layer S3M图层
 */
function updateMatModel(layer) {
    var bottomAltitude = 0;
    if(Cesium.defined(layer.style3D)) {
        bottomAltitude = layer.style3D.bottomAltitude;
    }
    var height = layer.height + bottomAltitude;
    var lon = layer.lon;
    var lat = layer.lat;
    if(layer._sceneMode === Cesium.SceneMode.SCENE3D){
        layer._position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
        layer.modelMatrix = Cesium.Transforms.eastNorthUpToFixedFrame(layer._position);
        layer.invModelMatrix = Cesium.Matrix4.inverse(layer.modelMatrix, layer.invModelMatrix);
    }
    else{
        if ((lon > 180 || lon < -180 || lat > 180 || lat < -180)) {
            let posLon = lon;
            let posLat = lat;
            layer._position = new Cesium.Cartesian3(posLon, posLat, height);
        } else {
            let projection = new Cesium.GeographicProjection();
            let position = Cesium.Cartesian3.fromDegrees(lon, lat, height);
            let cartographic = projection.ellipsoid.cartesianToCartographic(position);
            layer._position = projection.project(cartographic);
        }
        Cesium.Matrix4.fromTranslation(layer._position, layer.modelMatrix);//得到平移矩阵
        Cesium.Matrix4.multiply(transform_2d, layer.modelMatrix, layer.modelMatrix);//得到交换平移矩阵
        layer.invModelMatrix = Cesium.Matrix4.inverse(layer.modelMatrix, layer.invModelMatrix);
    }
}


S3MTilesLayer.prototype.update = function(frameState) {
    if(!this.ready || !isLayerVisible(this, frameState) || (frameState.passes.pick && !this._selectEnabled)) {
        return ;
    }

    if(!CRNTranscoder.wasmReady) {
        CRNTranscoder.init();
        return ;
    }

    this._schuduler.schedule(this, frameState);
    requestTiles(this);
    processTiles(this, frameState);
    updateTiles(this, frameState);

    if(this._style3D._dirty) {
        updateMatModel(this);
        this._style3D._dirty = false;
    }
    this._frameState = frameState;
};


S3MTilesLayer.prototype.isDestroyed = function() {
    return false;
};

S3MTilesLayer.prototype.destroy = function(){
    this._cache.reset();
    freeResource(this);
    this._rootTiles.length = 0;
    this._requestTiles.length = 0;
    this._processTiles.length = 0;
    this._renderQueue.length = 0;
    return Cesium.destroyObject(this);
};

Cesium.Scene.prototype.hookPickFunc = Cesium.Scene.prototype.pick;
Cesium.Scene.prototype.pick = function(windowPosition, width, height) {
    let picked = this.hookPickFunc(windowPosition, width, height);
    if (picked) {
        let isS3MTilesLayer = picked.primitive && picked.primitive instanceof S3MTilesLayer;
        if(isS3MTilesLayer){
            picked.primitive.setSelection(picked.id);
        }
    }
    else{
        for(let i = 0,j = this.primitives.length;i < j;i++){
            let primitive = this.primitives.get(i);
            primitive instanceof S3MTilesLayer && primitive.releaseSelection();
        }
    }

    return picked;
};

export default S3MTilesLayer;