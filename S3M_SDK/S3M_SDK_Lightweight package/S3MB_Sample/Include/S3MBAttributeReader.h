#include "S3MBLayerInfos.h"
#include "S3mbVariant.h"
using namespace std;
class S3MBAttributeReader
{
public:
	//Read attribute sample
	static void ReadAttribute(wstring strAttPath);
	//Get all .s3md files
	static void GetFiles(const std::wstring& path, std::vector<std::wstring>& files);
};
