echo off

set sourcePathReleaseDLL=E:\trunk\Builds\Win_Solution_vc11\Bin_Unicode_x64
set resulePathReleaseDLL=E:\S3M_SDK\Bin

set fileNames="CodeTransition.xml" "icudt38.dll" "icudt51.dll" "icuin38.dll" "icuuc38.dll" "icuuc51.dll" "libcurl.dll" "libexpat.dll" "libiconv.dll" "libImageOperator.dll" ^
             "liblzmafile.dll" "mfc110u.dll" "mfcm110u.dll" "Microsoft.VC90.CRT.manifest" "msvcp110.dll" "msvcr110.dll" "msvcr90.dll" "PrjConfig.xml" "resource.xml" "resource.zh-CN.xml" "SuAlgorithm.dll" "SuBase3D.dll" "SuBase.dll" ^ "SuCacheBuilder3D.dll" "SuCompactFile.dll" "SuElement.dll" "SuFileParser3DModel.fps" "SuFileParser.dll" "SuFileParserS3MB.fps" ^ "SuGeometry.dll" "SuNetToolkit.dll" "SuOGDC.dll" "SuperMap.xml" "SuProjection.dll" "SuSpatialIndex.dll" "SuStream.dll" ^
			  "SuToolkit3D.dll" "SuToolkit.dll" "vccorlib110.dll" "vcomp110.dll" ^ 

for %%i in (%fileNames%) do (
    if not exist %resulePathReleaseDLL% md %resulePathReleaseDLL%;
	copy /y %sourcePathReleaseDLL%\%%i %resulePathReleaseDLL%\%%i 
);

set sourcePathDebugDLL=E:\trunk\Builds\Win_Solution_vc11\BinD_Unicode_x64
set resulePathDebugDLL=E:\S3M_SDK\BinD

set fileNames="CodeTransition.xml" "icudt38.dll" "icudt51.dll" "icuin38.dll" "icuuc38.dll" "icuuc51d.dll" "libcurl.dll" "libexpat.dll" "libiconv.dll" "libImageOperator.dll" ^
              "liblzmafile.dll" "mfc110ud.dll" "mfcm110ud.dll" "Microsoft.VC90.DebugCRT.manifest" "msvcp110d.dll" "msvcr110d.dll" "msvcr90.dll" "PrjConfig.xml" "resource.xml" "resource.zh-CN.xml" "SuAlgorithmd.dll" "SuBase3Dd.dll" ^  "SuBased.dll" "SuCacheBuilder3Dd.dll" "SuCompactFiled.dll" "SuElementd.dll" "SuFileParser3DModeld.fps" "SuFileParserd.dll" ^  "SuFileParserS3MBd.fps" "SuGeometryd.dll" "SuNetToolkitd.dll" "SuOGDCd.dll" "SuperMap.xml" "SuProjectiond.dll" "SuSpatialIndexd.dll" "SuStreamd.dll" ^
			  "SuToolkit3Dd.dll" "SuToolkitd.dll" "vccorlib110d.dll" "vcomp110d.dll" ^ 

for %%i in (%fileNames%) do (
    if not exist %resulePathDebugDLL% md %resulePathDebugDLL%;
	copy /y %sourcePathDebugDLL%\%%i %resulePathDebugDLL%\%%i 
);

set sourcePathIncludeBase=E:\trunk\Include\Base
set resulePathIncludeBase=E:\S3M_SDK\include\Base

set fileNames="OgdcArray.h" "ogdccolordef.h" "OgdcColorDictTable.h" "OgdcColorset.h" "OgdcColorTable.h" "ogdcdefs.h" "OgdcDict.h" "ogdcexports.h" ^
              "OgdcHistogram.h" "OgdcList.h" "OgdcPoint.h" "OgdcPoint2D.h" "OgdcPoint3D.h" "OgdcPointEPS.h" "OgdcRect.h" "OgdcRect2D.h" ^
			  "OgdcRectF.h" "OgdcSize.h" "OgdcSize2D.h" "OgdcSize3D.h" "OgdcString.h" "OgdcStyle.h" "OgdcSystem.h" "OgdcTextStyle.h" ^
			  "OgdcTime.h" "OgdcVariant.h"

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeBase% md %resulePathIncludeBase%;
	copy /y %sourcePathIncludeBase%\%%i %resulePathIncludeBase%\%%i 
);
set sourcePathIncludeBase3D=E:\trunk\Include\Base3D
set resulePathIncludeBase3D=E:\S3M_SDK\include\Base3D

set fileNames="UGAnimation.h" "UGAnimationCurve.h" "UGBoundingBox.h" "UGBoundingSphere.h" "UGColorValue3D.h" "UGCommon3D.h" "UGGpuProgram.h" "UGGpuProgramManager.h" "UGGpuProgramParams.h" "UGLayerBlendMode.h" ^
			  "UGMaterial.h" "UGMaterial3D.h" "UGMaterialScriptManager.h" "UGMathEngine.h" "UGMatrix3d.h" "UGMatrix4d.h" "UGMesh.h" "UGModelEM.h" "UGModelEntityPool.h" "UGModelNode.h" "UGModelNodeEntities.h" ^
			  "UGModelNodeShells.h" "UGModelSkeletonDataPackTemplate.h" "UGNode.h" "UGPass.h" "UGPrerequisites3D.h" "UGQuaternion4d.h" "UGRenderOperationGroup.h" "UGScriptCompiler.h" "UGScriptCompilerManager.h" ^
			  "UGScriptTranslator.h" "UGScriptTranslatorManager.h" "UGSharedPtr.h" "UGTechnique.h" "UGTextureData.h" "UGTextureUnitState.h" "UGVector3d.h" "UGVector4d.h" "UGVertexDataPackageTemplate.h"

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeBase3D% md %resulePathIncludeBase3D%;
	copy /y %sourcePathIncludeBase3D%\%%i %resulePathIncludeBase3D%\%%i 
);

set sourcePathIncludeCacheBuilder3D=E:\trunk\Include\CacheBuilder3D
set resulePathIncludeCacheBuilder3D=E:\S3M_SDK\include\CacheBuilder3D

set fileNames="UGS3MBLayerInfos.h" "UGS3MBManager.h" "UGS3MBPagedLodTree.h" "UGS3MBSCP.h" "UGS3MBTags.h" "UGSCPFile.h" "UGSCPLayerInfos.h"

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeCacheBuilder3D% md %resulePathIncludeCacheBuilder3D%;
	copy /y %sourcePathIncludeCacheBuilder3D%\%%i %resulePathIncludeCacheBuilder3D%\%%i 
);

set sourcePathIncludeElement=E:\trunk\Include\Element
set resulePathIncludeElement=E:\S3M_SDK\include\Element

set fileNames="OgdcCoordSys.h" "OgdcElement.h" "OgdcElemFactory.h" "OgdcFeature.h" "OgdcFieldInfo.h" "OgdcPrjParams.h" "OgdcRasterBlock.h" ^
              "OgdcRasterScanline.h"

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeElement% md %resulePathIncludeElement%;
	copy /y %sourcePathIncludeElement%\%%i %resulePathIncludeElement%\%%i 
);

set sourcePathIncludeFileParser=E:\trunk\Include\FileParser
set resulePathIncludeFileParser=E:\S3M_SDK\include\FileParser

set fileNames="UGExchangeFileType.h" "UGFileParse.h" "UGFileParseDefine.h" "UGFileParseFactory.h" "UGFileParseManager.h" "UGFileParseModel.h" ^
              "UGFileParserConfigParams.h" "UGFileParserTile.h" "UGModelConfigParams.h" "UGVectorConfigParams.h"  

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeFileParser% md %resulePathIncludeFileParser%;
	copy /y %sourcePathIncludeFileParser%\%%i %resulePathIncludeFileParser%\%%i 
);

set sourcePathIncludeGeometry=E:\trunk\Include\Geometry
set resulePathIncludeGeometry=E:\S3M_SDK\include\Geometry

set fileNames="UGDataCodec.h" "UGEditType.h" "UGFeature.h" "UGGeometry.h" 

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeGeometry% md %resulePathIncludeGeometry%;
	copy /y %sourcePathIncludeGeometry%\%%i %resulePathIncludeGeometry%\%%i 
);

set sourcePathIncludeProjection=E:\trunk\Include\Projection
set resulePathIncludeProjection=E:\S3M_SDK\include\Projection

set fileNames="UGDatum.h" "UGGeoCoordSys.h" "UGGeoTranslator.h" "UGHorizonDatum.h" "UGPjCon.h" "UGPrimeMeridian.h" "UGPrjCoordSys.h" "UGPrjParams.h" ^
              "UGPrjTranslator.h" "UGProjection.h" "UGProjectionManager.h" "UGRefTranslator.h" "UGSpatialRef.h" "UGSpheroid.h" "UGTranslateParams.h" 

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeProjection% md %resulePathIncludeProjection%;
	copy /y %sourcePathIncludeProjection%\%%i %resulePathIncludeProjection%\%%i 
);

set sourcePathIncludeStream=E:\trunk\Include\Stream
set resulePathIncludeStream=E:\S3M_SDK\include\Stream

set fileNames="UGByteArray.h" "ugdefs.h" "ugexports.h" "UGFile.h" "UGFileStdio.h" "UGFileStream.h" "ugkeydef.h" "UGMemoryStream.h" "ugplatform.h" ^
              "UGStream.h" "ugversion.h" "ugwin32.h" 

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeStream% md %resulePathIncludeStream%;
	copy /y %sourcePathIncludeStream%\%%i %resulePathIncludeStream%\%%i 
);

set sourcePathIncludeToolkit=E:\trunk\Include\Toolkit
set resulePathIncludeToolkit=E:\S3M_SDK\include\Toolkit

set fileNames="UGAutoPtr.h" "UGColorValue3DEx.h" "UGErrorObj.h" "UGHash.h" "UGHashCode.h" "UGInter.h" "UGListenerControl.h" "UGLogFile.h" "UGMarkup.h" ^
              "UGPalette.h" "ugres.h" "UGSingleton.h" "UGStringEx.h" "UGStyle.h" "UGStyleEx.h" "UGTextCodec.h" "UGTextureInfo.h" "UGThread.h" "UGToolkit.h" 

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeToolkit% md %resulePathIncludeToolkit%;
	copy /y %sourcePathIncludeToolkit%\%%i %resulePathIncludeToolkit%\%%i 
);
set sourcePathIncludeToolkit3D=E:\trunk\Include\Toolkit3D
set resulePathIncludeToolkit3D=E:\S3M_SDK\include\Toolkit3D

set fileNames="UGJsonUtils.h" 

for %%i in (%fileNames%) do (
    if not exist %resulePathIncludeToolkit3D% md %resulePathIncludeToolkit3D%;
	copy /y %sourcePathIncludeToolkit3D%\%%i %resulePathIncludeToolkit3D%\%%i 
);

set sourcePathLibRelease=E:\trunk\Builds\Win_Solution_vc11\Bin_Unicode_x64
set resulePathLibRelease=E:\S3M_SDK\lib\Release

set fileNames="SuBase.lib" "SuBase3D.lib" "SuCacheBuilder.lib" "SuCacheBuilder3D.lib" "SuElement.lib" "SuFileParser.lib" "SuProjection.lib" "SuStream.lib" ^ "SuToolkit.lib" "SuToolkit3D.lib"

for %%i in (%fileNames%) do (
    if not exist %resulePathLibRelease% md %resulePathLibRelease%;
	copy /y %sourcePathLibRelease%\%%i %resulePathLibRelease%\%%i 
);

set sourcePathLibDebug=E:\trunk\Builds\Win_Solution_vc11\BinD_Unicode_x64
set resulePathLibDebug=E:\S3M_SDK\lib\Debug

set fileNames="SuBased.lib" "SuBase3Dd.lib" "SuCacheBuilderd.lib" "SuCacheBuilder3Dd.lib" "SuElementd.lib" "SuFileParserd.lib" "SuProjectiond.lib" "SuStreamd.lib" ^ "SuToolkitd.lib" "SuToolkit3Dd.lib"

for %%i in (%fileNames%) do (
    if not exist %resulePathLibDebug% md %resulePathLibDebug%;
	copy /y %sourcePathLibDebug%\%%i %resulePathLibDebug%\%%i 
);

pause
