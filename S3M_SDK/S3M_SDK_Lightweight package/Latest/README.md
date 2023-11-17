# 关于
提供用于实现S3MB数据读写的源码。
- 支持跨平台
- 支持读写符合CH/T 9040-2023《空间三维模型瓦片数据格式》行业标准规定的S3MB数据
- 支持读取S3M 3.0数据

# 目录
1. 3rdparty：包含生成所需第三方库的CMake脚本和参数文件

2. s3m : S3M解析库，用于实现对S3MB数据的解析

    > 注意：  
    > - Shell.h中RenderOperationGroup类对应CH/T 9040-2023文本中的Tile对象  
    > - Shell.h中RenderOperationPagedLOD类对应CH/T 9040-2023文本中的LODPackage对象  
    > - Shell.h中RenderOperationGeode类对应对应CH/T 9040-2023文本中的Patch对象  

3. sample：范例代码程序

4. sampledata：范例数据，包括属性描述文件范例和生成S3MB数据所需的纹理图片。

5. CMakeLists.txt：用于生成S3M解析库及范例的CMake脚本。


# 使用方法
## Windows

1. 下载构建并编译所需要的第三方库。

    - 打开本地所使用的Visual Studio，选择导入相关文件。
        - 若使用2017及以下版本，选择[文件]->[打开]->[文件夹]，选择本地存放3rdparty的路径。
        - 若使用2019及以上版本，选择[打开本地文件夹]，选择本地存放3rdparty的路径。
    - 选择需要构建的配置。
    - 选择[CMake]->[全部生成]。

2. 生成S3M解析库及范例。

    - 启动./cmake-gui.exe。其中，源代码路径选择：./Cmakelist.txt。生成路径可根据需要任意选择。
    - 点击configure，配置所需的工具、平台等。
   - 可选参数说明：
       - build_Shared ：生成为动态库使用。
       - build_sample ：生成范例。

3. 配置完参数后点击Generate。

4. 点击Open Project。

5. 生成工程。 

## Linux

1. 下载构建并编译所需要的第三方库。通过如下Linux终端命令完成操作：
    - cd ./3rdParty
    - mkdir build
    - cd build && cmake ../
    - make

    > 运行完make命令后，会在3rdparty目录中生成S3M解析库所依赖的库的头文件和库文件。

2. 生成S3M解析库及范例。通过如下Linux终端命令完成操作：
   - mkdir build
   - cd build && cmake ../
   - make

    > 运行完make命令后，会在当前目录生成S3M解析库和所需要的范例。

 