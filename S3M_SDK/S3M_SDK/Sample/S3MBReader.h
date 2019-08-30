#include <vector>
#include <iostream>

#include "Toolkit/UGLogFile.h"
#include "Toolkit/UGLogFile.h"
#include "Toolkit/UGInter.h"
#include "Toolkit3D/UGJsonUtils.h"

#include "CacheBuilder3D/UGS3MBManager.h"
#include "CacheBuilder3D/UGS3MBTags.h"
#include "CacheBuilder3D/UGS3MBLayerInfos.h"

#include "FileParser/UGFileParseModel.h"
#include "FileParser/UGModelConfigParams.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGFileParseModel.h"
using namespace std;
using namespace UGC;
class S3MBReader
{
public:
	S3MBReader();
	~S3MBReader();

public:
	void ReadS3MBAsROGroup();
	void ReadS3MBAttribute();

	void GetFiles(const std::string& path,std::vector<std::string>& files);
	void SetFilePath(const UGString & strFilePath);

private:

	// ≈‰÷√‘¥Œƒº˛
	UGString m_strFilePath;
};
