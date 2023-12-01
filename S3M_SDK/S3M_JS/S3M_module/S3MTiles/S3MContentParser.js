import DDSTexture from './DDSTexture.js';
import MaterialPass from './MaterialPass.js';
import S3MContentFactory from './Factory/S3MContentFactory.js';
import VertexCompressOption from './Enum/VertexCompressOption.js';

function S3MContentParser(){

}

function parseMaterial(context, content, tile) {
    let materialTable = {};
    let materials = Cesium.defaultValue(content.materials.material, content.materials.materials);
    for(let i = 0,j = materials.length;i < j;i++){
        let material = materials[i].material;
        let materialCode = Cesium.defaultValue(material.id, material.name);
        let materialPass = new MaterialPass();
        materialTable[materialCode] = materialPass;
        let ambient = Cesium.defaultValue(material.ambient, Cesium.Color.WHITE);
        materialPass.ambientColor = new Cesium.Color(ambient.r, ambient.g, ambient.b, ambient.a);
        let diffuse = Cesium.defaultValue(material.diffuse, Cesium.Color.WHITE);
        materialPass.diffuseColor = new Cesium.Color(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
        let specular = Cesium.defaultValue(material.specular, Cesium.Color.WHITE);
        materialPass.specularColor = new Cesium.Color(specular.r, specular.g, specular.b, specular.a);
        materialPass.shininess = material.shininess;
        materialPass.bTransparentSorting = material.transparentsorting;
        materialPass.alphaMode = material.alphaMode;
        let textureStates = Cesium.defaultValue(material.textureunitstates, material.textureStates);
        let len = textureStates.length;
        for(let k = 0;k < len;k++){
            let textureState = Cesium.defaultValue(textureStates[k].textureunitstate, textureStates[k].textureUnitState)
            let textureCode = Cesium.defaultValue(textureState.id, textureState.textureName);
            let uAddressMode = Cesium.defaultValue(textureState.addressmode, textureState.uAddressMode);
            let wrapS = uAddressMode.u === 0 ? Cesium.TextureWrap.REPEAT : Cesium.TextureWrap.CLAMP_TO_EDGE;
            let wrapT = uAddressMode.v === 0 ? Cesium.TextureWrap.REPEAT : Cesium.TextureWrap.CLAMP_TO_EDGE;
            materialPass.texMatrix = Cesium.Matrix4.unpack(Cesium.defaultValue(textureState.texmodmatrix,textureState.matrix)); 
            let textureInfo = content.texturePackage[textureCode];
            if(Cesium.defined(textureInfo) && textureInfo.arrayBufferView.byteLength > 0) {
                textureInfo.wrapS = wrapS;
                textureInfo.wrapT = wrapT;
                let keyword = tile.fileName + textureCode;
                let texture = context.textureCache.getTexture(keyword);
                if(!Cesium.defined(texture)){
                    if(Cesium.PixelFormat.isCompressedFormat(textureInfo.internalFormat)){
                        texture = new DDSTexture(context, textureCode, textureInfo);
                    }
                    else{
                        let isPowerOfTwo = Cesium.Math.isPowerOfTwo(textureInfo.width) && Cesium.Math.isPowerOfTwo(textureInfo.height);
                        texture = new Cesium.Texture({
                            context : context,
                            source : {
                                width : textureInfo.width,
                                height : textureInfo.height,
                                arrayBufferView : textureInfo.arrayBufferView
                            },
                            sampler : new Cesium.Sampler({
                                minificationFilter : isPowerOfTwo ? context._gl.LINEAR_MIPMAP_LINEAR : context._gl.LINEAR,
                                wrapS : wrapS,
                                wrapT : wrapT
                            })
                        });

                        if(isPowerOfTwo){
                            texture.generateMipmap(Cesium.MipmapHint.NICEST);
                        }
                    }

                    context.textureCache.addTexture(keyword, texture);
                }

                materialPass.textures.push(texture);
            }
        }
    }

    return materialTable;
}

function calcBoundingVolumeForNormal(vertexPackage, modelMatrix){
    let boundingSphere = new Cesium.BoundingSphere();
    let v1 = new Cesium.Cartesian3();
    let positionAttr = vertexPackage.vertexAttributes[0];
    let dim = positionAttr.componentsPerAttribute;
    let isCompress = Cesium.defined(vertexPackage.compressOptions) && (vertexPackage.compressOptions & VertexCompressOption.SVC_Vertex) === VertexCompressOption.SVC_Vertex;
    let normConstant = 1.0;
    let minVertex;
    let vertexTypedArray;
    if(isCompress){
        normConstant = vertexPackage.vertCompressConstant;
        minVertex = new Cesium.Cartesian3(vertexPackage.minVerticesValue.x, vertexPackage.minVerticesValue.y, vertexPackage.minVerticesValue.z);
        vertexTypedArray = new Uint16Array(positionAttr.typedArray.buffer, positionAttr.typedArray.byteOffset, positionAttr.typedArray.byteLength / 2);
    }
    else{
        vertexTypedArray = new Float32Array(positionAttr.typedArray.buffer, positionAttr.typedArray.byteOffset, positionAttr.typedArray.byteLength / 4);
    }

    let vertexArray = [];
    for(let t = 0; t < vertexPackage.verticesCount; t++){
        Cesium.Cartesian3.fromArray(vertexTypedArray, dim * t, v1);
        if(isCompress){
            v1 = Cesium.Cartesian3.multiplyByScalar(v1, normConstant, v1);
            v1 = Cesium.Cartesian3.add(v1, minVertex, v1);
        }
        vertexArray.push(Cesium.Cartesian3.clone(v1));
    }

    Cesium.BoundingSphere.fromPoints(vertexArray, boundingSphere);
    Cesium.BoundingSphere.transform(boundingSphere, modelMatrix, boundingSphere);
    vertexArray.length = 0;
    return boundingSphere;
    
}

let scratchCenter = new Cesium.Cartesian3();
function calcBoundingVolumeForInstance(vertexPackage){
    let boundingSphere = new Cesium.BoundingSphere();
    let boundingsValues = vertexPackage.instanceBounds;
    if(!Cesium.defined(boundingsValues)){
        return boundingSphere;
    }
    let pntLU = new Cesium.Cartesian3(boundingsValues[0], boundingsValues[1], boundingsValues[2]);
    let pntRD = new Cesium.Cartesian3(boundingsValues[3], boundingsValues[4], boundingsValues[5]);
    let center = new Cesium.Cartesian3.lerp(pntLU, pntRD, 0.5, scratchCenter);
    let radius = new Cesium.Cartesian3.distance(center, pntLU);
    boundingSphere.center = center;
    boundingSphere.radius = radius;
    return boundingSphere;
}

function calcBoundingVolume(vertexPackage, modelMatrix) {
    if(vertexPackage.instanceIndex > -1){
        return calcBoundingVolumeForInstance(vertexPackage);
    }

    return calcBoundingVolumeForNormal(vertexPackage, modelMatrix);
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

function parseGeodes(layer, content, materialTable, pagelodNode, pagelod) {
    let geodeList = pagelodNode.geodes;
    for(let i = 0,j = geodeList.length;i < j;i++){
        let geodeNode = geodeList[i];
        let geoMatrix = geodeNode.matrix;
        let modelMatrix = Cesium.Matrix4.multiply(layer.modelMatrix, geoMatrix, new Cesium.Matrix4());
        let boundingSphere;
        if(Cesium.defined(pagelod.boundingVolume)) {
            if(Cesium.defined(pagelod.boundingVolume.obb)){
                boundingSphere = createBoundingBox(pagelod.boundingVolume.obb, layer.modelMatrix)._boundingSphere;
            }else{
                boundingSphere = new Cesium.BoundingSphere(pagelod.boundingVolume.sphere.center, pagelod.boundingVolume.sphere.radius);
                Cesium.BoundingSphere.transform(boundingSphere, layer.modelMatrix, boundingSphere);
            }
        }
        
        let skeletonNames = geodeNode.skeletonNames;
        for(let m = 0,n = skeletonNames.length;m < n; m++){
            let geoName = skeletonNames[m];
            let geoPackage = content.geoPackage[geoName];
            let vertexPackage = geoPackage.vertexPackage;
            let arrIndexPackage = geoPackage.arrIndexPackage;
            let pickInfo = geoPackage.pickInfo;
            let material;
            if(arrIndexPackage.length > 0) {
                material = materialTable[arrIndexPackage[0].materialCode];
            }
            let geodeBoundingVolume = Cesium.defined(boundingSphere) ? boundingSphere : calcBoundingVolume(vertexPackage, modelMatrix);
            const renderEntity = S3MContentFactory[layer.fileType]({
                layer : layer,
                vertexPackage : vertexPackage,
                arrIndexPackage : arrIndexPackage,
                pickInfo : pickInfo,
                modelMatrix : modelMatrix,
                geoMatrix : geoMatrix,
                boundingVolume : geodeBoundingVolume,
                material : material,
                edgeGeometry : geoPackage.edgeGeometry,
                geoName : geoName
            });
            pagelod.renderEntities.push(renderEntity);
        }
    }

    if(!Cesium.defined(pagelod.boundingVolume) && (pagelod.renderEntities.length > 0)) {
        let arr = [];
        for(let i = 0,j = pagelod.renderEntities.length;i < j;i++){
            arr.push(pagelod.renderEntities[i].boundingVolume);
        }

        pagelod.boundingVolume = {
            sphere : Cesium.BoundingSphere.fromBoundingSpheres(arr),
            isCalc : true//3.0以下版本的叶子节点的包围球须重新计算
        };
    }
}

function parsePagelods(layer, content, materialTable) {
    let groupNode = content.groupNode;
    let pagelods = [];
    for(let i = 0, j = groupNode.pageLods.length;i < j; i++){
        let pagelod = {};
        let pagelodNode = groupNode.pageLods[i];
        pagelod.rangeMode = pagelodNode.rangeMode;
        pagelod.childTile = pagelodNode.childTile;
        pagelod.rangeList = pagelodNode.rangeList;
        pagelod.isLeafTile = pagelod.childTile === '';
        pagelod.renderEntities = [];
        if(pagelodNode.boundingSphere && (!pagelod.isLeafTile || layer.dataVersion > 2)){
            let center = pagelodNode.boundingSphere.center;
            let radius = pagelodNode.boundingSphere.radius;
            pagelod.boundingVolume = {
                sphere : {
                    center : new Cesium.Cartesian3(center.x, center.y, center.z),
                    radius : radius
                },
                isCalc : false //包围球是否是重新计算的
            };
        }

        if(pagelodNode.obb){
            pagelod.boundingVolume = pagelod.boundingVolume || {};
            pagelod.boundingVolume.obb = pagelodNode.obb;
        }

        parseGeodes(layer, content, materialTable, pagelodNode, pagelod);
        pagelods.push(pagelod);
    }

    return pagelods;
}

S3MContentParser.parse = function(layer, content, tile) {
    if(!Cesium.defined(content)) {
        return ;
    }

    layer.dataVersion = content.version;
    let materialTable =  parseMaterial(layer.context, content, tile);
    let pagelods =  parsePagelods(layer, content, materialTable);

    return pagelods;
};

export default S3MContentParser;
