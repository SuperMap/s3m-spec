#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>
#include <map>

#include "Skeleton.h"
#include "MemoryStream.h"
#include "Material.h"
#include "Texture.h"
#include "Shell.h"
#include "S3MBLayerInfos.h"


using namespace std;
namespace S3MB
{
	class S3MB_API S3MBReader
	{
	public:
		S3MBReader() { m_pROGroup = nullptr; } 
		~S3MBReader();

	public:
		//将读取的s3mb文件转化为RenderOperationGroup
		void ReadS3MBAsROGroup();
		//设置s3mb路径
		void SetFilePath(const wstring& strFilePath);
		//从数据流中读取s3mb
		bool LoadS3MBFromStream(void* buffer,unsigned int bufferSize);
		//输出模型信息到控制台
		void OutputGeometryInfoToConsole();
		//获取读取到的RenderOperationGroup
		RenderOperationGroup* GetRenderOperationGroup() { return m_pROGroup; };
		void Clear();

	private:
		// 配置源文件
		std::wstring m_strFilePath;

		RenderOperationGroup* m_pROGroup;
	};
}