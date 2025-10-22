# 关于
用于实现S3MB数据与3D Tiles数据的相互转换，以及OSGB、OBJ等数据转换为S3MB数据，支持跨平台。
- S3MB数据符合CH/T 9040-2023《空间三维模型瓦片数据格式》行业标准的规定，但目前仅支持普通材质。
- 支持符合0GC 3D Tiles标准规定的[3D Tiles](https://github.com/CesiumGS/3d-tiles)数据，支持格式包括:\*.b3dm、\*.i3dm、\*.cmpt数据。
- 支持符合Github社区规范的[13S](https://github.com/Esri/i3s-spec) 1.7及以上版本的数据。支持的原始数据类型包括：倾斜摄影三维模型、建筑白模、城市精模、BIM、点云、点外挂模型。

# 目录
1. 3rdParty：包含生成所需第三方库的CMake脚本，以及在Visual Studio 2017、C++15环境下编译的S3MBTooklkit所需的第三方库。

2. S3MBToolkit：包含实现数据转换所需的头文件和源文件

3. CMakeLists.txt：生成S3MBToolkit工具的CMake脚本

# 使用方法
1. 准备用于转换的数据。

2. ./S3MBToolkit/3rdParty文件夹提供Visual Studio 2017版本的第三方库，可以直接使用。
如果需要其他版本的第三方库，可以进入./S3MBToolkit/3rdParty文件夹，运行CMake脚本，抓取所需对应第三方库的源码。
若因网络原因导致Visual Studio编译报错，建议根据./S3MBToolkit/3rdParty/CMake脚本中的Github仓库路径，自行抓取第三方库源码，编译后放到指定路径。

3. 进入./S3MBToolkit文件夹，运行CMake脚本，其中，需要指定Qt的安装目录(Qt版本应大于5.11，需要在编译生成工具前提前安装好)，从而编译生成S3MBToolkit工具。

4. 根据转换需求，使用S3MBToolkit工具将源数据转换为目标格式数据。 