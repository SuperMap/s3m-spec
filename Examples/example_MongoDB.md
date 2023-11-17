# MongoDB存储示例

MongoDB是一种文档型分布式数据库，是非关系型数据库的典型代表，适合存储海量空间三维模型瓦片数据。

MongoDB的单个数据库(database)可以存储多份空间三维模型瓦片数据，每个database中有两类集合（collection）：[元数据集合](#元数据集合)和[数据集合](#数据集合)。
元数据集合存储描述文件相关信息；数据集合存储数据文件、索引树文件、属性描述文件和属性数据文件。

具体存储示例见[数据示例](#数据示例)。


## 元数据集合
元数据集合名称固定为: metadatas_s3m，为方便后续扩展，文档结构定义为自描述方式。元数据集合存储单个database中所有空间三维模型瓦片数据的描述文件信息，ID字段作为每份空间三维模型瓦片数据的标识。

### metadatas_s3m集合的字段信息

|字段名|字段类型|描述|
|:---:|:---:|:---|
|ID|Integer|一份空间三维模型瓦片数据的唯一标识|
|Name|String|元数据标签名称|
|Value|Variant|元数据标签值|

### metadatas_s3m集合Name字段取值及含义

|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|TilesetName|String|空间三维模型瓦片数据的名称|无，在database中唯一，即不同空间三维模型瓦片数据在同一database中不能重名。其取值决定了对应数据集合的名称|
|Version|String|版本号|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的version标签|
|Asset|String|数据的基本信息|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的asset标签|
|PyramidSplitType|String|数据的空间划分类型|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的pyramidSplitType标签|
|LodType|String|LOD切换到精细层时，精细层Tile数据的处理方式|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的lodType标签|
|Position|Position|整个TileTreeSet放置的空间点坐标位置，用[Position对象](#position对象各标签含义)表示|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的position标签|
|GeoBounds|Rect|数据的地理范围，用[Rect对象](#rect对象各标签含义)表示|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的geoBounds标签|
|HeightRange|HeightRange|数据的高度范围,用[HeightRange对象](#heightrange对象各标签含义)表示|对应表5中规定的heightRange标签|
|VertexAttributeDescript|Array\<CategoryDescript>|顶点扩展属性含义描述信息和数值范围，用[CategoryDescript对象](#categorydescript对象各标签含)数组表示|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的vertexAttributeDescript标签|
|Crs|String|坐标系信息|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的crs标签|
|TileInfo|TileInfo|瓦片信息，用[TileInfo对象](#tileinfo对象各标签含义)表示|无|
|IDFieldName|String| IDInfo中单体对象ID的属性字段名,对应属性描述文件（attribute.json）中FieldInfo里某个字段名|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的idFieldName标签|
|CompressedPackageType|String|数据文件（.s3mb）的压缩类型。<br>取值范围：{‘None’,‘Zip’,‘Gzip’}<br>分别对应：无压缩、zip压缩、gzip压缩|对应[Config对象](../Specification/scp.json.md#config对象各标签含义)中规定的compressedPackageType标签|

### Position对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|unit|String|空间坐标的单位|对应[Position对象](../Specification/scp.json.md#position对象各标签含义)中规定的unit标签|
|point3D|Point3D|空间点坐标值，用[Point3D对象](#point3d对象各标签含义)表示|对应[Position对象](../Specification/scp.json.md#position对象各标签含义)中规定的point3D标签|

### Rect对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|left|Double|数据地理范围的左值|对应[Rect对象](../Specification/scp.json.md#rect对象各标签含义)中规定的left标签|
|top|Double|数据地理范围的上值|对应[Rect对象](../Specification/scp.json.md#rect对象各标签含义)中规定的top标签|
|right|Double|数据地理范围的右值|对应[Rect对象](../Specification/scp.json.md#rect对象各标签含义)中规定的right标签|
|bottom|Double|数据地理范围的下值|对应[Rect对象](../Specification/scp.json.md#rect对象各标签含义)中规定的bottom标签|

### HeightRange对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|min|Double|最小值|对应[HeightRange对象](../Specification/scp.json.md#heightrange对象各标签含义)中规定的min标签|
|max|Double|最大值|对应[HeightRange对象](../Specification/scp.json.md#heightrange对象各标签含义)中规定的max标签|

### CategoryDescript对象各标签含
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|category|String|顶点扩展属性含义描述信息|对应[CategoryDescript对象](../Specification/scp.json.md#categorydescript对象各标签含义)中规定的category标签|
|range|Range|顶点扩展属性数值范围，用[Range对象](#range对象各标签含义)表示|对应[CategoryDescript对象](../Specification/scp.json.md#categorydescript对象各标签含义)中规定的range标签|

### Range对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|dimension|Integer|数据维度|对应[Range对象](../Specification/scp.json.md#range对象各标签含义)中规定的dimension标签|
|min|Array\<Double>|每个维度的最小值构成的数组，数组长度为dimension|对应[Range对象](../Specification/scp.json.md#range对象各标签含义)中规定的min标签|
|max|Array\<Double>|每个维度最大值构成的数组，数组长度为dimension|对应[Range对象](../Specification/scp.json.md#range对象各标签含义)中规定的max标签|

### TileInfo对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|tileName|String|瓦片名称|无|
|boundingBox|BoundingBox|瓦片数据范围，用[BoundingBox对象](#boundingbox对象各标签含义)表示|对应[TileTreeInfo对象](../Specification/scp.json.md#tiletreeinfo对象各标签含义)中规定的boundingBox标签|

### Point3D对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|x|Double|空间点的x坐标值|对应[Point3D对象](../Specification/scp.json.md#point3d对象各标签含义)中规定的x标签|
|y|Double|空间点的y坐标值|对应[Point3D对象](../Specification/scp.json.md#point3d对象各标签含义)中规定的y标签|
|z|Double|空间点的z坐标值|对应[Point3D对象](../Specification/scp.json.md#point3d对象各标签含义)中规定的z标签|

### BoundingBox对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|center|Point3D|包围盒中心点，用[Point3D对象](#point3d对象各标签含义)表示|对应[BoundingBox对象](../Specification/scp.json.md#boundingbox对象各标签含义)中规定的center标签|
|xExtent|Vector3|包围盒x方向和长度，用[Vector3对象](#vector3对象各标签含义)表示|对应[BoundingBox对象](../Specification/scp.json.md#boundingbox对象各标签含义)中规定的xExtent标签|
|yExtent|Vector3|包围盒y方向和长度，用[Vector3对象](#vector3对象各标签含义)表示|对应[BoundingBox对象](../Specification/scp.json.md#boundingbox对象各标签含义)中规定的yExtent标签|
|zExtent|Vector3|包围盒z方向和长度，用[Vector3对象](#vector3对象各标签含义)表示|对应[BoundingBox对象](../Specification/scp.json.md#boundingbox对象各标签含义)中规定的zExtent标签|

### Vector3对象各标签含义
|标签名|字段类型|描述|与描述文件各标签的对应关系|
|:---:|:---:|:---|:---|
|x|Double|向量的x分量|对应[Vector3对象](../Specification/S3MB/Material.md#vector3对象各标签含义)中规定的x |
|y|Double|向量的y分量|对应[Vector3对象](../Specification/S3MB/Material.md#vector3对象各标签含义)中规定的x |
|z|Double|向量的z分量|对应[Vector3对象](../Specification/S3MB/Material.md#vector3对象各标签含义)中规定的x |

## 数据集合
数据集合的命名规则为Tileset_Name，其中，Name是每份空间三维模型瓦片数据的名称，对应metadatas_s3m集合的TilesetName标签的值。如某空间三维模型瓦片数据的名称为cbd，其数据集合名称为Tileset_cbd。

一个数据集合存储一份空间三维模型瓦片数据的数据文件、索引树文件、属性描述文件、属性数据文件。

### 数据集合的字段信息
|字段名|字段类型|描述|
|:---:|:---:|:---|
|Tileset_Key|String|文件名称（含文件扩展名）。<br>不同文件的文件扩展名不同，数据文件、索引树文件、属性数据文件的文件扩展名分别为s3mb、json、s3md。<br>属性描述文件的文件名称为attribute.json|
|Tileset_Value|Binary Data|文件的二进制数据|

## 数据示例
以某一BIM数据为例给出如下示例。

### 元数据集合示例
部分Building@CBD数据的元数据具体内容如下（_id字段为MongoDB预定义主键）：

```
/* TilesetName */
{
    "_id" : ObjectId("5576849543e29094b23538d0"),
    "ID" : 1,
    "Name" : "TilesetName",
    "Value" : "Building@CBD"
}

/* Version */
{
    "_id" : ObjectId("5576849543e29094b23538d1"),
    "ID" : 1,
    "Name" : "Version",
    "Value" : "3.0"
}

/* Asset */
{
    "_id" : ObjectId("5576849543e29094b23538d2"),
    "ID" : 1,
    "Name" : "Asset",
    "Value" : ""
}

/* DataType */
{
    "_id" : ObjectId("5576849543e29094b23538d3"),
    "ID" : 1,
    "Name" : "DataType",
    "Value" : "BIM"
}

/* PyramidSplitType */
{
    "_id" : ObjectId("5576849543e29094b23538d4"),
    "ID" : 1,
    "Name" : "PyramidSplitType",
    "Value" : "QuadTree"
}

/* LodType */
{
    "_id" : ObjectId("5576849543e29094b23538d5"),
    "ID" : 1,
    "Name" : "LodType",
    "Value" : "Replace"
}

/* Position */
{
    "_id" : ObjectId("5576849543e29094b23538d6"),
    "ID" : 1,
    "Name" : "Position",
    "Value" : {
    "unit" : "Degree",
    "point3D" : {
        "x" : 116.36,
        "y" : 39.99,
        "z" : 0
        }
    }
}

/* GeoBounds */
{
    "_id" : ObjectId("5576849543e29094b23538d7"),
    "ID" : 1,
    "Name" : "GeoBounds",
    "Value" : {
        "left" : 116.3635,
        "top" : 40.0018,
        "right" : 116.3755,
        "bottom" : 39.9932
    }
}

/* HeightRange */
{
    "_id" : ObjectId("5576849543e29094b23538d8"),
    "ID" : 1,
    "Name" : "HeightRange",
    "Value" : {
        "min" : 9.4875,
        "max" : 119.9612
    }
}

/* VertexAttributeDescript */
{
    "_id" : ObjectId("5576849543e29094b23538d9"),
    "ID" : 1,
    "Name" : "VertexAttributeDescript",
    "Value" : [{
        "category" : "",
        "range" : {
            "dimension" : 1,
            "min" : [0],
            "max" : [0]
        }
    }]
}

/* Crs */
{
    "_id" : ObjectId("5576849543e29094b23538da"),
    "ID" : 1,
    "Name" : "Crs",
    "Value" : "epsg: 4490"
}

/* TileInfo */
{
    "_id" : ObjectId("5576849543e29094b23538db"),
    "ID" : 1,
    "Name" : "TileInfo",
    "Value" : {
        "tileName" : "Tile_-7281_21185_0000.s3mb",
        "boundingBox" : {
            "center" : {
              "x" : 344.276527615756145,
              "y" : -435.8184572990872,
              "z" : 64.24122925985044
            },
             "xExtent" : {
                "x" : 98.91085097278456,
                "y" : 0,
                "z" : 0
            },
            "yExtent" : {
                "x" : 0,
                "y" : 49.4554254863923,
                "z" : 0
            },
            "zExtent" : {
                "x" : 0,
                "y" : 0,
                "z" : 98.91085097278457
            }
        }
    }
}

/* IDFieldName */
{
    "_id" : ObjectId("5576849543e29094b23538dc"),
    "ID" : 1,
    "Name" :  "IDFieldName",
    "Value" : "SmID"
}

/* CompressedPackageType */
{
    "_id" : ObjectId("5576849543e29094b23538dd"),
    "ID" : 1,
    "Name" : "CompressedPackageType",
    "Value" : "Zip"
}
```

### 数据集合示例
部分Tileset_Building@CBD数据的数据集合的具体内容如下（_id字段为MongoDB预定义主键）：

```
/* 索引树文件 */ 
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d648d"),
    "Tileset_Key" : "Tile_-7281_21185_0000.json",
    "Tileset_Value" : new BinData(0, "eyJsb2RU...")
}

/* 数据文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d648b"),
    "Tileset_Key" : "Tile_-7281_21185_0000.s3mb",
    "Tileset_Value" : new BinData(0, "AACAPx8y...")
}

/* 数据文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d649e"),
    "Tileset_Key" : "Tile_-7281_21185_0000_0004_0000.s3mb",
    "Tileset_Value" : new BinData(0, "AACAPxS3...")
}

/* 数据文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d6491"),
    "Tileset_Key" : "Tile_-7282_21183_0000.s3mb",
    "Tileset_Value" : new BinData(0, "AACAP8UK...")
}

/* 数据文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d64a0"),
    "Tileset_Key" : "Tile_-7282_21183_0000_0001_0000.s3mb",
    "Tileset_Value" : new BinData(0, "AACAP4zS...")
}

/* 属性描述文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d64a3"),
    "Tileset_Key" : "attribute.json",
    "Tileset_Value" : new BinData(0, "eyJsYXll...")
}

/* 属性数据文件 */
{
    "_id" : ObjectId("5ffcf3099883b7c2d85d649a"),
    "Tileset_Key" : "Tile_-7281_21185_0000.s3md",
    "Tileset_Value" : new BinData(0, "zgcAAKgB...")
}

```