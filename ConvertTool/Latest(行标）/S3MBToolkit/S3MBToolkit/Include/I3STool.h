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
		// תs3m
		// ע�⣺1�������ݣ���֧��obj��ʽ������ļ�����
		//		 2�����������Ƿ�Ϊ��γ������Ĳ���
		bool ToS3M(const I3SParams& params);

	private:
		// ׼������
		I3SParams PrepareParams(const I3SParams& params);
		// ����ͼ��
		bool ProcessLayer(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// ����ڵ�
		bool ProcessNodes(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// �������
		bool ProcessSymbols(const I3SParams& params, const I3SParseParams& parseParams);
	};
}

#endif