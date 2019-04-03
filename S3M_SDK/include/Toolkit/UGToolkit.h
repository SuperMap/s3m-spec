// UGToolkit.h: interface for the UGToolkit class.
//
//////////////////////////////////////////////////////////////////////

#ifndef UGTOOLKIT_H
#define UGTOOLKIT_H

#include "Stream/ugdefs.h"
#include "Toolkit/UGTextCodec.h"
#include "Stream/ugplatform.h"
#include "Toolkit/UGThread.h"

#if defined(_AIX)
extern "C" int mt__trce (int  FileDescriptor, int Signal, struct sigcontext *Context, int Node);
#endif


namespace UGC{

// 全局锁，用来控制应用层把主线程拆分到多个子线程里面操作的情况导致的乱图和死锁
extern TOOLKIT_API UGMutex g_MutexToolkit;


class TOOLKIT_API UGToolkit  
{
private:
	UGToolkit();
	~UGToolkit();

public:

	//##ModelId=482030520203
	enum InvalidNameState
	{
		//##ModelId=482030520205
		nsIsEmpty,			//名称为空
		//##ModelId=482030520213
		nsBeyondLimit,		//长度超过30个字节
		//##ModelId=482030520214
		nsInvalidChar,		//有非法字符
		//##ModelId=482030520215
		nsHasAgainName,		//有重名
		//##ModelId=482030520222
		nsPrefixIsSM,		//前两个字符是"sm"
		//##ModelId=482030520223
		nsPrefixError,		//第一个字符为数字或下划线
		//##ModelId=482030520232
		nsAgainsystemName	//与系统名称冲突
	};	//数据集或属性字段命名错误类型


public:
	//! \brief 精确计算Cos值
	//! \remarks 主要用来处理当角度为某些特殊值时,可以直接返回精确的值
	//!			由于PI的不精确性, 用cos(弧度)得到的结果会有微小的误差, 尤其在应该为0等精确值时,
	//!			本函数输入参数为角度值,判断了这种情况, 会给出精确的值.
	//!			未来可以考虑内部缓存一些常用值,以提高效率
	//! \param dAngle 角度值
	//! \return cos的计算值
	//! 效率较低, 暂不提供
	//static UGdouble CosAngle(UGdouble dAngle);

	//! \brief 精确计算Sin值
	//! \remarks 主要用来处理当角度为某些特殊值时,可以直接返回精确的值
	//!			由于PI的不精确性, 用sin(弧度)得到的结果会有微小的误差, 尤其在应该为0等精确值时,
	//!			本函数输入参数为角度值,判断了这种情况, 会给出精确的值.
	//!			未来可以考虑内部缓存一些常用值,以提高效率
	//! \param dAngle 角度值
	//! \return sin的计算值
	//! 效率较低, 暂不提供
	// static UGdouble SinAngle(UGdouble dAngle);

	  //!  \brief Toolkit
	static UGint RoundLong(UGdouble dValue)
	{
	  //!  \brief Toolkit
		if(dValue>0) return UGint(dValue+0.5);
	  //!  \brief Toolkit
		else return UGint(dValue-0.5);
	};

	//! \brief 得到当前进程的id
	static UGint GetCurProcessId();

	static UGString GetMachineName();
// by zengzm 2007-7-13 和路径相关的函数都放到 UGFile中
	//! \attention 原来SuperMap50中Ref和Source的概念正好颠倒了;
	//! 但为了保持代码兼容,这里只修改变量名称, 不调整顺序, 使用者代码不需要改动
	//! 参数strSourcePath为绝对路径名(允许为空), 不带文件名; 参数strRefPath为文件名, 
	//! 可带相对路径或绝对路径信息;返回值为带绝对路径信息的文件名.
	//! 当strSourcePath为空的时候, 如果strRefPath为./abc.txt , 则返回abc.txt ;
	//! 当strSourcePath为空的时候, 如果strRefPath为../abc.txt , 则返回空字符串.
//	static UGString GetAbsolutePath(const UGString &strSrcPath, const UGString &strFile);
	// static UGString GetAbsolutePath(UGString strRefPath, UGString strSourcePath);

	//! 得到相对路径
	//! \remarks 三种路径的定义: 原路径(SourcePath)+相对路径(RefPath) == 绝对路径(AbsPath)
	//! 即 RefPath = AbsPath - SourcePath
	//! 举例说明: SourcePath="d:\\dir\\", AbsPath="d:\\dir\\path\\",  则 RefPath=".\\path\\"
	//! 参数和返回值都只能是只包含目录的路径信息, 并不包含任何文件名.
	//! \attention 原来SuperMap50中概念是不正确的
	//! 但为了保持代码兼容,这里只修改变量名称, 不调整顺序, 使用者代码不需要改动
//	static UGString GetRelativePath(UGString strSourcePath, UGString strAbsPath);
	// static UGString GetRelativePath(UGString strRefPath, UGString strSourcePath);


	  //!  \brief Toolkit
//	static void SplitPath(const UGString& strPath, UGString& strDrive, UGString& strDir, UGString& strFileName, UGString& strExtName);
	  //!  \brief Toolkit
//	static UGString ChangeExtName(const UGString &strFileName, const UGString &strExtName);
	  //!  \brief Toolkit
//	static UGString GetExtName(const UGString &strFileName); 

	// by zengzm 2005.6.2 这个函数没人用, 暂时封起来, 谁要用再说; 增加请加入到UGFile类中
	  //!  \brief Toolkit
	// static UGbool IsSameFile(UGString strFile1, UGString strFile2);
	  //!  \brief Toolkit
	// static UGString SetDefaultExtName(const UGString& strFileName, const UGString& strExtName);
	
	// modified by jifang 06/16/2005 把其中的UGchar *用UGString&替换
	  //!  \brief Toolkit
	//static UGuint GetTemporaryPath(UGuint nBufferLength, UGchar* lpBuffer);
	  //!  \brief Toolkit
//	static UGString GetTemporaryPath();
	  //!  \brief Toolkit
//	//static UGuint GetTemporaryFileName(UGchar* lpPathName, UGchar* lpPrefixString,UGuint uUnique,UGchar* lpTempFileName);
	  //!  \brief Toolkit
//	static UGuint GetTemporaryFileName(const UGString &strPathName, const UGString &strPrefixString, 
//									UGuint uUnique, UGString &strTempFileName);

	//{{ 动态库处理函数======================================================
	  //!  \brief Toolkit
	static void* LoadDll(const UGString& strLibName);
	  //!  \brief Toolkit
	// 通过库的句柄和函数名字,得到函数指针
	  //!  \brief Toolkit
	static void* GetProcAddress(void* hModule, const UGString& strProName);
	// 释放库
	  //!  \brief Toolkit
	static UGbool FreeDll(void* hModule);
	//}} 动态库处理函数======================================================

	//! 得到系统路径
	  //!  \brief Toolkit
	static UGbool GetSystemPath(UGStrings& arrPath);
	//! 得到绝对路径名，会遍历系统路径。首先找exe所在目录。找不到返回空字符串。
	  //!  \brief Toolkit
	static UGString GetAbsolutePathName(const UGString& strFileName);
	
	static OgdcString GetFontsPath();

    static UGbool GetPrintWMSLog();
    static void SetPrintWMSLog(UGbool bPrint);
// by zengzm 2007-7-13 和文件, 路径相关的函数都放到UGFile中了
//	static UGString GetAppPath();
//	static UGString GetModulePath(const UGchar* p);
//
//	//! \brief 得到单元测试数据所在的目录
//	//! \param strUnitTestName 单元测试工程的文件夹名，如TestBase、TestGeometry等
//	//! \remarks 返回的是绝对路径，可直接使用；采用的是ugcpath/01_SourceCode/TestUnit/strUnitTestName/Data/ 的形式
//	//!			 这样就要求各个单元测试工程的数据放在本测试工程目录下一级的Data目录下; 调用时,把strUnitTestName作为参数传入
//	//! \return 返回单元测试数据所在的目录
	  //!  \brief Toolkit
//	static UGString GetUnitTestDataPath(const UGString& strUnitTestName);

	  //!  \brief Toolkit
//	static UGbool FindFileExtPaths(const UGString& strPath, const UGString& strExtName, 
//								  UGArray<UGString>& strFileNames, UGbool bAddPath);
//
	  //!  \brief Toolkit
//	static UGbool DeleteFileEx(const UGString& strFileName);
	  //!  \brief Toolkit
//	static UGbool IsFileExisted(const UGString& strFileName);
	  //!  \brief Toolkit
//	// modified by zengzm 2007-4-2 CopyFileEx这个名字又被MFC给改名为CopyFileExA了，因此，在UGC中只好换一个名字，在前面加UG
	  //!  \brief Toolkit
//	// static UGbool CopyFileEx(const UGString& strExistedFileName, const UGString& strNewFileName, UGbool bFailIfExists);
	  //!  \brief Toolkit
//	static UGbool UGCopyFile(const UGString& strExistedFileName, const UGString& strNewFileName, UGbool bFailIfExists);

	//! \brief 得到当前系统默认的字符集
	static UGMBString::Charset GetCurCharset();

	//通过系统函数获取屏幕大小，X11上获取比较慢，这个函数调用要小心，推荐调用下面一个函数 add by xll 2006.12.30
	  //!  \brief Toolkit
	static UGuint GetScreenSize(UGuint nScreen);


	//! \brief 提供rgb转换为hsb的方法。
	//! \param red [in]。
	//! \param green [in]。
	//! \param blue [in]。
	//! \param &hsb [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static void RGB2HSB(UGint red,UGint green,UGint blue,UGArray<UGdouble> &hsb);

	static void RGB2HSB(UGint& red,UGint& green,UGint& blue, UGdouble& hue, UGdouble& saturation, UGdouble& brightness);

	//! \brief 提供hsb转换为rgb的方法。
	//! \param hue [in]。
	//! \param saturation [in]。
	//! \param brightness [in]。
	//! \param &rgbColor [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static void HSB2RGB(UGdouble hue,UGdouble saturation,UGdouble brightness,UGColor &rgbColor);

	static void HSB2RGB(UGdouble& hue,UGdouble& saturation,UGdouble& brightness, UGint& red, UGint& green, UGint& blue);

	// 创建guid
	static UGString CreateGuid();

#if defined OS_ANDROID || defined (IOS)
	// 在android中屏幕的大小有用户来设置，提供一个设置的方法
	static void setScreenSize(UGint width,UGint height);
#endif
	//获取屏幕大小，为了节省效率（X11上获取比较慢），每次ondraw前从系统获取值写入到成员变量中
	//后面要使用的时候就直接取成员变量中的值 add by xll 2006.12.30
	  //!  \brief Toolkit
	static UGuint GetScreenSizeFromMemory(UGuint nScreen);

#ifdef OS_ANDROID
	static UGString GetAndroidConfigDir();
	static void SetAndroidConfigDir(UGString configDir);
	static void SetAndroidFontsPath(UGString configDir);
	static UGString GetAndroidWebCacheDir();
	static UGString GetAndroidFontsPath();
	static void SetAndroidWebCacheDir(UGString webCacheDir);
	static void SetDebugMode(UGbool isDebug);
	static UGbool IsDebugMode();
	static void SetAndroidDPI(UGint dpi);
	static UGint GetAndroidResolution();
#else
# if defined IOS 
	static UGString& GetIOSConfigDir();
	static void SetIOSPath(UGString& configDir,UGString& temporaryPath,UGString& documentPath);
    static void SetIOSFontsPath(UGString path);
    static UGString GetIOSFontsPath();
  
#endif
#endif

//以下四个接口，主要是给移动平台使用！！！Win32上也用到，用于移动调试
	//设置显示设备屏幕密度
	static void SetDisplayDensity(UGdouble value);
	static UGdouble GetDisplayDensity();
	//设置显示设备屏幕密度
	static void SetScaledDisplayDensity(UGdouble value);//文字比例
	static UGdouble GetScaledDisplayDensity();

	//! \brief 内存信息结构
	struct MemoryStatus
	{
	  //!  \brief Toolkit
		MemoryStatus(){memset(this, 0, sizeof(MemoryStatus));}
		//! \brief 可用物理内存
		UGulong m_nAvailablePhysical;
		//! \brief 所有物理内存
		UGulong m_nTotalPhysical;
		//! \brief 可用虚拟内存
		UGulong m_nAvailableVirtual;
		//! \brief 所有虚拟内存
		UGulong m_nTotalVirtual;
		//! \brief 内存的占有率
		UGulong m_nMemoryLoad;
		//! \brief 当前进程可用内存大小(等于min(可用物理内存+可用虚拟内存,进程剩余空间)
		//! \remarks 进程空间 和 程序为23/64位相关,32位为2GB,64位则非常大了
		//! \attention 尚未实现, 非Windows平台尚未找到实现方法
		//UGulong m_nProcessAvailable = 5,
		//! \brief 当前进程全部内存大小(等于min(所有物理内存+所有虚拟内存,进程空间)
		//! \remarks 进程空间 和 程序为23/64位相关,32位为2GB,64位则非常大了
		//! \attention 尚未实现, 非Windows平台尚未找到实现方法
		//UGulong m_nProcessTotal = 6
	};

	struct ProcesMemoryStatus 
	{
		//!\brief 当前物理内存分配大小, 单位Kb
		UGulong m_nWorkingSetSize;
		//!\brief 当前虚拟内存分配大小,单位Kb
		UGulong m_nQuotaPagedPoolUsage;
	};

	//! \brief 得到系统内存状态, 单位均为KB
	//! \param memoryStatus 通过引用返回内存的各种信息[out]
	//! \return 返回查询得到的内存状态
#ifndef SYMBIAN60
	static UGbool GetMemoryStatus(MemoryStatus& memoryStatus);

	//! \brief 得到系统中空闲(可用)物理内存的大小(KB为单位)
	//! \return 返回系统中空闲(可用)物理内存的大小(KB为单位)
	//! \remarks 分平台实现,Windows上有API:GlobalMemoryStatus;Linux/Unix上用sysconf函数
	//! \attention 本函数已经被GetMemoryStatus替代, 等价于 GetMemoryStatus(AvailablePhysical); 
	//! 为了保持以前的程序可用,暂时保存;新的代码请直接使用 GetMemoryStatus
	static UGulong GetAvailablePhysicalMemory();
#endif
	
#ifdef WIN32
#if _MSC_VER> 1200
	//!\brief 获取当前进程的内存使用情况
	static UGbool GetCurrentProcMemoryStatus(ProcesMemoryStatus& procesMemory);
#endif
#endif

#if defined IOS
	//获取IOS设备当前的内存状况
	static UGbool GetIOSMemoryInfo(MemoryStatus &status);
	//获取IOS设备当前的串号
    static UGString GetIOSUUID();
    static UGint GetIOSResolution();
#endif
#if defined (OPENGL_ES_VERSION2)
    static UGint GetResolution();
#endif
	static UGuint GetCPUCount();

	//! \brief 得到系统中全部物理内存的大小(KB为单位)
	//! \return 返回系统中全部物理内存的大小(KB为单位)
	//! \remarks 分平台实现,Windows上有API:GlobalMemoryStatus;Linux/Unix上用sysconf函数
	//! \sa GetAvailablePhysicalMemory
	// 暂不提供
	//static UGuint GetPhysicalMemory();
	
	//! \brief 调整角度值,使之在 [0, 360)之间
	//! \param dAngle 要被调整的角度[in/out]
	static void AdjustAngle(UGdouble& dAngle);

	//! \brief 调整弧度值,使之在 [0, 2*PI)之间
	//! \param dRadian 要被调整的弧度[in/out]
	static void AdjustRadian(UGdouble& dRadian);

	//! \brief 计算X方向的长度在旋转一定角度后，又被resize，之后应该的长度
	//! \remarks 主要用在各种带有旋转角度的Geometry的Resize函数中对X方向的长度的处理
	//! \param dXLength X方向的长度[in]。
	//! \param dCos 该长度具备的旋转弧度的cos值[in]。
	//! \param dSin 该长度具备的旋转弧度的sin值[in]。
	//! \param dXRatio X方向的缩放比例[in]。
	//! \param dYRatio Y方向的缩放比例[in]。
	//! \return 处理后的长度。
	static UGdouble XRotateResize(UGdouble dXLength, UGdouble dCos, UGdouble dSin, 
								UGdouble dXRatio, UGdouble dYRatio);	

	//! \brief 计算Y方向的长度在旋转一定角度后，又被resize，之后应该的长度
	//! \remarks 主要用在各种带有旋转角度的Geometry的Resize函数中对Y方向的长度的处理
	//! \param dYLength Y方向的长度[in]。
	//! \param dCos 该长度具备的旋转弧度的cos值[in]。
	//! \param dSin 该长度具备的旋转弧度的sin值[in]。
	//! \param dXRatio X方向的缩放比例[in]。
	//! \param dYRatio Y方向的缩放比例[in]。
	//! \return 处理后的长度。
	static UGdouble YRotateResize(UGdouble dYLength, UGdouble dCos, UGdouble dSin, 
								UGdouble dXRatio, UGdouble dYRatio);


	//! \brief 16进制字符串转换为对应的Byte形式的Buffer
	//! \param pBuffer		转换成的字节串[in/out]
	//! \param nLength      字节Buffer长度[in]
	//! \param strValue     要转换的字符[in]
	//! \return 转换是否成功
	//! \remarks 在保存独立符号到xml中时，需要将独立符号的二进制流保存在xml中，此两个函数的作用主要就是将二进制流转换为对应的16进制格式的字符串来进行保存，同时提供对应的将xml中保存的16进制格式的字符串解析成二进制流的格式的函数
	static UGbool StringConver2Byte(UGbyte*& pBuffer,const UGint& nLength,UGString strValue);
	//! \brief Byte形式的Buffer转换为对应的16进制字符串
	//! \param pBuffer		转换成的字节串[in]
	//! \param nLength      字节Buffer长度[in]
	//! \param strResult     要转换的字符[in/out]
	//! \return 转换是否成功
	static UGbool ByteConver2String(const UGbyte* pBuffer,UGint nLength,UGString& strResult);


	//! \brief 16进制字符串转换为对应的Byte形式的Buffer
	//! \param pBuffer		转换成的字节串[in/out]
	//! \param nLength      字节Buffer长度[in]
	//! \param strValue     要转换的字符[in]
	//! \return 转换是否成功
	//! \remarks 在保存独立符号到xml中时，需要将独立符号的二进制流保存在xml中，此两个函数的作用主要就是将二进制流转换为对应的16进制格式的字符串来进行保存，同时提供对应的将xml中保存的16进制格式的字符串解析成二进制流的格式的函数
	static UGbool StringConver2Byte(UGchar*& pBuffer,const UGint& nLength,UGString strValue);
	//! \brief Byte形式的Buffer转换为对应的16进制字符串
	//! \param pBuffer		转换成的字节串[in]
	//! \param nLength      字节Buffer长度[in]
	//! \param strResult     要转换的字符[in/out]
	//! \return 转换是否成功
	static UGbool ByteConver2String(const UGchar* pBuffer,UGint nLength,UGString& strResult);

public:
	
	  //!  \brief Toolkit
	static UGbool InRange(UGint nValue, UGint nMin, UGint nMax);
	  //!  \brief Toolkit
	static UGbool InRange(const void* pValue, const void* pMin, const void* pMax);
	  //!  \brief Toolkit
	static UGint GetBitCount(OGDC::PixelFormat pf); 

public:	
	//! \brief 保存ColorTable为XML字符串。
	//! \param colorTable  颜色表。[in]
	//! \param nVersion  工作空间版本。[in]
	//! \param strElemName  XML标签，默认为ColorTable。[in]
	//! \return 。
	static UGString ColorTableToXML(const UGColorTable& colorTable, UGint nVersion = 0, const UGString& strElemName = _U(""));	
	//! \brief 从XML字符串解析得到ColorTable。
	//! \param colorTable  颜色表。[in]
	//! \param strXML  XML字符串。[in]
	//! \param nVersion  工作空间版本。[in]
	//! \return 。
	static UGbool ColorTableFromXML(UGColorTable& colorTable, const UGString& strXML, UGint nVersion = 0);	

	//! \brief 保存ColorDictTable为XML字符串。
	//! \param colorDictTable  颜色表。[in]
	//! \param nVersion  工作空间版本。[in]
	//! \param strElemName  XML标签，默认为ColorTable。[in]
	//! \return 。
	static UGString ColorDictTableToXML(const UGColorDictTable& colorDictTable, UGint nVersion = 0, const UGString& strElemName = _U(""));		

	//! \brief 得到XML文件头
	//! \remarks 在简体中文环境下返回 <?xml version="1.0" encoding="GB18030"?> 
	static UGString GetXmlFileHeader(UGMBString::Charset charset = GetCurCharset());
    //! \brief 得到波段数目
	static UGint GetBandCount(UGint nFileType, OGDC::PixelFormat nPixelFormat = IPF_UNKNOWN);
	//
	//! \brief 通过colorspace获取他的显示的像素格式和波段数量。
	//! \param colorSpace [in]。
	//! \param ePixFormat [in]。
	//! \param nCount [in]。
	static UGbool GetViewPixelFormat(ImgColorSpace nColorSpace, OGDC::PixelFormat& ePixFormat,UGint& nBandCount);	
	
	//! \brief 地理坐标2像素坐标。
	//! \param rectBound     地理坐标[in]。
	//! \param refRectBound  参考地理坐标[in]。
	//! \param refRectXY     参考数据的宽和高。
	//! \param rectImg       像素坐标[out]。
	void static XYToImg(const UGRect2D rectBound, const UGRect2D refRectBound, const UGSize refRectXY, UGRect& rectImg);

	//added by yanmb 2007.5.11
	
	//! \brief 规范文本输出格式
	//! \param strXml  需要规范的文本字符串
	//! \param nPace   文本缩进的空格数
	//! \remarks 文本要规范的格式典型样式是这样的
//! ####< >
//! ####   < />
//! ####   < >
//! ####   </ >
//! ####   < >
//! ####      < >
//! ####      </ >
//! ####      < />  //---这是单个标签的情况---------
//! ####      < />
//! ####   </ >
//! ####   < >
//! ####      < >
//! ####      </ >
//! ####   </ >
//! ####</ >
	//! \attention
	//! 1.在只相差一个级别的">"与"<"之间的内容我们可以规定为一个换行加3个空格
	//! 2.同一级别的">"与"</"之间的内容我们可以规定为一个换行
	//! 3.对于类似"< />"单个标签的情况,我们将其类同</ >来处理
	
	static UGString AdjustXmlString(UGString strXml, UGint nPace = 0);

	//! \brief  字符串进一步分割
	//! \remarks 主要用于GML解析时
	//! \param  strSrc		需要进一步分割的字符串[in]
	//! \param strDestArray 输出字符串数组 [out]
	//! \param str1st		第一次分割依赖的字符
	//! \param str2nd		第二次分割依赖的字符
	//! \attention
	//! 这里实在是个无奈之举，由于GML有些版本标签内的内容以逗号和空格隔开，
	//! 有些则以空格隔开。UGString的split方法只能做一步分割。
	//! 其他地方可能很少这种情况，所以这里只考虑符合GML的用法来做了。yanmb
	
	static UGbool SlipString(UGString &strSrc, UGStrings &strDestArray, UGString str1st, UGString str2nd);
public: 
	//! \brief 判断字符是否属于 空ASCII字符, 包括 空格,\t\r\n等
	static UGbool IsSpaceChar(UGachar ch);

	//! \brief 判断字符是否属于 空ASCII字符, 包括 空格,\t\r\n等
	static UGbool IsSpaceChar(UGwchar wch);

	//! \brief 判断字符是否属于 数字字符, 从 '0' 到 '9'
	static UGbool IsDigit(UGachar ch);

	//! \brief 判断字符是否属于 十六进制字符, 包括从 '0' 到 '9', 'A'到'F', 'a'到'f'
	static UGbool IsHexDigit(UGachar ch);

	//! \brief 判断字符是否属于 大写字符, 从 'A' 到 'Z'
	static UGbool IsBigLetter(UGachar ch);

	//! \brief 判断字符是否属于 小写字符, 从 'a' 到 'z'
	static UGbool IsLittleLetter(UGachar ch);

	//! \brief 判断字符是否属于 数字字符, 从 '0' 到 '9'
	static UGbool IsDigit(UGwchar ch);

	//! \brief 判断字符是否属于 十六进制字符, 包括从 '0' 到 '9', 'A'到'F', 'a'到'f'
	static UGbool IsHexDigit(UGwchar ch);

	//! \brief 判断字符是否属于 大写字符, 从 'A' 到 'Z'
	static UGbool IsBigLetter(UGwchar ch);

	//! \brief 判断字符是否属于 小写字符, 从 'a' 到 'z'
	static UGbool IsLittleLetter(UGwchar ch);

	//! \brief 判断该字节是否是lead byte，即该字节和后续字节联合在一起构成一个双字节的字符（如中文、日文等）
	static UGbool IsLeadByte(UGachar ch);

	//! \brief 判断一个UCS2的字符是否是英文字符
	//! \return 如果是中文或日文等字符，返回false；是英文字符，返回true
	static UGbool IsAscii(UGushort ch);

	static UGColor Brightness(UGColor c, UGint nPercentage);
	static UGColor Contrast(UGColor c, UGint nPercentage);

	//! \brief 去除传入字符串中的逗号
	//! \brief added by zhengyueling at 2011-4-21
	static void	RemoveComma(UGString& strSrc);

	//判断字符串是否utf-8，有缺陷，例如改gb2312的“食品”。但还可以用一下
	static UGbool IsUTF8(UGMBString &strSrc);

public:
	//UGVariant 的转换方法
	//! \brief 转换为double类型
	static UGdouble VariantToDouble(const UGVariant& var);
	//! \brief 转换为int32 类型
	static UGint VariantToInt(const UGVariant& var);
	//! \brief 转换为int64 类型
	static UGlong VariantToLong(const UGVariant& var);
	//! \brief 转换为String类型
	static UGString VariantToString(const UGVariant& var);

public:
	//! \brief 指定目录的可用磁盘空间
	//! \param  strPath		指定目录[in]
	//! \return 磁盘可用字节
#ifndef SYMBIAN60
	static UGulong GetDiskSpace(const UGString& strPath);
#endif
	//! \brief 获取当前的堆栈信息
	//! \remarks 在windows下面需要pdb文件才会有效。在aix下面需要产生相应的core文件。
	static void  GetStackInfo(UGStrings &strArray);
	//! \brief 获得指定的信号对应的字符串
	static UGString GetSignalStr(UGint nSig);

	//! \brief 变量转成double型
	static UGbool Variant2Double(const UGVariant& var, double& dKey);

	static UGbool CheckIsKeyword(UGString strWord);
	//only support win
	static UGint GetWDriveType(UGString& str);

	static UGbool IsValidFieldOrTableName(const UGString& strFieldOrTableName, InvalidNameState& nInvalidStata);

	static UGbool IsUnicodeVersion();
private:
	// added by zengzm 2007-3-26 仅供内部使用,把得到的数据存在内部全局变量中
	static void SaveScreenSize(UGuint nScreen, UGuint ScreenValue);
	
};
//以下这些模板函数做成全局的，放到类里其他工程用会连接不过

//得到最(较)大值
	  //!  \brief Toolkit
template<typename Type>
	  //!  \brief Toolkit
const Type& Max(const Type& tValue1, const Type& tValue2)
{
	  //!  \brief Toolkit
	return ( (tValue1<tValue2)? tValue2 : tValue1);
}
//在nCount个数中得到最大值
	  //!  \brief Toolkit
template<typename Type>
	  //!  \brief Toolkit
Type Max(const Type* ptValue, UGint nCount)
{
	  //!  \brief Toolkit
	assert(ptValue != NULL); //ptValue不能为空
	  //!  \brief Toolkit
	Type tResult(ptValue[0]); 
	  //!  \brief Toolkit
	for(UGint i=1; i<nCount; i++)
	{
	  //!  \brief Toolkit
		tResult = ((ptValue[i]<tResult)? tResult : ptValue[i]);
	}
	  //!  \brief Toolkit
	return tResult;
}

//得到最(较)大值,采用比较仿函数(小于比较)
template<typename Type, typename Compare>
const Type& Max(const Type& tValue1, const Type& tValue2, Compare compare)
{
	return (compare(tValue1, tValue2)? tValue2 : tValue1);
}

//在nCount个数中得到最大值,采用比较仿函数(小于比较)
template<typename Type, typename Compare>
Type Max(const Type* ptValue, UGint nCount, Compare compare)
{
	assert(ptValue != NULL); //ptValue不能为空
	Type tResult(ptValue[0]); 
	for(UGint i=1; i<nCount; i++)
	{
		tResult = (compare(ptValue[i], tResult)? tResult : ptValue[i]);
	}
	return tResult;
}

template<typename Type>
const Type& Min(const Type& tValue1, const Type& tValue2)
{
	return ( (tValue2<tValue1)? tValue2 : tValue1);
}

//在nCount个数中得到最小值
template<typename Type>
Type Min(const Type* ptValue, UGint nCount)
{
	assert(ptValue != NULL); //ptValue不能为空
	Type tResult(ptValue[0]); 
	for(UGint i=1; i<nCount; i++)
	{
		tResult = ((tResult<ptValue[i])? tResult : ptValue[i]);
	}
	return tResult;
}

//得到最(较)小值,采用比较仿函数(小于比较)
template<typename Type, typename Compare>
const Type& Min(const Type& tValue1, const Type& tValue2, Compare compare)
{
	return (compare(tValue2,tValue1)? tValue2 : tValue1);
}

//在nCount个数中得到最小值,采用比较仿函数(小于比较)
template<typename Type, typename Compare>
Type Min(const Type* ptValue, UGint nCount, Compare compare)
{
	assert(ptValue != NULL); //ptValue不能为空
	Type tResult(ptValue[0]); 
	for(UGint i=1; i<nCount; i++)
	{
		tResult = (compare(tResult, ptValue[i])? tResult : ptValue[i]);
	}
	return tResult;
}
template<typename Type>
void Sort(Type* pBegin, Type* pEnd)
{
	std::sort(pBegin,pEnd);
}
template<typename Type, typename Compare>
void Sort(Type* pBegin, Type* pEnd, Compare compare)
{
	std::sort(pBegin,pEnd,compare);
}
// StableSort, 基本同Sort, 区别在于StableSort保证相等元素的原本次序在排序后保持不变
template<typename Type>
void StableSort(const Type* pBegin, const Type* pEnd)
{
	std::stable_sort(pBegin, pEnd);
}

template<typename Type, typename Compare>
void StableSort(const Type* pBegin, const Type* pEnd, Compare compare)
{
	std::stable_sort(pBegin, pEnd, compare);
}

//! \brief 求两个集合之间的差
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type>
void Difference(const Type* pBegin1, const Type* pEnd1, 
				const Type* pBegin2, const Type* pEnd2, UGArray<Type>&arrResult)
{
	std::set_difference(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()));
}

//! \brief 求两个集合之间的差
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type, typename Compare>
void Difference(const Type* pBegin1, const Type* pEnd1, 
				const Type* pBegin2, const Type* pEnd2, 
				UGArray<Type>&arrResult, Compare compare)
{
	std::set_difference(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()), compare);
}

//! \brief 求两个集合之间的交
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type>
void Intersection(const Type* pBegin1, const Type* pEnd1, 
				  const Type* pBegin2, const Type* pEnd2, UGArray<Type>&arrResult)
{
	std::set_intersection(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()));
}

//! \brief 求两个集合之间的交
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type, typename Compare>
void Intersection(const Type* pBegin1, const Type* pEnd1, 
				  const Type* pBegin2, const Type* pEnd2, 
				  UGArray<Type>&arrResult, Compare compare)
{
	std::set_intersection(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()), compare);
}

//! \brief 求两个集合之间的对称差
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type>
void SymmetricDifference(const Type* pBegin1, const Type* pEnd1, 
						 const Type* pBegin2, const Type* pEnd2, UGArray<Type>&arrResult)
{
	std::set_symmetric_difference(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()));
}

//! \brief 求两个集合之间的对称差
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type, typename Compare>
void SymmetricDifference(const Type* pBegin1, const Type* pEnd1, 
						 const Type* pBegin2, const Type* pEnd2, 
						 UGArray<Type>&arrResult, Compare compare)
{
	std::set_symmetric_difference(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()), compare);
}

//! \brief 求两个集合之间的并
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type>
void Union(const Type* pBegin1, const Type* pEnd1, 
		   const Type* pBegin2, const Type* pEnd2, UGArray<Type>&arrResult)
{
	std::set_union(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()));
}

//! \brief 求两个集合之间的并
//! \remarks arrResult原有数据会被覆盖,空间不够会自动增长
template<typename Type, typename Compare>
void Union(const Type* pBegin1, const Type* pEnd1, 
		   const Type* pBegin2, const Type* pEnd2, 
		   UGArray<Type>&arrResult, Compare compare)
{
	std::set_union(pBegin1, pEnd1, pBegin2, pEnd2, 
		std::inserter(arrResult.Inner(), arrResult.Inner().begin()), compare);
}

//! \brief 让集合中的元素不重复
//! \param pBegin 集合开始指针
//! \param pEnd 集合结束指针
//! \remarks 在调用Unique之前,集合必须先通过排序,即必须是有序集合
//!			 处理后,保证前面若干个元素不重复
//!			 内部采用stl中的unique算法函数实现
//! \return 返回不重复元素的个数
template<typename Type>
UGuint Unique(Type* pBegin, Type* pEnd)
{
	return std::unique(pBegin, pEnd) - pBegin;
}

//! \brief 让集合中的元素不重复
//! \param pBegin 集合开始指针
//! \param pEnd 集合结束指针
//! \param Compare 元素比较对象
//! \remarks 在调用Unique之前,集合必须先通过排序,即必须是有序集合
//!			 处理后,保证前面若干个元素不重复
//!			 内部采用stl中的unique算法函数实现
//! \return 返回不重复元素的个数
template<typename Type, typename Compare>
UGuint Unique(Type* pBegin, Type* pEnd, Compare compare)
{
	return std::unique(pBegin, pEnd, compare) - pBegin;
}

//! \brief 让集合中的元素不重复
//! \param pBegin 集合开始指针
//! \param pEnd 集合结束指针
//! \param pOut 输出的集合指针
//! \remarks 在调用UniqueCopy之前,集合必须先通过排序,即必须是有序集合;且pOut有足够的空间(不小于返回值)
//!			 处理后,保证pOut前面若干个元素不重复
//!			 内部采用stl中的unique算法函数实现
//! \return 返回不重复元素的个数
template<typename Type>
UGuint UniqueCopy(Type* pBegin, Type* pEnd, Type* pOut)
{
	return std::unique_copy(pBegin, pEnd, pOut) - pOut;
}

//! \brief 让集合中的元素不重复
//! \param pBegin 集合开始指针
//! \param pEnd 集合结束指针
//! \param pOut 输出的集合指针
//! \param Compare 元素比较对象
//! \remarks 在调用UniqueCopy之前,集合必须先通过排序,即必须是有序集合;且pOut有足够的空间(不小于返回值)
//!			 处理后,保证pOut前面若干个元素不重复
//!			 内部采用stl中的unique算法函数实现
//! \return 返回不重复元素的个数
template<typename Type, typename Compare>
UGuint UniqueCopy(Type* pBegin, Type* pEnd, Type* pOut, Compare compare)
{
	return std::unique_copy(pBegin, pEnd, pOut, compare) - pOut;
}
//! \brief 获取ugc.xml中的配置，主要是是否使用自定义地图比率，x比率，y比率
extern TOOLKIT_API UGbool UGIsUserCoorRatio();
extern TOOLKIT_API void UGSetUserCoorRatio(UGbool bUserCoorRatio = FALSE);

//! \brief 设置/获取地图DPI，set会设置到配置文件里边去，get如果是自定义的从配置文件里边获取，如果不是自定义就是屏幕的DPI
extern TOOLKIT_API void SetSystemDPI(UGdouble dSystemDPI);
extern TOOLKIT_API UGdouble GetSystemDPI();
//! \brief 计算DC对应的DPI值。如果hDC为0的话，采用跟当前屏幕兼容的DC
extern TOOLKIT_API UGdouble ComputeDPI(UGlong hDC = 0);
//! \brief 计算DC对应的DPI值。如果hDC为0的话，采用跟当前屏幕兼容的DC
extern TOOLKIT_API void ComputeDPI(UGlong hDC, UGdouble& dDPIX, UGdouble& dDPIY);


extern TOOLKIT_API UGdouble UGMapCoorRatioX();
extern TOOLKIT_API UGdouble UGMapCoorRatioY();
}

#endif

