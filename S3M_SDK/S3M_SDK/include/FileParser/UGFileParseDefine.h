// UGFileParseDefine.h: interface for the UGFileParseDefine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGFILEPARSEDEFINE_H__D1C1742B_DF3C_4125_B515_2C1599C1E499__INCLUDED_)
#define AFX_UGFILEPARSEDEFINE_H__D1C1742B_DF3C_4125_B515_2C1599C1E499__INCLUDED_

#include "FileParser/UGFileParseFactory.h"

namespace UGC 
{

class FILEPARSER_API UGFileParseDefine  
{
public:
	UGFileParseDefine();
	UGFileParseDefine(const UGFileParseDefine &fileParseDefine);
	virtual ~UGFileParseDefine();
	
public:
	const UGFileParseDefine& operator=(const UGFileParseDefine &fileParseDefine);
	void Empty();
	
public:
	UGFileParseFactory* m_pFileParseFactory;
	UGString m_strName;
	void* m_hHandle;
};

}

#endif // !defined(AFX_UGFILEPARSEDEFINE_H__D1C1742B_DF3C_4125_B515_2C1599C1E499__INCLUDED_)

