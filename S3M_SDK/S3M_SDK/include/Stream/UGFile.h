#ifndef UGFILE_H
#define UGFILE_H

#include "ugdefs.h"
#include <sys/stat.h>

namespace UGC {

//! \brief 和文件, 路径相关的工具类, 全部是静态函数; 使用UGFile::XXX() 的形式调用
class STREAM_API UGFile
{
public:
	/// Options for ugfilematch
	enum 
	{
		/// No wildcard can ever match `/'
		FILEMATCH_FILE_NAME   = 1, 
		/// Backslashes don't quote special chars
		FILEMATCH_NOESCAPE    = 2, 
		/// Leading `.' is matched only explicitly
		FILEMATCH_PERIOD      = 4,
        /// Ignore `/...' after a match
		FILEMATCH_LEADING_DIR = 8,
        /// Compare without regard to case
		FILEMATCH_CASEFOLD    = 16        
	};
	
	/// Options for listing files
	enum 
	{
		/// Matching files and directories
		LIST_MATCH_ALL      = 0,              
		/// Don't list any files
		LIST_NO_FILES       = 1,
		/// Don't list any directories
		LIST_NO_DIRS        = 2,
		/// List all files
		LIST_ALL_FILES      = 4, 
		/// List all directories
		LIST_ALL_DIRS       = 8,
		/// List hidden files also
		LIST_HIDDEN_FILES   = 16, 
		/// List hidden directories also
		LIST_HIDDEN_DIRS    = 32,
		/// Don't include '..' in the listing
		LIST_NO_PARENT      = 64,
		/// Matching is case-insensitive
		LIST_CASEFOLD       = 128             
	};

	// 通过读取文件得到文件类型
	enum UGFileType
	{
		// 不识别的文件
		UG_FILE_UNKNOWN = 0,
		// zip文件
		UG_FILE_ZIP = 1      // 文件头标记4bytes 0x04034b50 ‘PK~~’
	};
	
	enum UGFileMod
	{
		UG_FILE_WRITE = 0x0080,		//_S_IWRITE
		UG_FILE_READ  = 0x0100,		//_S_IREAD
		UG_FILE_READWRITE = UG_FILE_WRITE | UG_FILE_READ
	};

	// 读取文件头来确定文件类型，不同的文件使用不同的规则。
	// param strFile[in] 要被测试的文件
	// remark 目前仅支持zip格式 :(
	// 支持的格式包含:
	// 1.   ZIP
	static UGFileType GetFileType(const UGString& strFile);

	//! \brief 得到当前路径）
	//! \remarks 应用程序在 "c:/dir/dir2/process.exe"，在"c:/dir/dir2/"目录下执行，返回"c:/dir/dir2"。
	//           在"c:/dir/"目录下执行，返回"c:/dir"。
	//! \return 返回路径
	static UGString GetCurrentPath();
	//! \brief 从文件全路径中得到路径信息
	//! \param strPathFile 文件全路径（带文件全名）
	//! \remarks "c:/dir/dir2/a.ext"返回"c:/dir/dir2/"
	//! \return 返回路径
	static UGString GetDir(const UGString& strPathFile);

	//! \brief 从文件全路径中得到文件名
	//! \param strPathFile 文件全路径（带文件全名）
	//! \remarks "c:/dir/dir2/a.ext"返回"a.ext"
	//! \return 返回文件名
	static UGString GetName(const UGString& strPathFile);

	//! \brief 从文件全路径中得到文件主题名(不带后缀名)
	//! \param strPathFile 文件全路径（带文件全名）
	//! \remarks "c:/dir/dir2/a.ext"返回a
	//! \return 返回文件主题名
	static UGString GetTitle(const UGString& strPathFile);

	//  [7/14/2009 linwenyu]
	//! \brief 判断文件名称是否合法
	//! \param strFileName 文件名
	//! \remarks "abc\/:*?"<>|"返回FALSE
	//! \return 包含"\/:*?"<>|"为不合法文件名
	static UGbool IsFileNameValid(const UGString& strFileName);

	//! \brief 从文件全路径中得到后缀名
	//! \param strPathFile 文件全路径（带文件全名）
	//! \remarks "c:/dir/dir2/a.ext"返回".ext"(注意:带".")
	//! \return 返回文件后缀名
	static UGString GetExt(const UGString& strPathFile);

	//! \brief 从文件全路径中得到去掉后缀名之后的全路径
	//! \param strPathFile 文件全路径（带文件全名）
	//! \remarks "c:/dir/dir2/a.ext"返回"c:/dir/dir2/a"
	//! \return 返回去掉后缀名之后的全路径
	static UGString StripExt(const UGString& strPathFile);

	//! \brief 把文件全路径分解为若干部分
	//! \param strPathFile 文件全路径（带文件全名）[in]
	//! \param strDir 路径名(win32下带有驱动名) [out]
	//! \param strTitle 文件主题 [out]
	//! \param strExt 文件后缀名 [out]
	//! \remarks 传入"c:/dir/dir2/a.ext",分别得到"c:/dir/dir2/", "a", ".ext"
	static void SplitPath(const UGString& strPathFile, UGString& strDir, UGString& strTitle, UGString& strExt);

	//! \brief 改变后缀名
	//! \param strFileName 原文件名
	//! \param strNewExt 新的后缀名, 是否有"."均可,函数内会处理好
	//! \return 返回新的文件名
	static UGString ChangeExt(const UGString &strFileName, const UGString &strNewExt);

	//! \brief 得到临时路径
	//! \return 返回临时路径
	static UGString GetTemporaryPath();

	//! \brief 得到临时文件名
	//! \param strPath 指定临时文件所在的目录
	//! \param strPrefixString 指定临时文件的前缀
	//! \param uUnique 指定用来进行唯一标示的数字
	//! \param strTempFileName 得到临时文件名
	//! \return 返回临时文件名
	/*
	Specifies an unsigned integer that the function converts to a hexadecimal string for use in creating the temporary filename. 
	If uUnique is nonzero, the function appends the hexadecimal string to lpPrefixString to form the temporary filename. In this case, the function does not create the specified file, and does not test whether the filename is unique. 

	If uUnique is zero, the function uses a hexadecimal string derived from the current system time. In this case, the function uses different values until it finds a unique filename, and then it creates the file in the lpPathName directory. 
	*/
	static UGuint GetTemporaryFileName(const UGString &strPath, const UGString &strPrefixString, 
									UGuint uUnique, UGString &strTempFileName);

	//! \brief 得到当前执行程序所在的路径
	//! \remarks 当前执行程序的路径有可能不和UGC动态库目录一致, 
	//!			 例如c:\tmp\a.exe 调用 d:\ugc\bin\ 中的 ugc动态库, 
	//!			 GetAppPath() 返回c:\tmp\, GetModulePath("SuBase")返回d:\ugc\bin\
	//! \return 返回当前执行程序所在的路径
	static UGString GetAppPath();

	//! \brief 得到指定UGC模块(动态库)所在的路径
	//! \param strModuleTitle 模块的名称, 注意, 不要带版本号和后缀名,函数内部会处理
	//! \remarks 举例: UGFile::GetModulePath("SuBase") 得到 Base dll所在的路径
	//!			 需要得到UGC工程文件(如dsp文件等)的路径,请用 GetModulePath + 相对路径构造,
	//!			 具体可以参考 GetUnitTestDataPath() 中的代码实现
	//! \return 返回路径
	static UGString GetModulePath(const UGString& strModuleTitle);

	//! \brief 得到单元测试数据所在的目录
	//! \param strUnitTestName 单元测试工程的文件夹名，如TestBase、TestGeometry等
	//! \remarks 返回的是绝对路径，可直接使用；采用的是ugcpath/01_SourceCode/TestUnit/strUnitTestName/Data/ 的形式
	//!			 这样就要求各个单元测试工程的数据放在本测试工程目录下一级的Data目录下; 调用时,把strUnitTestName作为参数传入
	//! \return 返回单元测试数据所在的目录
	static UGString GetUnitTestDataPath(const UGString& strUnitTestName);

	//! \brief 得到绝对路径
	//! \remarks 三种路径的定义: 原路径(SrcPath)+相对路径(RelPath) == 绝对路径(AbsPath)
	//! 举例说明: SrcPath="d:/dir/", RelPath="./new/",  则AbsPath="d:/dir/new/"
	//! 			Source Path			Relative Path			Absolute Path			
	//! 			/a/b/c/d/e/f/g		h/i/j/k				/a/b/c/d/e/f/g/h/i/j/k/
	//! 			/a/b/c/d/e/f/g/		./h/i/j/k			/a/b/c/d/e/f/g/h/i/j/k/
	//! 			/a/b/c/d/e/f/g/		../h/i/j/k			/a/b/c/d/e/f/h/i/j/k/
	//! 			/a/b/c/d/e/f/g/		../../h/i/j/k		/a/b/c/d/e/h/i/j/k/
	//! 			/a/b/c/d/e/f/g		/h/i/j/k			/h/i/j/k/  ?
	//! 当strSrcPath为空的时候, 如果strRelPath为./abc.txt , 则返回abc.txt ;
	//! 当strSrcPath为空的时候, 如果strRelPath为../abc.txt , 则返回空字符串.
	//! \param strSrcPath 原始路径
	//! \param strRelPath 相对路径, 可带有文件名
	//! \return 返回构造后的绝对路径(可带有文件名,随strRelPath而定)
	static UGString GetAbsolutePath(const UGString &strSrcPath, const UGString &strRelPath);
	
	//! \brief 得到相对路径
	//! \param strSrcPath 原路径
	//! \param strAbsPath 绝对路径
	//! \remarks 三种路径的定义: 原路径(SrcPath)+相对路径(RelPath) == 绝对路径(AbsPath)
	//! 即 RelPath = AbsPath - SrcPath
	//! 举例说明: SrcPath="d:/dir/", AbsPath="d:/dir/path/",  则 RelPath="./path/"
	//! 参数和返回值都只能是只包含目录的路径信息, 并不包含任何文件名.
	//! \return 返回相对路径
	static UGString GetRelativePath(UGString strSrcPath, UGString strAbsPath);

	
	//! \brief 在指定的目录中查找指定后缀名的文件,并放到一个集合中
	//! \param strPath 要查找的目录 [in]
	//! \param strExtName 指定的后缀名, 是否带有'.'均可 [in]
	//! \param strFileNames 用来输出的文件名集合 [out]
	//! \param bAddPath 用来指定在输出的文件名集合中,是否添加路径信息
	static UGbool FindFileExtPaths(const UGString& strPath, const UGString& strExtName, 
								  UGArray<UGString>& strFileNames, UGbool bAddPath);

	//static UGbool IsFileExisted(const UGString& strFileName);
	// modified by zengzm 2007-4-2 CopyFileEx这个名字又被MFC给改名为CopyFileExA了，因此，在UGC中只好换一个名字，在前面加UG
	//! \brief 拷贝文件
	//! \param strExistedFileName 已经存在的文件
	//! \param strNewFileName 新的文件名
	//! \param bFailIfExists 如果存在, 是否不覆盖
	static UGbool Copy(const UGString& strExistedFileName, const UGString& strNewFileName, UGbool bFailIfExists);

	
	/// Match filenames using *, ?, [^a-z], and so on
	//! \brief 判断文件名是否符合一定的规则(模式)
	//! \param strPattern 规定的模式
	//! \param strFileName 要判断的文件名
	//! \param nFlags 判断的一些处理方式
	//! \return 文件名符合规则返回true,否则返回false
	//! \remarks 例如: abc符合a*c 的规则,但不符合c*a 的规则
	static UGbool Match(const UGString& strPattern, const UGString& strFileName, 
					UGuint nFlags=(FILEMATCH_NOESCAPE|FILEMATCH_FILE_NAME));
	
	//! \brief 从某个目录中查找符合给定规则的所有文件
	//! \param strFileList 得到的符合给定规则的文件名数组 [out]
	//! \param strPath 要查找的目录 [in]
	//! \param strPattern 给定的规则 [in]
	//! \param nFlags 判断的一些处理方式 [in]
	//! \return 返回找到符合规则的文件个数
	//! \remarks 对于通常的应用(在某个目录中查找指定后缀名的文件名),使用FindFileExtPaths就可以了; 
	//!			ListFiles功能更加强大,但是从fox库移植过来,出问题不好维护.
	//!			给出的strFileList中没有带有路径信息
	static UGint ListFiles(UGArray<UGString>& strFileList,const UGString& strPath,
					const UGString& strPattern=_U("*"),UGuint nFlags=LIST_MATCH_ALL);
	
	//! \brief 判断文件或目录是否存在
	//! \param strFileDir 要判断的文件或目录
	//! \remarks IsExist支持文件或目录是否存在的判断, 如果是目录,请用"/"或"\\"作为最后一个字符
	//! \return 存在返回true,不存在返回false
	static UGbool IsExist(const UGString& strFileDir);
	
	// added by zengzm 2007-4-4 得到文件的创建,访问和最后更新时间
	//! \brief 得到文件创建时间
	static UGTime GetCreateTime(const UGString& strFileName);
	
	//! \brief 得到文件访问时间
	static UGTime GetAccessTime(const UGString& strFileName);
	
	//! \brief 得到文件最后更新时间
	static UGTime GetUpdateTime(const UGString& strFileName);

	//! \brief 得到文件的模式
	static UGushort getMode(const UGString & strFileName);
	
	static UGbool IsReadOnly(UGString strFile);

	//! \brief 修改文件读写权限
	//! \param &strName 磁盘文件的路径[in]。
	//! \param flag 转化成磁盘文件的属性[in]。
	static UGbool changeMode(UGString strFile,UGFileMod mod);

	//! \brief 得到文件属性 仅在win32 平台上有效
    static UGuint  getAttribute(const UGString & strFileName);
	
	//! \brief 文件状态信息结构体,目前根据实际需要,仅包括创建,访问和最后更新时间的信息,有其它需要再添加
	struct UGFileStatus 
	{
		UGFileStatus(){
			m_nSize = 0;
		}
		//! \brief 文件创建时间
		UGTime m_tmCreate;
		//! \brief 文件访问时间
		UGTime m_tmAccess;
		//! \brief 文件更新时间
		UGTime m_tmUpdate;
		//! \brief 文件大小（字节）
		UGulong m_nSize;
		//! \brief 文件的模式(属性)
		UGushort m_nMode;
	};
	
	//! \brief 得到文件的状态信息。
	//! \param strFileName 指定的文件名[in]。
	//! \param status 得到的状态信息[out]。
	//! \return 成功返回true,失败返回false。
	
	static UGbool GetStatus(const UGString& strFileName, UGFileStatus& status);

	#ifndef SYMBIAN60
	static UGbool GetStatus64(const UGString& strFileName, UGFileStatus& status);
	#endif

	//! \brief 创建文件夹
	//! \param strDir 要创建的文件夹名字
	//! \return 成功返回true;失败返回false
	//! \remarks 这个只是创建文件夹，如果文件夹所在路径不存在，不会创建。
	//! 如果需要在不存的路径上创建多级文件夹，请使用MkDirEx()函数
	static UGbool MkDir(const UGString& strDir);

	//! \brief 一次性创建多级文件夹
	//! \param strDir 要创建的文件夹名字
	//! \return 成功返回true;失败返回false
	static UGbool MkDirEx(const UGString& strDir);

	//! \brief 删除文件夹
	//! \param strDir 要删除的文件夹名字
	//! \return 成功返回true;失败返回false
	static UGbool RmDir(const UGString& strDir);

	//! \brief 递归删除文件夹
	//! \param strDir 要删除的文件夹名字
	//! \return TRUE 成功
	//! \return FALSE 失败
	//! \remark 删除文件夹，包括文件夹里的文件和子文件夹
	static UGbool RmDirEx(UGString strDir);

	//! \brief 递归拷贝文件夹
	//! \param strExistedDir 已经存在的文件夹名字
	//! \param strNewDir 新的文件夹名字
	//! \param bFailIfExists 如果存在, 是否不覆盖
	//! \remark 拷贝文件夹，包括文件夹里的文件和子文件夹
	static UGbool CopyDirEx(UGString strExistedDir, UGString strNewDir, UGbool bFailIfExists = FALSE);
	
	//! \brief 删除文件
	//! \param strFileName 要删除的文件名（带路径）
	//! \return 成功返回true；失败返回false
	static UGbool Delete(const UGString& strFileName);
	
	//! \brief 文件重命名
	//! \param strOldName 现有文件的文件名
	//! \param strNewName 重命名后的文件名
	//! \return 成功返回true；失败返回false
	static UGbool Rename(const UGString& strOldName, const UGString& strNewName);

#ifndef SYMBIAN60
	//! \brief 以指定位置将文件截断
	//! \param pFile 文件指针
	//! \param nPositon 文件截断位置
	//! \return 成功返回true；失败返回false
	static UGbool FileTruncate(void * pFileHandle, UGulong nPositon);
#endif
	//! \brief 根据两个不同的文件名判断是否是同一个文件。
	//! \param strName1 [in]文件名。
	//! \param strName2 [in]文件名。
	//! \return 成功返回true；失败返回false
	static UGbool JudgeSameFile(const UGString& strName1, const UGString& strName2);
#if defined IOS 
    static UGString& GetIOSTemporaryPath();
    static void SetIOSTemporaryPath(UGString& temporaryPath);
	static UGString& GetIOSDocumentPath();
	static void SetIOSDocumentPath(UGString& documentPath);
#endif

#ifdef OS_ANDROID
	static UGString GetAndroidTemporaryPath();
	static void SetAndroidTemporaryPath(UGString tempPath);

	/**
	 * 获取和设置应用的缓存目录，这个目录是应用自身的，不需要申明读写权限;
	 * 在初始化时，从组件设置，用于解决sqlite3不能获取临时目录的问题，或者类似的
	 * 而应用外的目录Sqlite3是不可用的
	 * Added by Xingjun, 2016.06.20
	 */
	static UGString GetAndroidAppCachePath();
	static void SetAndroidAppCachePath(UGString cachePath);

#endif
private:
	static UGint ugfilematch(const UGchar *pattern,const UGchar *str,UGuint flags=(FILEMATCH_NOESCAPE|FILEMATCH_FILE_NAME));
	static UGint DoMatch(const UGchar *pattern,const UGchar *str,UGuint flags);

	//! \brief 预处理目录。
	//! \param strPath [in] 要预处理的目录。
	//! \param bIsFile [in] 是否带有文件名。
	//! \return 处理后的目录。
	//! \remarks 处理的内容包括：删除左右的空格；把"\\"替换为"/"，以便各个平台都能认识；
	//!		把"//"替换为"\\\\"，因为网络路径只认识这个; 如果不带有文件名,则还要确保最后一个字符是"/"
	static UGString PreDealPath(UGString strPath, UGbool bIsFile);	

	//! \brief 判断文件是否存在, 分平台来实现
	//! \remarks 供IsExist在内部调用, 实现最小功能分平台实现, IsExist做到和平台无关
	static UGbool InnerIsExist(const UGString& strFileDir);
};

} //namespace UGC 

#endif

