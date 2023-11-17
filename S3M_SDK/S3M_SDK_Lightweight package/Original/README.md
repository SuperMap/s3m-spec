# 关于
提供实现S3MB数据读写的开发工具包（Win64位 C++）。
- 支持读写S3M 1.0数据（符合T/CAGIS 1-2019《空间三维模型数据格式》的规定）
- 支持读取S3M 2.0数据。

# 目录
1、MSProject：包括各Visual Studio版本对应的工程文件、dll以及所需要的第三方库，用于实现S3MB数据的读写；

2、S3MB_DLL/Include：使用MSProject/VS201X/Bin路径下的S3MB_DLL.dll文件需要的头文件；

3、S3MB_Sample：范例程序代码，用于实现使用MSProject/VS201X/Bin路径下的S3MB_DLL.dll文件读取S3MB文件；

4、SampleData/attribute：范例数据，用于读取属性数据文件（s3md文件）和属性描述文件(attribute.json)；

5、S3MB_SDK_201x.sln：各Visual Studio版本对应的解决方案文件。


# 使用方法
1. 启动对应Visual Studio版本的.sln文件。

2. 打开S3MB_Sample/Src路径下的main.ccp文件，修改main.ccp文件中的mian函数内的fVersionNumber值，用于选择s3mb文件的版本。其中，fVersionNumber值为1时，s3mb文件版本为1.0，fVersionNumber值为2时，s3mb文件的版本为2.0。

3. 运行工程。