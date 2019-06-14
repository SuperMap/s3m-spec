#pragma once

#include "Base3D/UGModelNode.h"
#include "FileParser/UGFileParseModel.h"
#include "CacheBuilder3D/UGS3MBTags.h"
#include "Toolkit3D/UGJsonUtils.h"

namespace UGC
{
class CACHEBUILDER3D_API UGS3MBLayerInfos
{
public:
	//! \brief 属性信息保存到文件
	static UGbool SaveLayerInfo2File(std::vector<UGLayerCacheInfo> &vecLayerInfos, UGString& strAttFilePath);
	//! \brief 属性信息加载
	static UGbool LoadLayerInfoFromFile( const UGString& strAttFilePath, std::vector<UGLayerCacheInfo> &vecLayerInfos);

	//! \brief 属性值保存到文件
	static UGbool SaveAttributeDataToFile(std::map<UGint, std::pair<UGint, UGArray<UGFeature*> > >& mapDatasetFeature, \
		UGString& strFileName, UGbool bZip);

	//! \brief 属性值保存到文件
	static UGbool SaveAttributeDataToFile(const UGArray<UGFeature*> arrFeature, UGString strFileName, UGbool bZipp = FALSE);

	//! \brief 属性值保存加载
	static UGbool LoadAttributeDataFromFile(const UGString& strAttFilePath, std::map<UGuint, UGFeature*>& mapFeature);

private:
	//! \brief 构造json对象
	static void MakeMarkup(std::vector<UGLayerCacheInfo> &vecLayerInfos, UGMarkup& markup);

	//! \brief 字段信息保存xml
	static void AddFieldInfos(UGMarkup& markup, const OgdcFieldInfos& fieldInfos);
	static void GetFieldInfos(OgdcFieldInfos& fieldInfos, UGMarkup& markup);

	//! \brief 字段信息保存xml（UGFieldDefine 里没有字段别名）
	static void AddFieldDefines(UGMarkup& markup, const UGArray<UGFieldDefine>& arrFieldDefine);

	static void AddIDRange(UGMarkup& markup, const UGint nMin, const UGint nMax);
	static void GetIDRange(UGMarkup& markup, UGint &nMin, UGint &nMax);

	static UGbool SaveAttributeDataToMarkup(std::map<UGint, std::pair<UGint, UGArray<UGFeature*> > >& mapDatasetFeature, UGMarkup& markup);
	static UGbool LoadAttributeDataFromMarkup(std::map<UGuint, UGFeature*>& mapFeature, UGMarkup& markup);
	
	//! \brief mapFeature 里的ID是否在范围内
	static UGbool IsIDExisted(std::map<UGuint, UGFeature*>& mapFeature, UGint nBeginID, UGint nEndID);

	//! \brief 对象的字段值
	static void AddFeatureValues(UGMarkup& markup, UGFeature *pFeature);
	static UGbool GetFeatureValues(UGFeature *pFeature, UGMarkup& markup);

public:
	/*S3MB 升级之后 属性字段与属性值的存储读取接口*/
	//--------------------------------------------JSON------------------------------------------------
	//! \brief 属性信息保存到文件
	static UGbool SaveLayerInfoToJson(std::vector<UGLayerCacheInfo> &vecLayerInfos, UGString& strAttFilePath);

	//! \brief 属性信息加载
	static UGbool LoadLayerInfoFromJson( const UGString& strAttFilePath, std::vector<UGLayerCacheInfo> &vecLayerInfos);

	//! \brief 点云属性信息保存到文件
	static UGbool SaveLayerInfoToJson(std::map<UGuchar, UGlong>& mapClassificationInfos, UGString& strAttFilePath);

	//! \brief 点云属性信息加载
	static UGbool LoadLayerInfoFromJson(const UGString& strAttFilePath, std::map<UGuchar, UGlong>& mapClassificationInfos);

	//! \brief 属性值保存到文件
	static UGbool SaveAttributeDataToJson(const UGArray<UGFeature*> arrFeature, UGString strFileName);

	//! \brief 属性值保存到文件
	static UGbool SaveAttributeDataToJson(std::map<UGint, std::pair<UGint, UGArray<UGFeature*> > >& mapDatasetFeature, \
		UGString& strFileName);

	//! \brief 属性值保存加载
	static UGbool LoadAttributeDataFromJson(const UGString& strAttFilePath, std::map<UGuint, UGFeature*>& mapFeature);

private:
	//-----------------------------Json-------------------------------------------------------------
	//! \brief 构造json对象
	static void ToJson(std::vector<UGLayerCacheInfo>& vecLayerInfos, UGJsonValue& jsonValue);
	static void FromJson(UGJsonValue& jsonValue, std::vector<UGLayerCacheInfo>& vecLayerInfos);

	//! \brief 点云
	static void ToJson(std::map<UGuchar, UGlong> &mapClassificationInfos, UGJsonValue & jsonValue);
	static void FromJson(UGJsonValue & jsonValue, std::map<UGuchar, UGlong> &mapClassificationInfos);

	static UGbool AttributeDataToJson(std::map<UGint, std::pair<UGint, UGArray<UGFeature*> > >& mapDatasetFeature, UGJsonValue & jsonValue);
	static UGbool AttributeDataFromJson(std::map<UGuint, UGFeature*>& mapFeature, UGJsonValue & jsonValue);

	static void AddIDRangeToJson(UGJsonValue & jsonValue, const UGint nMin, const UGint nMax);
	static void GetIDRangeFromJson(UGJsonValue & jsonValue, UGint &nMin, UGint &nMax);

	static void AddFieldDefinesToJson(UGJsonValue & jsonValue, const UGArray<UGFieldDefine>& arrFieldDefine);
	
	static void AddFieldInfosFromJson(UGJsonValue & jsonValue, const OgdcFieldInfos& fieldInfos);
	static void GetFieldInfosFromJson(OgdcFieldInfos& fieldInfos, UGJsonValue & jsonValue);

	static void AddFeatureValuesToJson(UGJsonValue & jsonValue, UGFeature *pFeature);
	static UGbool GetFeatureValuesFromJson(UGFeature *pFeature, UGJsonValue & jsonValue);

	//FieldInfo Type
	static UGString GetFieldInfoType(OgdcFieldInfo::FieldType nType);
	static OgdcFieldInfo::FieldType GetFieldInfoType(UGString strType);
};

}
