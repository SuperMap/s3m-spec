// UGFileParseManager.h: interface for the UGFileParseManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSEMANAGER_H__32BDBDDA_A4D5_4C75_8CBE_E24F0D816A97__INCLUDED_)
#define AFX_UGFILEPARSEMANAGER_H__32BDBDDA_A4D5_4C75_8CBE_E24F0D816A97__INCLUDED_

#include "FileParser/UGFileParseDefine.h"

namespace UGC 
{
	
class FILEPARSER_API UGFileParseManager  
{
public:
	UGFileParseManager();
	virtual ~UGFileParseManager();
	
public:	
	
	//! \brief 得到文件解析插件的数目。
	static UGint GetCount();	

	//! \brief 得到文件解析工厂定义。
	//! \param nIndex [in] 索引。
	//! \return 。
	static UGFileParseDefine* GetAt(UGint nIndex);	
	
	//! \brief 加载一个指定文件名的文件解析插件。
	//! \param strPathName [in]。
	//! \return 加载成功返回true。
	static UGbool LoadFPS(const UGString& strPathName);	
	
	//! \brief 卸载文件解析插件。
	//! \param hHandle [in] 动态库句柄。
	//! \return 成功卸载返回true。
	static UGbool UnloadFPS(void* hHandle);		
	
	//! \brief 创建一个文件解析类,需要调用DestroyFileParser销毁
	//! \param nFileType  文件类型
	//! \param bIsUserFME 是否使用FME
	//! \attention 因为二维、三维目前也在使用此函数 故bIsUserFME默认为FALSE
	static UGFileParser* CreateFileParser(UGint nFileType, UGbool bIsUserFME = FALSE);
	
	//! \brief 删除文件解析类。
	static void DestroyFileParser(UGFileParser*& pFileParser);
		
	//! \brief 得到能够支持的文件类型。
	static void GetSupportFileTypes(UGArray<UGint>& arrFileTypes);

	//! \brief 得到能够支持的文件扩展名。
	static void GetSupportFileExtNames(UGStrings& arrFileExtNames);
	
	//! \brief 根据后缀名得到文件类型。
	static UGint ExtNameToFileType(const UGString& strExtName);	
	
	//! \brief 根据文件类型得到后缀名。
	static UGString FileTypeToExtName(UGint nFileType);
	
	//! \brief 根据后缀名得到分组类型。
	static UGFileType::EmGroup ExtNameToFileGroup(const UGString& strExtName);
	
	//! \brief 根据文件类型得到分组类型。
	static UGFileType::EmGroup FileTypeToFileGroup(UGint nFileType);

	UGbool AddFileParserProvider(void* handle);
private:
	UGFileParseDefine* Check(void* hHandle);
	UGFileParseDefine* Find(UGint nFileType);
	void LoadFPS();
	void UnloadFPS();
	//! \brief 数据源互斥对象
private:
	UGbool m_bLoaded;
	UGMutex m_mutex;
	UGArray<UGFileParseDefine*> m_fileParseDefines;
};

extern FILEPARSER_API UGFileParseManager g_fileParseManager;
}

#endif // !defined(AFX_UGFILEPARSEMANAGER_H__32BDBDDA_A4D5_4C75_8CBE_E24F0D816A97__INCLUDED_)

