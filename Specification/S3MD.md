# 属性数据文件存储格式

本页面说明了属性数据文件(*.s3md)的存储形式及具体内容。按照本页面规范生成的属性数据文件示例见[属性数据文件示例](../Examples/example_s3md.md)。

属性数据文件应包含TileTree中所有对象的属性数据，包括每图层的属性描述信息和每个对象的各属性值。

## 属性数据文件各标签含义
|标签名|类型|描述|
|:---:|:---:|:---|
|layerInfos|Array\<LayerAttributeValue>|TileTree中所有对象的属性数据信息，用[LayerAttributeValue对象](#layerattributevalue对象各标签含义)构成的数组表示|

## LayerAttributeValue对象各标签含义
|标签名|类型|描述|
|:---:|:---:|:---|
|idRange|IDRange|对应Tile范围内对象ID的范围，用[IDRange对象](./attribute.json.md#idrange对象各标签含义)表示|
|fieldInfos|Array\<FieldInfo>|对应Tile范围内字段信息集合，用[FieldInfo对象](./attribute.json.md#fieldinfo对象各标签含义)构成的数组表示|
|records|Array<Record>|各对象属性值的记录信息集合，用[Record对象](#record对象各标签含义)构成的数组表示|

## Record对象各标签含义
|标签名|类型|描述|
|:---:|:---:|:---|
|id|int32|对象的ID|
|values|Array\<Value>|各字段的属性值详细描述，用[Value对象](#value对象各标签含义)构成的数组表示|

## Value对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|name|String|字段名|
|value|Variant|字段值|