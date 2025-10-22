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
		// 设置/获取节点数量
		void SetNodeCount(unsigned int nCount);
		unsigned int GetNodeCount() const;

	private:
		// 节点数量
		unsigned int m_nNodeCount;
	};

	class STK_API I3SAttStorageInfo
	{
	public:
		I3SAttStorageInfo();
		~I3SAttStorageInfo();

		// 设置/获取键值
		void SetKey(const std::wstring& strKey);
		std::wstring GetKey() const;

		// 设置/获取名称
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

	private:
		// 键值
		std::wstring m_strKey;
		// 名称
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
		// 设置/获取渲染器类型
		void SetRendererType(I3SDrawingInfo::I3SRendererType eType);
		I3SDrawingInfo::I3SRendererType GetRendererType() const;

		// 设置/获取属性名称
		void SetFieldName(const std::wstring& strName);
		std::wstring GetFieldName() const;

		// 设置/获取材质颜色
		void SetMaterialColor(const ColorValue& color);
		ColorValue GetMaterialColor() const;
		bool GetHasMaterialColor() const;

		// 设置/获取颜色混合模式
		void SetColorMixMode(I3SDrawingInfo::I3SColorMixMode eMode);
		I3SDrawingInfo::I3SColorMixMode GetColorMixMode() const;

		// 设置/获取符号类型
		void SetSymbolType(I3SDrawingInfo::I3SSymbol3DType eType);
		I3SDrawingInfo::I3SSymbol3DType GetSymbolType() const;

		// 设置/获取属性值和符号名称的关系
		void SetFieldSymbols(const std::map<Variant, std::wstring>& mapValue);
		std::map<Variant, std::wstring> GetFieldSymbols() const;

		// 设置/获取默认外挂符号的名称
		void SetDefaultSymbolRef(const std::wstring& strRef);
		std::wstring GetDefaultSymbolRef() const;

		// 设置/获取外挂文件名称、路径
		void SetSymbolPaths(const std::map<std::wstring, std::wstring>& mapPath);
		std::map<std::wstring, std::wstring> GetSymbolPaths() const;

		// 设置/获取符号名称和变换矩阵的关系
		void SetSymbolTransforms(const std::map<std::wstring, Matrix4d>& mapValue);
		std::map<std::wstring, Matrix4d> GetSymbolTransforms() const;

	private:
		// 渲染器类型
		I3SDrawingInfo::I3SRendererType m_eRendererType;
		// 属性名称（其值作为区分不同符号的特征值）
		std::wstring m_strFieldName;
		// 材质颜色
		ColorValue m_malColor;
		bool m_bHasMalColor;
		// 颜色混合模式
		I3SDrawingInfo::I3SColorMixMode m_eColorMixMode;
		// 符号类型
		I3SDrawingInfo::I3SSymbol3DType m_eSymbolType;
		// 属性值和符号名称的映射关系
		std::map<Variant, std::wstring> m_mapFieldSymbol;
		// 默认外挂符号的名称
		std::wstring m_strDefaultSymbolRef;
		// 外挂文件名称、路径
		std::map<std::wstring, std::wstring> m_mapSymbolPath;
		// 符号名称和变换矩阵的映射关系
		std::map<std::wstring, Matrix4d> m_mapSymbolTransform;
	};

	// 和S3MBFieldInfo配合使用
	class STK_API I3SAttributeInfo
	{
	public:
		I3SAttributeInfo();
		~I3SAttributeInfo();

		// 设置/获取属性名称
		void SetFieldName(const std::wstring& strName);
		std::wstring GetFieldName() const;

		// 设置/获取属性的原始字段类型
		void SetOriginFieldType(FieldType nType);
		FieldType GetOriginFieldType() const;

		// 设置/获取是否编码
		void SetIsCoded(bool bIs);
		bool GetIsCoded() const;

		// 设置/获取编码映射关系
		void SetCodeValues(const std::map<Variant, std::wstring>& mapValue);
		std::map<Variant, std::wstring> GetCodeValues() const;

	private:
		// 属性名称
		std::wstring m_strFieldName;
		// 属性的原始字段类型
		FieldType m_nOriFieldType;
		// 是否编码
		bool m_bIsCoded;
		// 编码映射关系
		std::map<Variant, std::wstring> m_mapCodeValue;
	};

	class STK_API I3SIndexInfo
	{
	public:
		I3SIndexInfo();
		~I3SIndexInfo();

		// 设置/获取页数
		void SetPageNum(unsigned int nNum);
		unsigned int GetPageNum() const;

		// 设置/获取每页node数量
		void SetNodesPerPage(unsigned int nNum);
		unsigned int GetNodesPerPage() const;

		// 设置/获取lod切换类型
		void SetLodSelMetricType(const std::wstring& strType);
		std::wstring GetLodSelMetricType() const;

	private:
		// 页数
		unsigned int m_nPageNum;
		// 每页node数量
		unsigned int m_nNodesPerPage;
		// lod切换类型
		std::wstring m_strLodSelMetricType;
	};

	// i3s材质信息
	class STK_API I3SMaterialInfo
	{
	public:
		I3SMaterialInfo();
		~I3SMaterialInfo();

		// 设置/获取名称
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

		// 设置/获取反光度
		void SetShininess(float fValue);
		float GetShininess() const;

		// 设置/获取环境光
		void SetAmbient(const ColorValue& color);
		ColorValue GetAmbient() const;

		// 设置/获取散射光
		void SetDiffuse(const ColorValue& color);
		ColorValue GetDiffuse() const;

		// 设置/获取反射光
		void SetSpecular(const ColorValue& color);
		ColorValue GetSpecular() const;

		// 设置/获取裁剪模式
		void SetCullMode(CullingMode nMode);
		CullingMode GetCullMode() const;

	private:
		// 名称
		std::wstring m_strName;
		// 发光，影响发射光点的大小
		float m_fShininess;
		// 环境光
		ColorValue m_ambient;
		// 散射光
		ColorValue m_diffuse;
		// 反射光
		ColorValue m_specular;
		// 渲染引擎用的的裁剪模式
		CullingMode m_nCullMode;
	};

	// i3s节点信息
	class STK_API I3SNodeInfo
	{
	public:
		I3SNodeInfo();
		~I3SNodeInfo();

	public:
		// 设置/获取id
		void SetId(const std::wstring& strId);
		std::wstring GetId() const;

		// 设置/获取包围盒
		void SetObb(const OrientedBoundingBox& obb);
		OrientedBoundingBox GetObb() const;

		// 设置/获取LOD切换阈值
		void SetRangeValue(double dValue);
		double GetRangeValue() const;

		// 设置/获取几何数据路径的数组
		void SetGeometries(const std::vector<std::wstring>& vecGeo);
		std::vector<std::wstring> GetGeometries() const;

		// 设置/获取纹理图片路径的数组
		void SetTextures(const std::vector<std::wstring>& vecTex);
		std::vector<std::wstring> GetTextures() const;

		// 设置/获取要素数据路径的数组
		void SetFeatures(const std::vector<std::wstring>& vecFea);
		std::vector<std::wstring> GetFeatures() const;

		// 设置/获取属性数据路径的数组
		void SetAttributes(const std::vector<std::wstring>& vecAtt);
		std::vector<std::wstring> GetAttributes() const;

		// 设置/获取共享信息路径
		void SetSharedResource(const std::wstring& strRes);
		std::wstring GetSharedResource() const;

		// 设置/获取子节点路径的数组
		void SetChildNodes(const std::vector<std::wstring>& vecNode);
		std::vector<std::wstring> GetChildNodes() const;

		// 设置/获取顶点扩展属性和id的映射关系
		void SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeExtensionAndIdRelation() const;

	private:
		// id
		std::wstring m_strId;
		// 包围盒
		OrientedBoundingBox m_obb;
		// LOD切换阈值
		double m_dRangeValue;
		// 几何数据路径的数组
		std::vector<std::wstring> m_vecGeometry;
		// 纹理图片路径的数组
		std::vector<std::wstring> m_vecTexture;
		// 要素数据路径的数组
		std::vector<std::wstring> m_vecFeature;
		// 属性数据路径的数组
		std::vector<std::wstring> m_vecAttribute;
		// 共享信息路径
		std::wstring m_strSharedRes;
		// 子节点路径的数组
		std::vector<std::wstring> m_vecChildNode;
		// 顶点扩展属性和id的映射关系(这里也有这个成员变量是因为可能node之间的顶点扩展属性不完全一样)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttExtAndId;
	};

	class STK_API I3SLayerInfo
	{
	public:
		I3SLayerInfo();
		~I3SLayerInfo();

	public:
		// 设置/获取id
		void SetId(const std::wstring& strId);
		std::wstring GetId() const;

		// 设置/获取图层类型
		void SetType(const std::wstring& strType);
		std::wstring GetType() const;

		// 设置/获取名称
		void SetName(const std::wstring& strName);
		std::wstring GetName() const;

	private:
		// id
		std::wstring m_strId;
		// 图层类型
		std::wstring m_strType;
		// 名称
		std::wstring m_strName;
	};
}

#endif