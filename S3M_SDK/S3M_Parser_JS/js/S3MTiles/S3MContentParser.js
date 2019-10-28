define([
    './XmlParser',
    './DDSTexture',
    './MaterialPass',
    './Factory/S3MContentFactory'
],function(
    XmlParser,
    DDSTexture,
    MaterialPass,
    S3MContentFactory
    ){
    "use strict";

    function S3MContentParser(){

    }

    function parseMaterial(context, doc, content) {
        let groupNode = doc.firstChild;
        let namespace = groupNode.namespaceURI;
        let materialNode = XmlParser.queryFirstNode(groupNode,'Material3Ds',namespace);
        let materialNodes = XmlParser.queryChildNodes(materialNode,'material',namespace);
        let materialTable = {};
        for(let i = 0,j = materialNodes.length;i < j;i++){
            let materialPass = new MaterialPass();
            let materialNode = materialNodes[i];
            let materialCode = XmlParser.queryStringValue(materialNode,'name',namespace);
            materialTable[materialCode] = materialPass;
            let ambient = XmlParser.queryFirstNode(materialNode,'Ambient',namespace);
            let r = Cesium.defaultValue(XmlParser.queryNumericValue(ambient,'AmbientR',namespace), XmlParser.queryNumericValue(ambient,'R',namespace));
            let g = Cesium.defaultValue(XmlParser.queryNumericValue(ambient,'AmbientG',namespace), XmlParser.queryNumericValue(ambient,'G',namespace));
            let b = Cesium.defaultValue(XmlParser.queryNumericValue(ambient,'AmbientB',namespace), XmlParser.queryNumericValue(ambient,'B',namespace));
            let a = Cesium.defaultValue(XmlParser.queryNumericValue(ambient,'AmbientA',namespace), XmlParser.queryNumericValue(ambient,'A',namespace));
            materialPass.ambientColor = new Cesium.Color(r,g,b,a);
            let diffuse = XmlParser.queryFirstNode(materialNode,'Diffuse',namespace);
            r = Cesium.defaultValue(XmlParser.queryNumericValue(diffuse,'DiffuseR',namespace), XmlParser.queryNumericValue(diffuse,'R',namespace));
            g = Cesium.defaultValue(XmlParser.queryNumericValue(diffuse,'DiffuseG',namespace), XmlParser.queryNumericValue(diffuse,'G',namespace));
            b = Cesium.defaultValue(XmlParser.queryNumericValue(diffuse,'DiffuseB',namespace), XmlParser.queryNumericValue(diffuse,'B',namespace));
            a = Cesium.defaultValue(XmlParser.queryNumericValue(diffuse,'DiffuseA',namespace), XmlParser.queryNumericValue(diffuse,'A',namespace));
            materialPass.diffuseColor = new Cesium.Color(r,g,b,a);
            let specular = XmlParser.queryFirstNode(materialNode,'Specular',namespace);
            r = Cesium.defaultValue(XmlParser.queryNumericValue(specular,'SpecularR',namespace), XmlParser.queryNumericValue(specular,'R',namespace));
            g = Cesium.defaultValue(XmlParser.queryNumericValue(specular,'SpecularG',namespace), XmlParser.queryNumericValue(specular,'G',namespace));
            b = Cesium.defaultValue(XmlParser.queryNumericValue(specular,'SpecularB',namespace), XmlParser.queryNumericValue(specular,'B',namespace));
            a = Cesium.defaultValue(XmlParser.queryNumericValue(specular,'SpecularA',namespace), XmlParser.queryNumericValue(specular,'A',namespace));
            materialPass.specularColor = new Cesium.Color(r,g,b,a);
            materialPass.shininess = XmlParser.queryNumericValue(materialNode,'Shininess',namespace);
            materialPass.bTransparentSorting = XmlParser.queryBooleanValue(materialNode,'TransparentSorting',namespace);
            let textureNodes = XmlParser.queryChildNodes(materialNode,'texture',namespace);
            for(let k = 0;k < textureNodes.length;k++){
                let textureNode = textureNodes[k];
                let textureCode = XmlParser.queryStringValue(textureNode,'name',namespace);
                let textureName = XmlParser.queryStringValue(textureNode,'TextureName',namespace);
                let addressModeNode = XmlParser.queryFirstNode(textureNode, 'AddressMode', namespace);
                let wrapS = XmlParser.queryStringValue(addressModeNode, 'u', namespace);
                let wrapT = XmlParser.queryStringValue(addressModeNode, 'v', namespace);
                wrapS = wrapS === 'TAM_WRAP' ? Cesium.TextureWrap.REPEAT : Cesium.TextureWrap.CLAMP_TO_EDGE;
                wrapT = wrapT === 'TAM_WRAP' ? Cesium.TextureWrap.REPEAT : Cesium.TextureWrap.CLAMP_TO_EDGE;
                let texMatrix = XmlParser.queryStringValue(textureNode,'TexModMatrix',namespace).split(',');
                texMatrix = texMatrix.map(function(item) {
                    return parseFloat(item);
                });

                materialPass.texMatrix = Cesium.Matrix4.unpack(texMatrix);
                let textureInfo = content.texturePackage[textureCode];
                textureInfo.wrapS = wrapS;
                textureInfo.wrapT = wrapT;
                if(Cesium.defined(textureInfo) && textureInfo.imageBuffer.byteLength > 0){
                    let texture = new DDSTexture(context, textureCode, textureInfo);
                    materialPass.textures.push(texture);
                }
            }
        }

        return materialTable;
    }

    function parsePagelods(layer, content, doc, materialTable) {
        let groupNode = doc.firstChild;
        let namespace = groupNode.namespaceURI;
        let pagelodsNode = XmlParser.queryFirstNode(groupNode,'PageLods',namespace);
        let pagelodNodes = XmlParser.queryChildNodes(pagelodsNode,'PagedLOD',namespace);
        let pagelods = [];
        for(let i = 0,j = pagelodNodes.length;i < j;i++){
            let pagelod = {};
            let pagelodNode = pagelodNodes[i];
            let rangeDataList = XmlParser.queryStringValue(pagelodNode,'RangeDataList',namespace);
            if(rangeDataList){
                rangeDataList = rangeDataList.replace(/.osgb$/, ".s3m");
            }

            let rangeMode = XmlParser.queryStringValue(pagelodNode,'RangeMode',namespace);
            let rangeList = XmlParser.queryNumericValue(pagelodNode,'RangeList',namespace);
            let bSphereNode = XmlParser.queryFirstNode(pagelodNode,'BoundingSphere',namespace);
            let x = XmlParser.queryNumericValue(bSphereNode,'x',namespace);
            let y = XmlParser.queryNumericValue(bSphereNode,'y',namespace);
            let z = XmlParser.queryNumericValue(bSphereNode,'z',namespace);
            let radius = XmlParser.queryNumericValue(bSphereNode,'radius',namespace);
            pagelod.rangeMode = rangeMode;
            pagelod.rangeDataList = rangeDataList;
            pagelod.rangeList = rangeList;
            pagelod.boundingVolume = {
                sphere : {
                    center : new Cesium.Cartesian3(x, y, z),
                    radius : radius
                }
            };
            pagelods.push(pagelod);
            pagelod.geoMap = parseGeodes(layer, content, doc, materialTable, pagelodNode, pagelod.boundingVolume);
        }

        return pagelods;
    }

    function parseGeodes(layer, content, doc, materialTable, parentNode, boundingVolume) {
        let geoMap = {};
        let groupNode = doc.firstChild;
        let namespace = groupNode.namespaceURI;
        let geodeNodes = XmlParser.queryChildNodes(parentNode, 'Geode', namespace);
        for(let i = 0, j = geodeNodes.length;i < j;i++){
            let geodeNode = geodeNodes[i];
            let geoMatrix = XmlParser.queryStringValue(geodeNode, 'GeoDeModMatrix',namespace).split(',');
            geoMatrix = geoMatrix.map(function(item) {
                return parseFloat(item);
            });

            geoMatrix = Cesium.Matrix4.unpack(geoMatrix);
            let modelMatrix = Cesium.Matrix4.multiply(layer.modelMatrix, geoMatrix, new Cesium.Matrix4());
            let boundingSphere = new Cesium.BoundingSphere(boundingVolume.sphere.center, boundingVolume.sphere.radius);
            Cesium.BoundingSphere.transform(boundingSphere, modelMatrix, boundingSphere);
            let geoNameNodes = XmlParser.queryChildNodes(geodeNode, 'GeoName');
            for(let m = 0, n = geoNameNodes.length;m < n;m++){
                let geoName = geoNameNodes[m].textContent.trim();
                let geoPackage = content.geoPackage[geoName];
                let vertexPackage = geoPackage.vertexPackage;
                let arrIndexPackage = geoPackage.arrIndexPackage;
                let material;
                if(arrIndexPackage.length > 0) {
                    material = materialTable[arrIndexPackage[0].materialCode];
                }
                let pickInfo = vertexPackage.pickInfo;
                geoMap[geoName] = S3MContentFactory[layer.fileType]({
                    layer : layer,
                    vertexPackage : vertexPackage,
                    arrIndexPackage : arrIndexPackage,
                    pickInfo : pickInfo,
                    modelMatrix : modelMatrix,
                    boundingVolume : boundingSphere,
                    material : material
                });
            }
        }

        return geoMap;
    }

    S3MContentParser.parse = function(layer, content) {
        if(!Cesium.defined(content)) {
            return ;
        }

        let doc = XmlParser.read(content.xmlDoc);
        let materialTable = parseMaterial(layer.context, doc, content);
        let pagelods = parsePagelods(layer, content, doc, materialTable);
        //parseGeodes(layer, content, doc, parentNode, undefined);

        return pagelods;

    };

    return S3MContentParser;
});