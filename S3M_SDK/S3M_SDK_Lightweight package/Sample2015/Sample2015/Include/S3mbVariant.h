#pragma once
#include <string>
#include <time.h>

using namespace std;
namespace S3MB
{
#define ROUND(x) (((x)>0)?int((x)+0.5):(int((x)-0.5)))
	class S3mbTime
	{
	public:
		//! \brief 拷贝构造函数
		S3mbTime(double dValue);
		//! \brief 获取时间
		double GetTime() const;
		//! \brief 传入一个格式化字符串,把S3mbTime的信息生成一个便于阅读的字符串
	//! \remarks 
	//! %d Day of month as decimal number (01 – 31);
	//! %H Hour in 24-hour format (00 – 23);
	//! %m Month as decimal number (01 – 12)
	//! %M Minute as decimal number (00 – 59)
	//! %S Second as decimal number (00 – 59)
	//! %y Year without century, as decimal number (00 – 99)
	//! %Y Year with century, as decimal number
		string Format(const char* pFormat) const;

		//! \brief 把double数值转化为tm结构
		void GetDateTime(struct tm& tmDest) const;
	private:
		//! \brief 获取标准的tm结构
		void GetStandardTm(struct tm& tmDest) const;
		double m_time;
	};
	class S3mbVariant
	{
	public:
		//! \brief 支持的类型
		enum VarType
		{
			//! \brief 未定义的类型
			Null = 0,
			//! \brief 单字节类型
			Byte = 1,
			//! \brief 双字节整型类型
			Short = 2,
			//! \brief 四字节整型类型
			Integer = 3,
			//! \brief 八字节整型类型
			Long = 4,
			//! \brief 四字节浮点类型
			Float = 5,
			//! \brief 八字节浮点类型
			Double = 6,
			//! \brief 时间类型
			Time = 7,
			//! \brief 二进制类型
			Binary = 8,
			//! \brief 字符串
			String = 9,
			//! \brief 日期类型
			Date = 10,
			//! \brief 时间戳类型 
			TimeStamp = 11,
			//! \brief 布尔类型
			Boolean = 12
		};//为了与UGFieldInfo中的三种日期时间类型区分开，把原来的time类型扩展为time date timestamp，定义参考UGFieldInfo中对应的类型

		struct binaryVal {
			//! \brief 存储 Binary 指针或者 String指针
			void* pVal;
			//! \brief 存储Binary 数据流的大小 以字节为单位。
			unsigned int nSize;
		};

		//! \brief 变体联合
		union VarValue
		{
			unsigned char bVal;
			short sVal;
			int	iVal;
			long  lVal;
			float fVal;
			double dVal;
			double   tmVal; // S3mbTime的内部实现
			struct binaryVal binVal;
		};

	public:
		//! \brief 默认构造函数
		S3mbVariant();

		//! \brief 析构函数
		~S3mbVariant();

		//! \brief 拷贝构造函数
		S3mbVariant(const S3mbVariant& varSrc);

		//! \brief unsigned char类型构造函数
		S3mbVariant(unsigned char bVal);
		//! \brief short类型构造函数
		S3mbVariant(short sVal);
		//! \brief int类型构造函数
		S3mbVariant(int iVal);
		//! \brief long类型构造函数
		S3mbVariant(long lVal);
		//! \brief float类型构造函数
		S3mbVariant(float fVal);
		//! \brief double类型构造函数
		S3mbVariant(double dVal);
		//! \brief S3mbTime类型构造函数
		S3mbVariant(const S3mbTime& tmVal);
		//! \brief unsigned char* 类型构造函数
		//! \remarks 内部为内存拷贝，外边注意释放pData;
		S3mbVariant(const unsigned char* pData, int nSize);
		//! \brief string 类型构造函数
		S3mbVariant(const string& str);

		//! \brief 将 varSrc 赋值变体。
		const S3mbVariant& operator=(const S3mbVariant& varSrc);
		//! \brief 将 bVal 赋值变体
		const S3mbVariant& operator=(unsigned char bVal);
		//! \brief 将 sVal 赋值变体
		const S3mbVariant& operator=(short sVal);
		//! \brief 将 iVal 赋值变体
		const S3mbVariant& operator=(int iVal);
		//! \brief 将 lVal 赋值变体
		const S3mbVariant& operator=(long lVal);
		//! \brief 将 fVal 赋值变体
		const S3mbVariant& operator=(float fVal);
		//! \brief 将 dVal 赋值变体
		const S3mbVariant& operator=(double dVal);
		//! \brief 将 tmVal 赋值变体
		const S3mbVariant& operator=(const S3mbTime& tmVal);
		//! \brief 将 str 赋值变体
		const S3mbVariant& operator=(const string& str);

		//! \brief 设置变量值为bVal 
		void Set(unsigned char bVal);
		//! \brief 设置变量值为sVal 
		void Set(short sVal);
		//! \brief 设置变量值为nVal 
		void Set(int nVal);
		//! \brief 设置变量值为nVal 
		void Set(long lVal);
		//! \brief 设置变量值为dVal 
		void Set(float dVal);
		//! \brief 设置变量值为dVal 
		void Set(double dVal);
		//! \brief 设置变量值为tmVal 
		void Set(const S3mbTime& tmVal);
		//! \brief 设置变量值为pData指向的字符串
		void Set(const unsigned char* pData, unsigned int nSize);
		//! \brief 设置变量值为str 
		void Set(const string& str);
		//! \brief 设置为空
		void SetNull();

		//! \brief 判断是否相等
		//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
		bool operator==(const S3mbVariant& varSrc) const;

		//! \brief 判断是否不相等
		//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
		bool operator!=(const S3mbVariant& varSrc) const;

		//! \brief 判断大小
		//! \remarks 类型不相等, 则认为不相等(COleVariant的处理方式)
		bool operator<(const S3mbVariant& varSrc) const;

		//! \brief 清空
		void Clear();

		//! \brief 获取变体类型
		VarType GetType() const
		{
			return m_nType;
		}

		//! \brief 获取变体值
		VarValue& GetValue()
		{
			return m_value;
		}

		//! \brief 获取变体值
		const VarValue& GetValue() const
		{
			return m_value;
		}

		//! \brief 转换为double类型
		double ToDouble() const;
		//! \brief 转换为int32 类型
		int ToInt() const;
		//! \brief 转换为int64 类型
		long ToLong() const;
		//! \brief 转换为String类型
		//! \remarks 对于UGTime类型，转换成数据库识别的格式也就是 %Y-%m-%d %H:%M:%S 。
		string ToString() const;
		//! \brief 转换为String类型
		//! \remarks 对于UGTime类型，转换成本地当前的时间风格,对于其它类型和ToString()方法一样。
		string ToStringLocal() const;

	public:
		//! \brief 变体类型
		VarType m_nType;

		//! \brief 变体值
		VarValue m_value;

	};
}