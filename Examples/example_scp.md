# [描述文件](../Specification/scp.json.md)示例
以某一BIM数据的描述文件为例，该描述文件包含了两个TileTree，名称分别为Tile_-7281_21185_0000和Tile_-7282_21183_0000，具体内容如下：

```json
{
	"asset": "",
	"version": "3.01",
	"dataType": "BIM",
	"pyramidSplitType": "QuadTree",       // 四叉树剖分
	"lodType": "Replace",        	      // LOD为替换模式    
	"geoBounds": {                   	  // 地理空间范围
		"left": 116.36350025,
		"top": 40.00181201,
		"right": 116.37550341,
		"bottom": 39.99320925
	},
	"heightRange": {                     // 高度范围
		"min": 9.48750158,
		"max": 119.96122596
	},
	"vertexAttributeDescript":[     	// 顶点扩展属性的含义描述信息和数组范围
		{
			"category": "",
			"range": {
				"dimension": 1,
				"min": [
					0.0
				],
				"max": [
					0.0
				]
			}
		}
	],
	"position": {                	  // 插入点，单位为度
		"point3D":{
			"x": 116.36254801,
			"y": 39.99005683,
			"z": 0.0
		},
		"unit": "Degree"
	},
	"crs": "epsg:4490",   	        // 坐标系统采用2000国家大地坐标系    
	"rootTiles": [               	// 各TileTree对应的根节点文件路径及包围盒
		{                           // 第一个根节点的信息
			"url": "./Tile_-7281_21185_0000/Tile_-7281_21185_0000.s3mb",
			"boundingBox": {
				"center": {
					"x": 344.27652261575619,
					"y": -435.8184572990872,
					"z": 64.24122925985044
				},
				"xExtent": {
					"x": 98.91085097278456,
					"y": 0,
					"z": 0
				},
				"yExtent": {
					"x": 0,
					"y": 49.4554254863923,
					"z": 0
				},
				"zExtent": {
					"x": 0,
					"y": 0,
					"z": 98.91085097278457
				}
			}
		},
		{                         // 第二个根节点的信息
			"url": "./Tile_-7282_21183_0000/Tile_-7282_21183_0000.s3mb",
			"boundingBox": {
				"center": {
					"x": -375.6826015377789,
					"y": 320.81098183130089,
					"z": 38.44527131863524
				},
				"xExtent": {
					"x": 62.03345700298257,
					"y": 0,
					"z": 0
				},
				"yExtent": {
					"x": 0,
					"y": 79.00295032321977,
					"z": 0
				},
				"zExtent": {
					"x": 0,
					"y": 0,
					"z": 58.00596482136575
				}
			}
		}
	],
	"idFieldName": "SmID",
	"compressedPackageType": "Zip",
	"extensions": {}
}
```
