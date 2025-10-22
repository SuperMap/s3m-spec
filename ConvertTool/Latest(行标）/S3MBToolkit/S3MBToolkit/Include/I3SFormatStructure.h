#if !defined(I3SFORMATSTRUCTURE_H__C208EEA6_A176_4378_AC88_2EC66D30B266__INCLUDED_)
#define I3SFORMATSTRUCTURE_H__C208EEA6_A176_4378_AC88_2EC66D30B266__INCLUDED_

#pragma once
#include "BoundingBox.h"
#include "Common.h"
#include "Matrix.h"
#include "Palette.h"
#include "stdafx.h"
#include "Variant.h"

#include <string>

namespace S3MB
{
	class STK_API I3SMetadata
	{
	public:
		I3SMetadata();
		~I3SMetadata();

	public:
		// ����/��ȡ�ڵ�����
		void SetNodeCount(unsigned int nCount);
		unsigned int GetNodeCount() const;

	private:
		// �ڵ�����
		unsigned int m_nNodeCount;
	};

	class STK_API I3SAttStorageInfo
	{
	public:
		I3SAttStorageInfo();
		~I3SAttStorageInfo();

		// ����/��ȡ��ֵ
		void SetKey(const std::wstring& strKey);
		std::wstring GetKey() const;

		// ����/��ȡ����
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

	private:
		// ��ֵ
		std::wstring m_strKey;
		// ����
		std::wstring m_strName;
	};

	class STK_API I3SDrawingInfo
	{
	public:
		enum I3SRendererType
		{
			RT_None,
			RT_Simple,
			RT_UniqueValue
		};

		enum I3SColorMixMode
		{
			CMM_None,
			CMM_Multiply,
			CMM_Replace
		};

		enum I3SSymbol3DType
		{
			ST_None,
			ST_Point,
			ST_StyleReference
		};

	public:
		I3SDrawingInfo();
		~I3SDrawingInfo();

	public:
		// ����/��ȡ��Ⱦ������
		void SetRendererType(I3SDrawingInfo::I3SRendererType eType);
		I3SDrawingInfo::I3SRendererType GetRendererType() const;

		// ����/��ȡ��������
		void SetFieldName(const std::wstring& strName);
		std::wstring GetFieldName() const;

		// ����/��ȡ������ɫ
		void SetMaterialColor(const ColorValue& color);
		ColorValue GetMaterialColor() const;
		bool GetHasMaterialColor() const;

		// ����/��ȡ��ɫ���ģʽ
		void SetColorMixMode(I3SDrawingInfo::I3SColorMixMode eMode);
		I3SDrawingInfo::I3SColorMixMode GetColorMixMode() const;

		// ����/��ȡ��������
		void SetSymbolType(I3SDrawingInfo::I3SSymbol3DType eType);
		I3SDrawingInfo::I3SSymbol3DType GetSymbolType() const;

		// ����/��ȡ����ֵ�ͷ������ƵĹ�ϵ
		void SetFieldSymbols(const std::map<Variant, std::wstring>& mapValue);
		std::map<Variant, std::wstring> GetFieldSymbols() const;

		// ����/��ȡĬ����ҷ��ŵ�����
		void SetDefaultSymbolRef(const std::wstring& strRef);
		std::wstring GetDefaultSymbolRef() const;

		// ����/��ȡ����ļ����ơ�·��
		void SetSymbolPaths(const std::map<std::wstring, std::wstring>& mapPath);
		std::map<std::wstring, std::wstring> GetSymbolPaths() const;

		// ����/��ȡ�������ƺͱ任����Ĺ�ϵ
		void SetSymbolTransforms(const std::map<std::wstring, Matrix4d>& mapValue);
		std::map<std::wstring, Matrix4d> GetSymbolTransforms() const;

	private:
		// ��Ⱦ������
		I3SDrawingInfo::I3SRendererType m_eRendererType;
		// �������ƣ���ֵ��Ϊ���ֲ�ͬ���ŵ�����ֵ��
		std::wstring m_strFieldName;
		// ������ɫ
		ColorValue m_malColor;
		bool m_bHasMalColor;
		// ��ɫ���ģʽ
		I3SDrawingInfo::I3SColorMixMode m_eColorMixMode;
		// ��������
		I3SDrawingInfo::I3SSymbol3DType m_eSymbolType;
		// ����ֵ�ͷ������Ƶ�ӳ���ϵ
		std::map<Variant, std::wstring> m_mapFieldSymbol;
		// Ĭ����ҷ��ŵ�����
		std::wstring m_strDefaultSymbolRef;
		// ����ļ����ơ�·��
		std::map<std::wstring, std::wstring> m_mapSymbolPath;
		// �������ƺͱ任�����ӳ���ϵ
		std::map<std::wstring, Matrix4d> m_mapSymbolTransform;
	};

	// ��S3MBFieldInfo���ʹ��
	class STK_API I3SAttributeInfo
	{
	public:
		I3SAttributeInfo();
		~I3SAttributeInfo();

		// ����/��ȡ��������
		void SetFieldName(const std::wstring& strName);
		std::wstring GetFieldName() const;

		// ����/��ȡ���Ե�ԭʼ�ֶ�����
		void SetOriginFieldType(FieldType nType);
		FieldType GetOriginFieldType() const;

		// ����/��ȡ�Ƿ����
		void SetIsCoded(bool bIs);
		bool GetIsCoded() const;

		// ����/��ȡ����ӳ���ϵ
		void SetCodeValues(const std::map<Variant, std::wstring>& mapValue);
		std::map<Variant, std::wstring> GetCodeValues() const;

	private:
		// ��������
		std::wstring m_strFieldName;
		// ���Ե�ԭʼ�ֶ�����
		FieldType m_nOriFieldType;
		// �Ƿ����
		bool m_bIsCoded;
		// ����ӳ���ϵ
		std::map<Variant, std::wstring> m_mapCodeValue;
	};

	class STK_API I3SIndexInfo
	{
	public:
		I3SIndexInfo();
		~I3SIndexInfo();

		// ����/��ȡҳ��
		void SetPageNum(unsigned int nNum);
		unsigned int GetPageNum() const;

		// ����/��ȡÿҳnode����
		void SetNodesPerPage(unsigned int nNum);
		unsigned int GetNodesPerPage() const;

		// ����/��ȡlod�л�����
		void SetLodSelMetricType(const std::wstring& strType);
		std::wstring GetLodSelMetricType() const;

	private:
		// ҳ��
		unsigned int m_nPageNum;
		// ÿҳnode����
		unsigned int m_nNodesPerPage;
		// lod�л�����
		std::wstring m_strLodSelMetricType;
	};

	// i3s������Ϣ
	class STK_API I3SMaterialInfo
	{
	public:
		I3SMaterialInfo();
		~I3SMaterialInfo();

		// ����/��ȡ����
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

		// ����/��ȡ�����
		void SetShininess(float fValue);
		float GetShininess() const;

		// ����/��ȡ������
		void SetAmbient(const ColorValue& color);
		ColorValue GetAmbient() const;

		// ����/��ȡɢ���
		void SetDiffuse(const ColorValue& color);
		ColorValue GetDiffuse() const;

		// ����/��ȡ�����
		void SetSpecular(const ColorValue& color);
		ColorValue GetSpecular() const;

		// ����/��ȡ�ü�ģʽ
		void SetCullMode(CullingMode nMode);
		CullingMode GetCullMode() const;

	private:
		// ����
		std::wstring m_strName;
		// ���⣬Ӱ�췢����Ĵ�С
		float m_fShininess;
		// ������
		ColorValue m_ambient;
		// ɢ���
		ColorValue m_diffuse;
		// �����
		ColorValue m_specular;
		// ��Ⱦ�����õĵĲü�ģʽ
		CullingMode m_nCullMode;
	};

	// i3s�ڵ���Ϣ
	class STK_API I3SNodeInfo
	{
	public:
		I3SNodeInfo();
		~I3SNodeInfo();

	public:
		// ����/��ȡid
		void SetId(const std::wstring& strId);
		std::wstring GetId() const;

		// ����/��ȡ��Χ��
		void SetObb(const OrientedBoundingBox& obb);
		OrientedBoundingBox GetObb() const;

		// ����/��ȡLOD�л���ֵ
		void SetRangeValue(double dValue);
		double GetRangeValue() const;

		// ����/��ȡ��������·��������
		void SetGeometries(const std::vector<std::wstring>& vecGeo);
		std::vector<std::wstring> GetGeometries() const;

		// ����/��ȡ����ͼƬ·��������
		void SetTextures(const std::vector<std::wstring>& vecTex);
		std::vector<std::wstring> GetTextures() const;

		// ����/��ȡҪ������·��������
		void SetFeatures(const std::vector<std::wstring>& vecFea);
		std::vector<std::wstring> GetFeatures() const;

		// ����/��ȡ��������·��������
		void SetAttributes(const std::vector<std::wstring>& vecAtt);
		std::vector<std::wstring> GetAttributes() const;

		// ����/��ȡ������Ϣ·��
		void SetSharedResource(const std::wstring& strRes);
		std::wstring GetSharedResource() const;

		// ����/��ȡ�ӽڵ�·��������
		void SetChildNodes(const std::vector<std::wstring>& vecNode);
		std::vector<std::wstring> GetChildNodes() const;

		// ����/��ȡ������չ���Ժ�id��ӳ���ϵ
		void SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeExtensionAndIdRelation() const;

	private:
		// id
		std::wstring m_strId;
		// ��Χ��
		OrientedBoundingBox m_obb;
		// LOD�л���ֵ
		double m_dRangeValue;
		// ��������·��������
		std::vector<std::wstring> m_vecGeometry;
		// ����ͼƬ·��������
		std::vector<std::wstring> m_vecTexture;
		// Ҫ������·��������
		std::vector<std::wstring> m_vecFeature;
		// ��������·��������
		std::vector<std::wstring> m_vecAttribute;
		// ������Ϣ·��
		std::wstring m_strSharedRes;
		// �ӽڵ�·��������
		std::vector<std::wstring> m_vecChildNode;
		// ������չ���Ժ�id��ӳ���ϵ(����Ҳ�������Ա��������Ϊ����node֮��Ķ�����չ���Բ���ȫһ��)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttExtAndId;
	};

	class STK_API I3SLayerInfo
	{
	public:
		I3SLayerInfo();
		~I3SLayerInfo();

	public:
		// ����/��ȡid
		void SetId(const std::wstring& strId);
		std::wstring GetId() const;

		// ����/��ȡͼ������
		void SetType(const std::wstring& strType);
		std::wstring GetType() const;

		// ����/��ȡ����
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

	private:
		// id
		std::wstring m_strId;
		// ͼ������
		std::wstring m_strType;
		// ����
		std::wstring m_strName;
	};
}

#endif