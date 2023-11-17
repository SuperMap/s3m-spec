# 描述文件存储格式
本页面说明了描述文件(scp.json)的存储形式及具体内容。按照本页面规范生成的描述文件示例见[描述文件示例](../Examples/example_scp.md)。

描述文件（Config对象）用于描述数据(TileTreeSet)的基本信息，包括版本号、数据类型、坐标系信息以及根节点信息等内容。Config对象的UML图见下图。

![](../../images/Config.png)


### Config对象各标签含义

|标签名|	类型|	描述|
|:---:|:---:|:---|
|asset|	String|	数据的基本信息，如生产单位等|
|version	|String|	版本号|
|dataType|	String|	地理空间数据的类型。<br>取值范围：{‘ArtificialModel’,‘RealityMesh’, ‘PointCloud’,‘BIM’,‘Vector’,‘Network’，‘InstanceModel’}<br>分别对应：人工模型，倾斜摄影三维模型，点云，BIM，矢量数据，网络数据和外挂模型|
|pyramidSplitType	|String|	数据的空间划分类型<br>取值范围：{‘QuadTree’,‘Octree’,‘RTree’,‘K-DTree’, ‘Grid’,‘BinarySpacePartitioningTree’ ,‘UnorderedTree’}<br>分别对应：四叉树，八叉树，R树，K-D树，网格，BSP树，自由树|
|lodType|	String|	LOD切换到精细层时，精细层Tile数据的处理方式<br>取值范围：{‘Add’,‘Replace’}<br>分别对应：添加到当前层，替换当前层|
|geoBounds|	Rect|	数据的地理范围，用[Rect对象](#rect对象各标签含义)表示,和crs标签的坐标系一致|
|heightRange|HeightRange|数据的高度范围，用[HeightRange对象](#heightrange对象各标签含义)表示|
|vertexAttributeDescript|	Array\<CategoryDescript>|顶点扩展属性的含义描述信息和数值范围，用[CategoryDescript对象](#categorydescript对象各标签含义)构成的数组表示。顶点扩展属性记录用户自定义扩展的顶点属性|
|position|	Position|	整个TileTreeSet放置的空间点坐标位置，用[Position对象](#position对象各标签含义)表示|
|crs	|String	|坐标系信息，可表述任何按照GB/T 30170所描述的坐标参照系类型<br>表示形式：crs:{‘type:content’}，其中crs为关键字，type可以是epsg或wkt，content是字符串内容<br>epsg形式的表述格式示例：crs:{‘epsg:4490’}<br>wkt形式的表述格式：crs:{‘wkt: wktcontent’}，wktcontent应符合GB/T 33187.1—2016中6.4的规定|
|rootTiles|	Array\<TileTreeInfo>|TileTreeSet中所有RootTile的信息，用[TileTreeInfo对象](#tiletreeinfo对象各标签含义)构成的数组表示。|
|idFieldName	|String	|IDInfo中[单体对象ID](./S3MB/S3MB.md#monomeridinfo对象各属性含义)的属性字段名,对应属性描述文件（attribute.json）中[FieldInfo](./attribute.json.md#fieldinfo对象各标签含义)里某个字段名|
|compressedPackageType	|String|	数据文件（.s3mb）的压缩类型<br>取值范围：{‘None’,‘Zip’,‘Gzip’}<br>分别对应：无压缩、zip压缩、gzip压缩|
|extensions|	String	|用户自定义的扩展信息|

### Rect对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|left|	double|	数据地理范围的左值|
|top|	double	|数据地理范围的上值|
|right|	double|	数据地理范围的右值|
|bottom|	double|	数据地理范围的下值|

### HeightRange对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|min|	double|	最小值|
|max	|double|	最大值|

### CategoryDescript对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|category|	String|	顶点扩展属性的含义描述信息|
|range|	Range|	顶点扩展属性的数值范围，用[Range对象](#range对象各标签含义)表示|

### Range对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|dimension	|uint16	|数据的维度数|
|min|	Array\<double>|	每个维度的最小值构成的数组，数组长度等于dimension|
|max	|Array\<double>|	每个维度的最大值构成的数组，数组长度等于dimension|

### Position对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|point3D|	Point3D|	空间点坐标值，用[Point3D对象](#point3d对象各标签含义)表示，经纬度的表示法应符合GB/T 16831的规定|
|unit|	String|	空间坐标值的单位<br>取值范围：{‘Degree’,‘Meter’}<br>分别对应：度、米|

### Point3D对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|x|	double	|空间点的x坐标值|
|y|	double|	空间点的y坐标值|
|z|	double	|空间点的z坐标值|

### TileTreeInfo对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|url	|String|	RootTile所在路径，可以是相对路径、绝对路径或网络路径。如果采用相对路径，则为相对于描述文件所在位置的路径|
|boundingBox|	BoundingBox	|RootTile数据的空间范围，用[BoundingBox对象](#boundingbox对象各标签含义)表示|

### BoundingBox对象各标签含义
|标签名|	类型	|描述|
|:---:|:---:|:---|
|center	|Point3D	|包围盒中心点，用[Point3D对象](#point3d对象各标签含义)表示|
|xExtent	|Vector3	|包围盒x方向和长度，用[Vector3对象](./S3MB/Material.md#vector3对象各标签含义)表示|
|yExtent	|Vector3	|包围盒y方向和长度，用[Vector3对象](./S3MB/Material.md#vector3对象各标签含义)表示|
|zExtent	|Vector3	|包围盒z方向和长度，用[Vector3对象](./S3MB/Material.md#vector3对象各标签含义)表示|
