#if !defined(S3MBWRITER_H__DEC17B34_A38E_4EF0_8CB2_419D83CB01BD__INCLUDED_)
#define S3MBWRITER_H__DEC17B34_A38E_4EF0_8CB2_419D83CB01BD__INCLUDED_

#pragma once
#include "Shell.h"
#include "S3MBTools.h"

namespace S3MB
{
	class S3MB_API S3MBWriter
	{
	public:
		S3MBWriter();
		~S3MBWriter();

	public:
		// 处理实体信息并保存进流中
		void ProcessEntities();
		// 取出PagedLOD的实体
		void FillEntityPack(RenderOperationPagedLOD* pROPagedLOD);
		// 取出Geode的实体
		void FillEntityPack(RenderOperationGeode* pROGeode);
		// 取出Skeleton的材质和纹理
		void FillEntityPack(Skeleton* pSkeleton);
		// 取出Material的纹理
		void FillEntityPack(Material* pMaterial3D);
		// 保存文件到本地
		bool WriteFile(const wstring& filePath);
		// 设置需要写入文件的RenderOperationGroup
		void SetROGroup(RenderOperationGroup* pGroup);
		// 设置是否改变图片格式,以及目标图片格式是否建mipmap
		void SetIsChangeTexture(bool bChange, bool bGenerateMipMap = true);
		// 设置扩展信息
		void SetExtensions(const std::wstring& strExtensions);
		// 设置Skeleton压缩参数
		void SetSkeletonCompressParam(SkeletonCompressParam pParam);
		void Clear();

	private:
		// 配置源文件
		std::wstring m_strFilePath;

		RenderOperationGroup* m_pROGroup;
		SkeletonCompressParam m_SkeletonCompressParam;

		// 壳儿放置到内存数据流
		MemoryStream m_streamShell;

		// 骨架放置到内存数据流
		MemoryStream m_streamSkeleton;

		// IDInfo放置到内存数据流
		MemoryStream m_streamIDInfo;

		// 是否有IDInfo
		unsigned char m_bHasIDInfo;

		// 纹理放置到内存数据流
		MemoryStream m_streamTexture;

		// 所有材质放入 Json
		wstring m_strJsonMaterials;

		// 目标压缩方式与原有压缩方式不一致时，是否进行转换
		bool m_bChangeTexture;

		// 压缩纹理时是否创建mipmap
		bool m_bGenerateMipMap;

		// 目标压缩纹理类型
		unsigned int m_CompressType;
		// 版本
		float m_fVersion;

		std::map<wstring, Skeleton*> m_mapSkeleton;
		std::map<wstring, Material*> m_mapMaterial;
		std::map<wstring, TextureDataInfo*> m_mapTexture;

		// 扩展信息（JSON格式）
		std::wstring m_strExtensions;
	};
}

#endif