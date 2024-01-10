# 关于
用于实现S3MB数据在Web端的解析、加载。
- 支持符合T/CAGIS 1-2019《空间三维模型数据格式》标准规定的S3MB数据（对应S3M 1.0）
- 支持S3M 2.0、S3M 3.0数据。
- 支持符合CH/T 9040-2023《空间三维模型瓦片数据格式》标准规定的S3MB数据。

<b>注意：</b>目前仅支持解析纹理压缩格式为DXT的数据。

# 目录
1、data: 数据目录，目前文件夹内包含两种范例数据

2、S3M_module：用于解析的和加载的源码

3、\*.html：范例程序

# 使用方法
1. 在Cesium官网下载[release包](https://github.com/CesiumGS/cesium)，将包中Build文件夹放在S3M_JS目录下，与S3M_module同级。

2. 准备数据，数据目录为S3M_module的data文件夹。
   
3. 将源码包放在服务端（比如tomcat）的webapp文件夹下发布。

4. 若使用精模数据，范例为CBD.html，若使用实例化数据，范例为instance.html。