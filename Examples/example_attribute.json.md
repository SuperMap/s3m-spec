# [属性描述文件](../Specification/attribute.json.md)示例
属性相关数据为可选。如果存在属性数据，则属性描述文件的名称限定为attribute.json，且与描述文件在同级目录。

本页面以某一BIM数据的属性描述文件为例进行说明，示例数据如下：

``` json
{
	"layerInfos": [
		{
			"layerName": "Building_Sub",   // 对应的原始数据集名称
			"idRange": {                   // TileTreeSet中包含的数据集对象的ID范围
				"minID": 1,
				"maxID": 10
			},
			"fieldInfos": [                // 字段描述信息
				{
					"name": "SmID",
					"alias": "SmID",
					"type": "int32",
					"size": 4,
					"isRequired": true
				},
				{
					"name": "MODELNAME",
					"alias": "ModelName",
					"type": "String",
					"size": 30,
					"isRequired": false
				}
			]
		}
	]
}
```