# 关于
S3M_JS解析库可以迁移到VUE中使用，该页面说明使用要点、常见问题及解决方法。

# 使用要点
1. S3M_module需放vue工程静态资源文件夹，建议数据也放静态资源文件夹。

# 常见问题及解决方法

1. 启动项目报错，例如使用webpack v5，但其中polyfill Node核心模块被删除，会显示如下报错信息。通过安装npm i node-polyfill-webpack-plugin 然后修改vue.config.js文件来解决，具体修改信息见下图。

    ![报错信息](.\images\ErrorMessage.png)

    ![修改信息](.\images\Sample.png)

2. 启动工程后报错：Cesium is not defined。
    - 如果是由于eslint禁止访问window上的属性，可以在vue.config.js中关闭eslint：lintOnSave: false
    - 如果是其他原因导致的，可以通过搜索来查找解决方法，以保证能直接拿到window上cesium对象。

3. 如果在组件中引用S3MTilesLayer.js页面卡死，可以通过如下操作去掉源码中的混淆代码：在S3ModelParser.js文件中将下面引用注释掉，并注释代码中使用的地方即可。
    - 注意：经过此操作后可以加载范例数据，但不能使用Draco和Meshopt压缩的数据。

    ![注释内容](.\images\CodeAnnotation.png)
       
