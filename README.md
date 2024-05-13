<p align="center">
<img src="./images/s3m.png" />
</p>

S3M (Spatial 3D Model)是一种开放式、可扩展的空间三维数据格式，旨为海量多源异构三维地理空间数据在不同终端之间的传输、交换与共享提供数据格式的具体规范。

<p align="center">
<img src="./images/data.png" />


## 主要技术特点

S3M的主要技术特点可以概括为：

1. 支持表达多源地理空间数据：倾斜摄影三维模型、BIM、建筑白模、城市精细模型、激光点云、矢量、地下管线等多源数据。

2. 具备海量数据高效绘制的能力：支持LOD、批次绘制、实例化等技术，可以提升渲染性能。

3. 具备单体化选择和查询能力：支持高效选择、支持批量修改对象颜色、批量修改对象可见性。

4. 全面支持PBR（Physically-based rendering，基于物理的渲染）材质：支持利用更符合物理学规律的方式来渲染出更真实的对象和场景。

5. 支持纹理压缩和几何压缩技术：支持WebP、CRN、KTX2.0等纹理压缩格式以及几何压缩技术，可大幅降低数据量，实现不同终端使用同一套数据。

6. 支持骨骼动画，可用于表达人、物运动效果。

7. 简洁易读：格式简洁，易解析，适用于WebGL等轻量级客户端。

8. 高效传输：数据紧凑，以二进制形式保存，占用空间小，传输性能高。

9. 快速载入显卡：数据结构尽可能跟OpenGL接口保持一致，减少加载时间，高性能，低消耗。

10. 跨终端：独立于任何终端，适用于PC端、Web端、移动端、游戏引擎等，具有较好的兼容性。

11. 可扩展性：支持扩展，允许增加一些多用途扩展或特定厂商的扩展。

12. 完善的工具支撑：提供S3M数据解析和转换工具，支持3D-Tiles、OSGB等格式与S3M格式相互转换。

13. 完善的配套设施：完整的解决方案，成熟的可行性，强大的实用性。基于S3M形成了完整的B/S及C/S架构的三维GIS应用解决方案，从数据生成、服务器端发布到多种客户端加载应用等多个环节解决用户实际问题。

## 对接软件

目前，S3M标准格式已在国内外多类软件平台得到应用，主要内容见下方。

### 数据生产

| **厂商** | **产品** | **产品定位** | **能力**  |
|--- |----| ----| ---- |
|[Bentley](https://www.bentley.cn/) |Context Capture | 实景建模软件 | 支持生产符合S3M标准的倾斜摄影三维模型数据 |
|[大疆](https://www.dji.com/au)| 大疆智图DJI Terra| 三维重建软件| 支持生产符合S3M标准的倾斜摄影三维模型数据|
|[东方道迩](http://gx.eastdawn.com.cn/)| pixe3DBuilder Engine(P3BJet)|跨平台三维重建核心引擎| 支持生产符合S3M标准的倾斜摄影三维模型数据 |
|[大势智慧](https://www.daspatial.com/) | 重建大师（GET3D）| 超大规模实景三维集群建模软件 | 支持生产符合S3M标准的倾斜摄影三维模型数据  |
|[瞰景科技](http://www.soarsky.com.cn/) | Smart3D | 实景三维建模系统 | 支持生产符合S3M标准的倾斜摄影三维模型数据  |
|[天际航](https://www.whulabs.com/) | DP-Modeler | 图像快速建模系统  |  支持生产符合S3M标准的倾斜摄影三维模型数据 |

### 数据应用
| **厂商** | **产品** | **产品定位** | **能力**  |
|--- |----| ----| ---- |
|[华为](https://www.huawei.com/cn/) | 沃土数字平台 | 构建数字世界的底座 | 将S3M作为其3D GIS标准服务，对项目合作伙伴免费开放 |
|[阿里](https://datav.aliyun.com/portal/) |DataV| 数据可视化平台 | 支持在平台中加载、可视化和应用S3M数据 |
| [京东](https://www.jdcloud.com/cn/solutions/cityos)|城市操作系统 | 智能城市大数据平台 | 支持在平台中加载、可视化和应用S3M数据 |
| [睿呈时代](https://www.resafety.com/)| RAY-BDV|大数据可视化平台 | 支持加载、可视化和应用S3M数据|
|[光辉城市](https://www.sheencity.com/mars) |DIVA | 数字孪生系统构建平台 | 支持在平台中加载、可视化和应用S3M数据 |
|[飞渡科技](https://www.freedoonline.com/) |iFreedo | 数字底板“中枢” | 支持加载、可视化和应用S3M数据 |
| [华为](https://developer.huawei.com/consumer/cn/hms/3d-engine/?ha_source=hms1) | HMS Core 3D Engine | 实时3D引擎 |通过SuperMap Hi-Fi 3D SDKs支持加载、可视化和应用S3M数据|
|[Epic Game](https://www.unrealengine.com/zh-CN/) |UE4 | 游戏引擎| 通过SuperMap Hi-Fi 3D SDKs支持加载、可视化和应用S3M数据 |
|[Epic Game](https://www.unrealengine.com/zh-CN/) | UE5 |游戏引擎 |通过SuperMap Hi-Fi 3D SDKs支持加载、可视化和应用S3M数据 |
| [Unity](https://unity.com/cn) | Unity |实时3D互动内容创作和运营平台 | 通过SuperMap Hi-Fi 3D SDKs支持加载、可视化和应用S3M数据|
|[维图斯](https://www.ventuz.com.cn/) | Ventuz | 三维实时图文内容创作软件 | 支持加载、可视化和应用S3M数据 |
|[中设数字](http://www.cbim.com.cn/)|马良XCUBE| BIM数智设计软件 |支持加载、可视化和应用S3M数据 |
|[天房云科技](http://www.tftech.cn/)|天房云平台| BIM+GIS应用平台 | 支持加载、可视化和应用S3M数据|

### [超图全系列基础平台产品](https://www.supermap.com/zh-cn/a/product/gis-about-2023.html)支持

| **产品** | **产品定位** | **能力**  |
|----| ----| ----|
| SuperMap iDesktop/SuperMap iDesktopX| 二三维一体化桌面 GIS 软件平台 | 支持将倾斜摄影三维模型、点云、BIM、城市精细模型、建筑白模、矢量数据、地下管线等多源异构数据生成为S3M数据，还支持S3M数据处理功能 |
| SuperMap iExplorer3D | 三维场景浏览软件 | 支持加载、可视化和应用S3M数据 |
| SuperMap iObjects |大型全组件式GIS软件开发平台 | 支持通过API将倾斜摄影三维模型、点云、BIM、城市精细模型、建筑白模、矢量数据、地下管线等多源异构数据生成为S3M数据，还支持S3M数据处理功能|
|SuperMap iServer|  服务器GIS软件平台 | 支持将S3M数据发布为三维服务，供多终端使用|
|SuperMap Hi-Fi 3D SDKs |高保真三维GIS开发平台 | 支持加载、可视化和应用S3M数据 |
|SuperMap iClient3D for WebGL/WebGPU/Cesium|三维GIS网络客户端开发平台 | 支持加载、可视化和应用S3M数据 |
|SuperMap iMobile for iOS/Android | 移动GIS开发平台 | 支持加载、可视化和应用S3M数据 |


### 其他

深圳可视化城市空间数字平台将S3M作为其GIS标准服务，对深圳市各行政事业单位免费开放。

并且光启元RayData专业版、吉嘉时空的Gaea Explorer、洛阳众智WiseMap、深圳嘉瑞建设BIM项目应用系统等国内外多个倾斜摄影建模软件、BIM软件及前端的三维渲染引擎正在开展对接工作。


## 开放资源
* ConvertTool：转换工具，用于实现S3M数据与3D Tiles的相互转换，以及与OSGB、OBJ的转换。

* S3M_SDK：读写及解析工具，包括S3M数据的C++读写开发包和JavaScript解析源码以及相关范例。

* Specification：提供中文、英文两种语言表述的[T/CAGIS 1-2019标准文本](/Specification/T／CAGIS%201-2019/)，以及[S3M数据格式规范](/Specification/Specification.md)。


## 许可

数据格式标准规范遵循[Creative Commons Attribution-NoDerivatives 4.0 International](https://creativecommons.org/licenses/by-nd/4.0/legalcode)协议。

S3M_SDK、ConvertTool以及相关数据示例遵循[Apache License V2.0协议](https://www.apache.org/licenses/LICENSE-2.0) 。





















