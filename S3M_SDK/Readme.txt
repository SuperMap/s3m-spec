S3MB_SDK是S3MB数据读写开发包（C++），目录结构如下：
1、include:头文件
2、lib：链接库
3、Bin/BinD：动态库
4、Sample：读写S3MB数据的范例程序，Sample.sln为VS2012的工程文件，运行时需要将Plugins下的SuFileParser3DModel.fps拷贝到S3MB_SDK/Bin/，SuFileParser3DModeld.fps拷贝到S3MB_SDK/BinD/。
包含功能：
1）数据读取类 —— S3MBReader，读取数据的骨架、材质、纹理；属性等信息
2）数据写入类 —— S3MBWriter，将Altizure的数据转成S3MB数据
5、SampleData 范例数据