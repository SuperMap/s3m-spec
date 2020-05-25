#include <vector>
#include <iostream>
#include <codecvt>
#include <locale>
#include "zlib/include/zconf.h"
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
	class S3MBReader
	{
	public:
		S3MBReader() {}
		~S3MBReader();

	public:
		void ReadS3MBAsROGroup();
		void ReadS3MBAttribute();

		void GetFiles(const std::string& path, std::vector<std::string>& files);
		void SetFilePath(const string& strFilePath);
		bool OpenFileS3MB();
		bool LoadFileS3MB(ifstream& stream);
		//获取读取到的RenderOperationGroup
		RenderOperationGroup* GetRenderOperationGroup() { return m_pROGroup; };
		void Clear();

	private:
		// 配置源文件
		std::string m_strFilePath;

		RenderOperationGroup* m_pROGroup;
	};
}