# [材质内容](../Specification/S3MB/Material.md)示例

根据材质类型，分别提供了[普通材质内容示例](#普通材质内容示例)和[PBR材质内容示例](#pbr材质内容示例)。

## 普通材质内容示例

```json
{
	"materials": [                  // 材质对象描述信息集合
		{
			"material": {           // 第一个材质对象
			    // 材质基本信息
				"name": "0_10710_Sec_0005_-7281_21185_0000_0000_0",// 材质对象的名称
				"type": "common",

				// Common材质基本属性
				"ambient": {
					"r": 1.0,
					"g": 1.0,
					"b": 1.0,
					"a": 1.0
				},
				"diffuse": {
					"r": 1.0,
					"g": 1.0,
					"b": 1.0,
					"a": 1.0
				},
				"specular": {
					"r": 1.0,
					"g": 1.0,
					"b": 1.0,
					"a": 1.0
				},
				"shininess": 0.0,

				// 渲染状态信息
				"cullMode": "CULL_NONE",
				"alphaMode": "AM_OPAQUE",
				"alphaCutoff": 1.0,
				"vertexShaderName": "",
				"fragmentShaderName": "",

				// 材质采用的纹理信息
				"textureStates": [
					{
						"textureUnitState": {
							"textureName": "0_10710_Sec_0005_-7281_21185_0000_0000",  // 纹理对象的名称
							"url": "",   // URL为空，则纹理数据通过textureName与数据文件中存储的纹理数据关联
							"uAddressMode": 0,
							"vAddressMode": 0,
							"wAddressMode": 0,
							"filteringOption": 2,
							"minFilter": 2,
							"maxFilter": 2,
							"matrix": [
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0
							]
						}
					}
				],

				// 扩展信息
				"extensions": {}
			}
		}
	]
}
```


## PBR材质内容示例

```json
{
	"materials": [               // 材质对象描述信息集合
		{
			// 第一个材质对象
			"material": {
				// 材质基本信息
				"name": "0_10710_Sec_0005_-7281_21185_0000_0000_0",// 材质对象的名称
				"type": "PBR",

				// PBR基本属性
				"baseColor": {
					"x": 1.0,
					"y": 1.0,
					"z": 1.0,
					"w": 0.4
				},
				"baseColorTextureIndex": -1,
				"baseColorTextureCoordIndex": -1,
				"baseColorTextureMotion": {
					"offsetStartU": 0.0,
					"offsetStartV": 0.0,
					"offsetSpeedU": 0.0,
					"offsetSpeedV": 0.0,
					"offsetPeriod": 0.0,
					"tilingStartU": 1.0,
					"tilingStartV": 1.0,
					"tilingSpeedU": 1.0,
					"tilingSpeedV": 1.0,
					"tilingPeriod": 0.0
				},
				"metallicFactor": 0.0,
				"roughnessFactor": 1.0,
				"metallicRoughnessTextureIndex": -1,
				"metallicRoughnessTextureCoordIndex": -1,

				// 常用贴图：法线，遮挡，自发光
				"normalTextureIndex": -1,
				"normalTextureCoordIndex": -1,
				"normalTextureScale": 0.0,
				"occlusionTextureIndex": -1,
				"occlusionTextureCoordIndex": -1,
				"occlusionTextureStrength": 0.0,
				"emissiveFactor": {
					"x": 2.118547,
					"y": 2.118547,
					"z": 2.118547
				},
				"emissiveTextureIndex": 0,
				"emissiveTextureCoordIndex": 0,
				"emissiveTextureMotion": {
					"offsetStartU": 0.0,
					"offsetStartV": 0.0,
					"offsetSpeedU": 0.05,
					"offsetSpeedV": 0.0,
					"offsetPeriod": 20.0,
					"tilingStartU": 1.0,
					"tilingStartV": 1.0,
					"tilingSpeedU": 1.0,
					"tilingSpeedV": 1.0,
					"tilingPeriod": 0.0
				},
				"maskTextureIndex": -1,
				"maskTextureCoordIndex": -1,
				"semanticTextureIndex": -1,
				"semanticTextureCoordIndex": -1,

				// 渲染状态信息
				"cullMode": "CULL_NONE",
				"alphaMode": "AM_BLEND",
				"alphaCutoff": 0.5,
				"vertexShaderName": "",
				"fragmentShaderName": "",

				// 材质采用的纹理信息
				"textureStates": [
					{
						"textureUnitState": {
							// 纹理对象的名称 
							"textureName": "0_10710_Sec_0005_-7281_21185_0000_0000",
							// URL为空，则纹理数据通过textureName与数据文件中存储的纹理数据关联
							"url": "",
							"uAddressMode": 0,
							"vAddressMode": 0,
							"wAddressMode": 0,
							"filteringOption": 2,
							"minFilter": 2,
							"maxFilter": 2,
							"matrix": [
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0,
								0.0,
								0.0,
								0.0,
								0.0,
								1.0
							]
						}
					}
				],

				// 扩展信息
				"extensions": {}
			}
		}
	]
}
```