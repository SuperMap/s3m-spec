#if !defined(I3SPARAMS_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_)
#define I3SPARAMS_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_

#pragma once
#include "Common.h"
#include "I3SFormatStructure.h"
#include "ProcessParams.h"
#include "S3MBLayerInfos.h"
#include "stdafx.h"

namespace S3MB
{
	// �û����õĲ�����
	class STK_API I3SParams : public ProcessParams
	{
	public:
		I3SParams();
		~I3SParams();

		virtual ProcessType GetProcessType();

	public:
		// ����/��ȡԴ�ļ�·��
		void SetInputPath(const std::wstring& strPath);
		std::wstring GetInputPath() const;

		// ����/��ȡ���·��
		void SetOutputPath(const std::wstring& strPath);
		std::wstring GetOutputPath() const;

		// ����/��ȡs3m�汾��
		void SetS3MVersion(float fVersion);
		float GetS3MVersion() const;

		// ����/��ȡ��������
		void SetDataName(const std::wstring& strName);
		std::wstring GetDataName() const;

		// ����/��ȡ�Ƿ��Ǿ�γ������
		// ע�⣺�������õĲ���
		void SetIsLonLatCoordinates(bool bIs);
		bool GetIsLonLatCoordinates() const;

	private:
		// Դ�ļ�·��
		std::wstring m_strInputPath;
		// ���·��
		std::wstring m_strOutputPath;
		// s3m�汾��
		float m_fS3MVersion;
		// ��������
		std::wstring m_strDataName;
		// �Ƿ��Ǿ�γ������
		bool m_bLonLatCoord;
	};

	// �����Ĳ�����
	class STK_API I3SParseParams
	{
	public:
		I3SParseParams();
		~I3SParseParams();

	public:
		// ����/��ȡi3s�汾��
		void SetI3SVersion(float fVersion);
		float GetI3SVersion() const;

		// ����/��ȡ���ڵ�·��������
		void SetRootNodes(const std::vector<std::pair<std::wstring, OrientedBoundingBox> >& vecNode);
		std::vector<std::pair<std::wstring, OrientedBoundingBox> > GetRootNodes() const;

		// ����/��ȡ�������Ժ�id��ӳ���ϵ
		void SetVertexAttributeAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeAndIdRelation() const;

		// ����/��ȡ������չ���Ժ�id��ӳ���ϵ
		void SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeExtensionAndIdRelation() const;

		// ����/��ȡ�Ƿ���face����
		void SetHasFaceAttributes(bool bHas);
		bool GetHasFaceAttributes() const;

		// ����/��ȡ�����ʽ��ӳ���ϵ
		void SetTextureFormats(const std::map<std::wstring, std::wstring>& mapFormat);
		std::map<std::wstring, std::wstring> GetTextureFormats() const;

		// ����/��ȡ����ѹ������
		void SetVertexCompressionType(S3MBVertexTag nType);
		S3MBVertexTag GetVertexCompressionType() const;

		// ����/��ȡ����ϵ
		void SetEpsgCode(int nCode);
		int GetEpsgCode() const;
		bool GetHasEpsgCode() const;

		// ����/��ȡͼ����slpk���·��
		void SetLayerPath(const std::wstring& strPath);
		std::wstring GetLayerPath() const;

		// ����/��ȡ���·��
		void SetOutputPath(const std::wstring& strPath);
		std::wstring GetOutputPath() const;

		// ����/��ȡ������Ϣ
		void SetDrawingInfo(const I3SDrawingInfo& info);
		I3SDrawingInfo GetDrawingInfo() const;
		bool GetHasDrawingInfo() const;

		// ����/��ȡ������Ϣ
		void SetIndexInfo(const I3SIndexInfo& info);
		I3SIndexInfo GetIndexInfo() const;

		// ����/��ȡ������Ϣ
		void SetAttributeInfos(const std::vector<I3SAttributeInfo>& vecInfo);
		std::vector<I3SAttributeInfo> GetAttributeInfos() const;
		void SetFieldInfos(const S3MBFieldInfos& infos);
		S3MBFieldInfos GetFieldInfos() const;

		// ����/��ȡ����Ҫ����Ϣ
		void SetAttStorageInfos(const std::vector<I3SAttStorageInfo>& vecInfo);
		std::vector<I3SAttStorageInfo> GetAttStorageInfos() const;

	private:
		// i3s�汾��
		float m_fI3SVersion;
		// ���ڵ�<·��,��Χ��>������
		std::vector<std::pair<std::wstring, OrientedBoundingBox> > m_vecRootNode;
		// �������Ժ�id��ӳ���ϵ(id����dracoѹ��)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttAndId;
		// ������չ���Ժ�id��ӳ���ϵ(id����dracoѹ��)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttExtAndId;
		// �Ƿ���face����
		bool m_bHasFaceAttributes;
		// �����ʽ��ӳ���ϵ
		std::map<std::wstring, std::wstring> m_mapTexFormat;
		// ����ѹ������
		S3MBVertexTag m_nVexCompType;
		// ����ϵ
		int m_nEpsgCode;
		bool m_bHasEpsgCode;
		// ͼ����slpk�е�·��
		std::wstring m_strLayerPath;
		// ���·��
		std::wstring m_strOutputPath;
		// ������Ϣ
		I3SDrawingInfo m_drawingInfo;
		bool m_bHasDrawingInfo;
		// ������Ϣ
		I3SIndexInfo m_indexInfo;
		// ������Ϣ
		std::vector<I3SAttributeInfo> m_vecAttInfo;
		S3MBFieldInfos m_fieldInfos;
		// ����Ҫ����Ϣ
		std::vector<I3SAttStorageInfo> m_vecAttStorageInfo;
	};
}

#endif