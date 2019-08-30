
#if !defined(AFX_UGFILEPARSEFACTORY_H__1885519B_9074_4676_8294_31061A9FCB47__INCLUDED_)
#define AFX_UGFILEPARSEFACTORY_H__1885519B_9074_4676_8294_31061A9FCB47__INCLUDED_

#include "FileParser/UGFileParse.h"

namespace UGC 
{

//! \brief 本类主要是创建文件格式的解析基类。
//! \remarks 所有有关创建所有文件格式的解析基类。
class FILEPARSER_API UGFileParseFactory  
{
public:
	UGFileParseFactory();
	virtual ~UGFileParseFactory();

public:
	//! \brief 创建文件解析类(根据文件类型区分)
	virtual UGFileParser* CreateFileParser(UGint nFileType) const=0;	

	//! \brief 得到能够支持的文件类型。
	virtual void GetSupportFileTypes(UGArray<UGint>& arrFileTypes) const=0;	
	
	//! \brief 得到能够支持的文件扩展名。
	virtual void GetSupportFileExtNames(UGStrings& arrFileExtNames) const=0;	
	
	//! \brief 根据后缀名得到文件类型。
	virtual UGint ExtNameToFileType(const UGString& strExtName) const=0;		
	
	//! \brief 根据文件类型得到后缀名。
	virtual UGString FileTypeToExtName(UGint nFileType) const=0;	
	
	//! \brief 得到插件名称。
	virtual UGString GetName() const=0;	
	
public:		
	//! \brief 删除文件解析类。
	virtual void DestroyFileParser(UGFileParser*& pFileParser) const;	

	//! \brief 是否支持的文件类型。
	virtual UGbool IsSupportFileType(UGint nFileType) const;		
	
	//! \brief 是否支持的文件扩展名。
	virtual UGbool IsSupportFileExtName(const UGString& strExtName) const;
	
};

} //namespace UGC


#endif // !defined(AFX_UGFILEPARSEFACTORY_H__1885519B_9074_4676_8294_31061A9FCB47__INCLUDED_)


