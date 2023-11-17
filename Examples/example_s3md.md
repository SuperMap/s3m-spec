# [属性数据文件](../Specification/S3MD.md)示例

属性相关数据为可选。如果存在属性数据，则除了属性描述文件描述各图层的相关属性信息，在每个TileTree文件夹中，存在与RootTile同名，扩展名为.s3md的属性数据文件，用于存储该TileTree下所有属性数据。以Tile_-7281_21185_0000为例，属性数据为文件夹下Tile_-7281_21185_0000.s3md文件，具体内容如下：

```json
{
	"layerInfos": [                    // 属性数据集合
		{
			"idRange": {               // 对应Tile范围内包含的对象ID范围
				"minID": 1,
				"maxID": 1
			},
			"fieldInfos": [            // 各个字段描述的信息（SmID、MODELNAME）
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
			],
			"records": [                 // 各个字段值集合
				{
					"id": 1,
					"values": [          // ID=1的对象各个字段的值（SmID、MODELNAME）
						{
							"name": "SmID",
							"value": 1
						},
						{
							"name": " ModelName",
							"value ": "国家体育场"
						}
					]
				}
			]
		}
	]
}
```