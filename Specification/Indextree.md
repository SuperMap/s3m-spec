# 索引树文件存储格式
索引树文件应包含TileTree中各LOD层的文件信息。按照本页面规范生成的数据示例见[索引树文件示例](../Examples/example_indextree.md)。

### 索引树文件各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|name|String|Tile的名称|
|tileInfo|TileInfo|Tile的信息，用[TileInfo对象](#tileinfo对象各标签含义)表示|
|status|Status|RootTile的状态，用于描述TileTree的整体信息，用[Status对象](#status对象各标签含义)表示|

### TileInfo对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|lodNum|int32|数据所在LOD层号，自顶向下递增，起始层号为0|
|modelPath|String|数据文件相对于该索引文件的路径|
|indexFilePath|String|子节点索引文件相对于该索引文件的路径|
|rangeMode|String|LOD选取模式<br>取值范围：{‘distanceFromEyePoint’,‘pixelSizeOnScreen’, ‘geometricError’}<br>分别表示：基于瓦片与视点的距离的LOD选取，基于瓦片投影在屏幕上的像素数的LOD选取，基于几何误差模式的LOD选取<br>|
|rangeValue|double|LOD选取阈值因子|
|children|Array\<TileInfo>|各子节点Tile的信息，用TileInfo构成的数组表示|
|state|String|子节点Tile的状态<br>取值范围：{‘AllLeaf’,‘NoLeaf’,‘PartLeaf’}<br>分别表示：全部子节点都是叶子节点，全部子节点都不是叶子节点，部分子节点是叶子节点<br>说明：叶子节点是指没有子节点的节点|
|boundingBox|BoundingBox|包围盒,用[BoundingBox对象](./scp.json.md#boundingbox对象各标签含义)表示|

### Status对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|lodCount|int32|Tile的LOD层级总数|
|tilesCount|int32|Tile总数|