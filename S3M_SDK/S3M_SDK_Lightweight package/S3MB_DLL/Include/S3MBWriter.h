#pragma once
#include "StringHelper.h"
#include "Shell.h"
namespace S3MB
{
	class S3MB_API S3MBWriter
	{
	public:
		S3MBWriter()
		{
			m_bChangeTexture = true;
			m_CompressType = 14;
		}
		~S3MBWriter() {}
	public:
		void ProcessSaveInfo(int vertexCompressOptions = 0);
		//! \brief 取出PagedLOD的实体
		void FillEntityPack(RenderOperationPagedLOD* pROPagedLOD);
		//! \brief 取出Geode的实体
		void FillEntityPack(RenderOperationGeode* pROGeode);
		//! \brief 取出Geometry的材质和纹理
		void FillEntityPack(Geometry* pGeometry);
		//! \brief 取出Material的纹理
		void FillEntityPack(Material* pMaterial3D);

		bool Save(int vertexCompressOptions = 0);
		//设置需要写入文件的RenderOperationGroup
		void SetROGroup(RenderOperationGroup* pGroup) { this->m_pROGroup = pGroup; }
		//设置导出文件路径
		void SetFilePath(wstring filePath) {
			if (filePath.find(L':') == wstring::npos)
			{
				this->m_strFilePath = StringHelper::GetAbsolutePath(filePath);
			}
			else
				this->m_strFilePath = filePath;
		}
		//设置是否改变图片格式
		void SetIsChangeTexture(bool bChange) { this->m_bChangeTexture = bChange; }

		void Clear();
	private:
		// 配置源文件
		std::wstring m_strFilePath;

		RenderOperationGroup* m_pROGroup;

		//! \brief 壳儿放置到内存数据流
		MemoryStream m_streamShell;

		//! \brief 骨架放置到内存数据流
		MemoryStream m_streamGeometry;

		//! \brief SelectionInfo放置到内存数据流
		MemoryStream m_streamSelectionInfo;

		//! \brief 是否有SelectionInfo
		unsigned char m_bHasSelectionInfo;

		//! \brief 纹理放置到内存数据流
		MemoryStream m_streamTexture;

		//! \brief 所有材质放入 Json
		wstring m_strJsonMaterials;

		//! \brief 目标压缩方式与原有压缩方式不一致时，是否进行转换
		bool m_bChangeTexture;

		// 压缩纹理类型
		unsigned int m_CompressType;

		//! \brief 处理为 HashCode 作为对象名称
		std::map<wstring, Geometry*> m_mapGeometry;
		std::map<wstring, Material*> m_mapMaterial;
		std::map<wstring, TextureDataInfo*> m_mapTexture;
	};
}