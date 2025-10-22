#if !defined(I3STOOL_H__87DE149A_55B5_485B_883F_4CE1487C7A8B__INCLUDED_)
#define I3STOOL_H__87DE149A_55B5_485B_883F_4CE1487C7A8B__INCLUDED_

#pragma once
#include "Common.h"
#include "I3SParser.h"
#include "stdafx.h"

namespace S3MB
{
	class STK_API I3STool
	{
	public:
		I3STool();
		~I3STool();

	public:
		// 转s3m
		// 注意：1、点数据，仅支持obj格式的外挂文件类型
		//		 2、必须设置是否为经纬度坐标的参数
		bool ToS3M(const I3SParams& params);

	private:
		// 准备参数
		I3SParams PrepareParams(const I3SParams& params);
		// 处理图层
		bool ProcessLayer(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// 处理节点
		bool ProcessNodes(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// 处理符号
		bool ProcessSymbols(const I3SParams& params, const I3SParseParams& parseParams);
	};
}

#endif