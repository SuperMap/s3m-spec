# 属性描述文件存储格式
本页面说明了属性描述文件(attribute.json)的存储形式及具体内容。按照本页面规范生成的属性描述文件示例见[属性描述文件示例](../Examples/example_attribute.json.md)。

属性描述文件用于描述数据(TileTreeSet)中各图层的属性描述信息，包括各图层对象的ID范围及字段信息等内容。

### 属性描述文件各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|layerInfos|Array\<LayerFieldInfo>|TileTreeSet中各图层的属性描述信息集合，用[LayerFieldInfo对象](#layerfieldinfo对象各标签含义)构成的数组表示|


### LayerFieldInfo对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|layerName|String|图层名|
|idRange|IDRange|某一图层对象ID的范围，用[IDRange对象](#idrange对象各标签含义)表示|
|fieldInfos|Array\<FieldInfo>|某一图层字段信息集合，用[FieldInfo对象](#fieldinfo对象各标签含义)构成的数组表示|

### IDrange对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|minID|int32|某一范围内对象的ID最小值|
|maxID|int32|某一范围内对象的ID最大值|

### FieldInfo对象各标签含义

|标签名|类型|描述|
|:---:|:---:|:---|
|name|String|字段名|
|alias|String|字段别名|
|type|String|字段类型<br>取值范围：{‘bool’,‘int16’, ‘uint16’, ‘int32’,‘uint32’, ‘int64’, ‘uint64’, ‘float’, ‘double’, ‘wchar’, ‘text’, ‘date’, ‘time’, ‘timestamp’}<br>其中，text为String类型；date为日期型；time为时间型；timestamp为时间日期型|
|size|int32|字段长度|
|isRequired|bool|是否是必填字段<br>取值范围：{‘true’,‘false’}<br>分别对应：是，否|
