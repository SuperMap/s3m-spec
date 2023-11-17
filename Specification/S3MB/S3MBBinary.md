# 数据文件的二进制流描述

数据文件流内容如下：
```
S3MBFile{
  float version;                //数据文件版本号
  uint32 compressedType;        //压缩类型。值为0时，表示不压缩；值为1时，表示zip压缩；值为2时，表示gzip压缩
  uint32 uncompressedSize;		//uncompressed package的字节数
  uint32 compressedSize;        //compressed package的字节数
  byte* compressedPackage;      //数据压缩包，长度为compressedSize
};
```

compressedPackage解压缩后，主要包含LODPackages、ModelElements、IDInfo和Extensions四个部分，以及用于标识扩展信息的options和用于标识IDInfo对象表述形式的idInfoRepresentationOption。

<img src="../../images/s3mb_binary.png" art="数据文件二进制数据包" width="700"/>

## LODPackages的二进制流描述

LODPackages及相关对象的二进制流结构应符合如下规定：
```
LODPackages{
  uint32 streamSize; 	              //二进制流字节数
  int32 lodPackageCount;              //LODPackage对象的个数
  LODPackage lodPackages[lodPackageCount];
  byte reserved[n];					 //补齐四字节，n取值为0～3
};
LODPackage{
  float rangeValue;                   //LOD选取阈值因子
  RangeMode rangeMode;                //LOD选取模式，存储为uint16
  BoundingSphere boundingSphere;      //包围球
  BoundingBox boundingBox;            //包围盒
  String strChildTile;                //挂接的子文件的相对路径
  int32  patchCount ;                 //包含的Patch个数
  Patch  patches [patchCount];
  String animations;                  //动画字符串（JSON格式）
};
RangeMode{
  Distance_From_EyePoint = 0,         //根据到相机的距离选取LOD
  Pixel_Size_OnScreen = 1，           //根据投影到屏幕的像素大小选取LOD
  Geometric_Error = 2                 //根据几何误差模式选取LOD
};
BoundingSphere{
  double x;                           //中心点x坐标
  double y;                           //中心点y坐标
  double z;                           //中心点z坐标
  double r;                           //包围球半径
};
BoundingBox{
  Point3D center;                     //包围盒中心点
  Vector3 xExtent;                    //包围盒x方向和长度
  Vector3 yExtent;                   //包围盒y方向和长度
  Vector3 zExtent;                   //包围盒Z方向和长度
};
Point3D{
  double x;                          //空间点的x坐标值
  double y;                          //空间点的y坐标值
  double z;                          //空间点的z坐标值
};
Vector3{
  double x;                          //向量的x分量
  double y;                          //向量的y分量
  double z;                          //向量的z分量
};
Patch{
  Matrix4D matrix4d;	             	// 应符合右手笛卡尔坐标系
  int32 skeletonCount;
  String skeletonNames[skeletonCount];  //骨架名
};
Matrix4D{                               //4×4矩阵，行主序
  double values[16];
};
```

LODPackage的Patch中仅存储骨架名，对应的元素对象存储在[ModelElements](#modelelements的二进制流描述)中。

## ModelElements的二进制流描述

ModelElements及相关对象的二进制流结构应符合如下规定：

```
ModelElements{
  uint32 skeletonsStreamSize; 	          //骨架数据流的二进制长度，以byte为单位
  int32 skeletonCount;                   //骨架对象的数量
  Skeleton skeletons[skeletonCount];     //骨架对象集合
  uint32 texturesStreamSize;             //纹理数据流的二进制长度，以byte为单位
  int32 textureCount;                    //纹理对象的数量
  Texture textures[textureCount];        //纹理对象集合
  String materials;               	      //材质字符串（JSON格式）
};
Skeleton{                              
  uint32 skeletonStreamSize;
  String name;
  byte reserved[n];					      //补齐四字节，n取值为0～3
  uint32 options;				          //骨架对象是否压缩的标识信息，取值为0时，表示不压缩，取值为1时，表示压缩
  VertexDataPackage vertexDataPackages;   //顶点数据包
  int32 indexPackageCount;                
  IndexPackage indexPackages[indexPackageCount];
  BoundingBox boundingBox；
};
VertexDataPackage{
  uint32 vertexDataPackageStreamSize;
  uint32 vertexCount;               //顶点的数量
  uint16 vertexDimension;  
  uint16 vertexStride;
  float vertexData[vertexCount * vertexDimension];
  uint32 normalCount;               //法线的数量
  uint16 normalDimension;
  uint16 normalStride;
  float normalData[normalCount * normalDimension];
  uint32 vertexColorCount;          //顶点颜色值的数量
  uint16 vertexColorStride;
  byte reserved[2];
  uint32 vertexColorData[vertexColorCount]; //颜色采用uint32存储，byte[0]~byte[3]分别表示R、G、B、A的值
  uint16 textureCoordCount;     //纹理坐标对象的数量
  byte reserved[2];
  TextureCoord textureCoords[textureCoordCount];
  uint16 instanceInfoCount;     //实例化信息的数量
  byte reserved[2];
  InstanceInfo instanceInfos[instanceInfoCount];
  VertexAttributeExtension extension;		//顶点扩展属性
  uint32 tangentCount;				//切线的数量
  uint16 tangentDimension;
  uint16 tangentStride;
  float tangentData[tangentCount * tangentDimension];
};
TextureCoord{
  uint32 coordsCount;
  uint16 dimension;
  uint16 stride;
  float data[coordsCount*dimension];
};
InstanceInfo{                    //实例化信息
  uint32 instanceCount;          //实例化对象的数量
  InstanceObiect instanceObjects[instanceCount];   
};  
InstanceObject{                  //实例化对象
  uint16 count;                  //实例化对象变换矩阵值的数量，取值为16
  byte reserved[2];
  float matrixValues[count];     //4×4实例化对象变换矩阵，行主序，前12个值存储该矩阵内容的前三行（3×4），后4个值为预留位
};
VertexAttributeExtension{
  uint32 vertexAttributeCount;		//顶点属性数组的数量
  VertexAttribute vertexAttributes[vertexAttributeCount];
  String descript;                  //顶点属性数组的语义描述信息，采用JSON格式表述
  byte reserved[n];					 //补齐四字节，n取值为0～3
};
VertexAttribute{
  uint32 count;
  uint16 dimension;
  VertexAttributeType vertexAttributetype;       //存储为uint16
  Variant data[count*dimension];     //顶点属性，说明：若VertexAttributeType为AT_32BIT，则variant类型为uint32；若VertexAttributeType为AT_FLOAT，则variant类型为float；若VertexAttributeType为AT_DOUBLE，则variant类型为double；若VertexAttributeType为AT_16BIT，则variant类型为uint16
};
VertexAttributeType{
  AT_32BIT=0,       //属性类型用uint32表示
  AT_FLOAT=1,       //属性类型用float表示
  AT_DOUBLE=2,      //属性类型用double表示
  AT_16BIT=3        //属性类型用uint16表示
};
IndexPackage{
  uint32 indexPackageStreamSize;
  uint32 indexCount;
  VertexIndexType indexType;       //存储为byte
  bool isUseIndex; 
  DrawPrimitiveType drawPrimitiveType;     //存储为byte
  byte reserved;
  variant indexData[indexCount];//索引值，说明：若IndexType为IT_16BIT，则variant类型为uint16；若IndexType为IT_32BIT，则variant类型为uint32）
  byte reserved[n];					 //补齐四字节，n取值为0或2
  int32 passCount;
  String passNames[passCount];
  byte reserved[n];					 //补齐四字节，n取值为0～3
};
VertexIndexType{
  IT_16BIT = 0,               //索引值用uint16表示
  IT_32BIT = 1                //索引值用uint32表示
};
DrawPrimitiveType{
  DPT_POINT_LIST = 1,         //点
  DPT_LINE_LIST  = 2,         //线
  DPT_LINE_STRIP = 3,         //条带线
  DPT_TRIANGLE_LIST = 4,      //独立三角形
  DPT_TRIANGLE_STRIP = 5,     //三角形条带
  DPT_TRIANGLE_FAN = 6,       //三角形扇面
  DPT_QUAD_STRIP = 8,         //四边形条带
  DPT_QUAD_LIST = 9,          //四边形串的集合，不共享边
  DPT_POLYGON = 10            //多边形
};
Texture{
  String strName;
  byte reserved[n];					 //补齐四字节，n取值为0～3
  int32 mipmapLevel;
  TextureData textureData;
};
TextureData{
  uint32 width;
  uint32 height;
  TextureCompressType compressType;   //存储为uint32
  uint32 dataSize;
  PixelFormat pixelFormat;            //存储为uint32
  byte data[dataSize];
};
TextureCompressType{
  TC_NONE = 0,
  TC_DXT1_RGB = 33776,		//RGB，16进制的值为0x83F0
  TC_DXT1_RGBA = 33777,		//RGBA，16进制的值为0x83F1
  TC_DXT3 = 33778,			//16进制的值为0x83F2
  TC_DXT5 = 33779,			//16进制的值为0x83F3
  TC_PVR = 35843，			//16进制的值为0x8C03
  TC_ETC1 = 36196，			//16进制的值为0x8D64
  TC_ETC2 = 37496,			//16进制的值为0x9278
  TC_WEBP = 38000,			//16进制的值为0x9470
  TC_CRN = 38001,     		//16进制的值为0x9471
  TC_KTX2 = 38002           //16进制的值为0x9472
};
PixelFormat{
  PF_RGB8 = 32849,			//16进制的值为0x8051
  PF_RGBA8 = 32856,			//16进制的值为0x8058
  PF_RGB32F = 34837,		//16进制的值为0x8815
  PF_RGBA32F = 34836		//16进制的值为0x8814
};
BoundingBox{
  Point3D center;           //数据包围盒中心点
  Vector3 xExtent;          //数据包围盒x方向和长度
  Vector3 yExtent;          //数据包围盒y方向和长度
  Vector3 zExtent;          //数据包围盒Z方向和长度
};
Point3D{
  double x;                //空间点的x坐标值
  double y;                //空间点的y坐标值
  double z;                //空间点的z坐标值
};
Vector3{
  double x;               //向量的x分量
  double y;               //向量的y分量
  double z;               //向量的z分量
};
```
上述二进制流描述中的Skeleton对象为无压缩的Skeleton对象，采用几何压缩技术的Skeleton对象的二进制流描述见[相关规定](./Skeleton.md#二进制流描述)。


## IDInfo的二进制流描述
IDInfo及相关对象的二进制流结构应符合如下规定：

```
IDInfo{
  uint32 iDInfoStreamSize;          //IDInfo数据流的二进制长度，以byte为单位
  int32  count;                              //Tile中骨架的数量
  SkeletonIDInfo  skeletonIDInfos[count];    //Tile中骨架ID信息集合
};
SkeletonIDInfo{
  String name;                                //骨架的名称
  int32 monomerCount;                         //骨架中单体对象的数量
  MonomerIDInfo monomerIDInfos[monomerCount]; //骨架中单体对象ID信息集合
};
MonomerIDInfo{
  uint32 id;    		                          //单体对象ID
  int32 segmentCount;                         //单体对象包含的数据块数量
  SegmentIDInfo segmentIDInfos[segmentCount];  
};
SegmentIDInfo{
  int32 vertexOffset; 
  int32 vertexCount;
};
```
采用JSON格式表述时，IDInfo对象应以字符串数据类型存储，其标签对应上述IDInfo二进制流结构中的属性变量。
