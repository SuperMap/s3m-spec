#if !defined(S3MBREADER_H__163B95BB_0AD1_4574_B93D_E66CC3D82755__INCLUDED_)
#define S3MBREADER_H__163B95BB_0AD1_4574_B93D_E66CC3D82755__INCLUDED_

#pragma once
#include "Shell.h"

namespace S3MB
{
	class S3MB_API S3MBReader
	{
	public:
		S3MBReader();
		~S3MBReader();

	public:
		// 将读取的s3mb文件转化为RenderOperationGroup
		void ReadFile(const wstring& strFilePath);
		// 从数据流中读取s3mb
		bool LoadS3MBFromStream(void* buffer, unsigned int bufferSize);
		// 输出模型信息到控制台
		void OutputSkeletonInfoToConsole();
		// 获取读取到的RenderOperationGroup
		RenderOperationGroup* GetRenderOperationGroup();
		// 获取扩展信息
		std::wstring GetExtensions() const;
		void Clear();

	private:
		// 配置源文件
		std::wstring m_strFilePath;

		RenderOperationGroup* m_pROGroup;
		// 扩展信息（JSON格式）
		std::wstring m_strExtensions;
	};
}

#endif