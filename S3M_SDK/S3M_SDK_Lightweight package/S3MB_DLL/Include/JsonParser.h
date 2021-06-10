#ifndef  _JSONPARSER_H_
#define  _JSONPARSER_H_

namespace S3MB
{
	//通用名称定义
#define JSON_POS_X					L"x"
#define JSON_POS_Y					L"y"
#define JSON_POS_Z					L"z"
#define JSON_POS_W					L"w"
#define JSON_POS_RADIUS			    L"r"
#define JSON_BBOX_MAX			    L"max"
#define JSON_RECT_LEFT				L"left"
#define JSON_RECT_TOP				L"top"
#define JSON_RECT_RIGHT			    L"right"
#define JSON_RECT_BOTTOM		    L"bottom"
#define JSON_BBOX_MIN				L"min"
#define JSON_COLOR_R			    L"r"
#define JSON_COLOR_G				L"g"
#define JSON_COLOR_B				L"b"
#define JSON_COLOR_A			    L"a" 
}

#include "stdafx.h"
#include "Skeleton.h"
#include "Palette.h"

using namespace std;
namespace S3MB
{
	class S3MBSCP;
	enum JsonValueType
	{
		nullValue = 0, ///< 'null' value
		intValue,      ///< signed integer value
		uintValue,     ///< unsigned integer value
		realValue,     ///< double value
		stringValue,   ///< UTF-8 wstring value
		booleanValue,  ///< bool value
		arrayValue,    ///< array value (ordered list)
		objectValue    ///< object value (collection of name/value pairs).
	};

	class S3MB_API JsonValue
	{
	public:
		JsonValue() { m_isManaged = false;  m_value = nullptr; }
		JsonValue(JsonValueType type, bool isManaged = false);
		//通过Json字符串初始化自身
		JsonValue(wstring& strJsonContent);
		~JsonValue(void);

		//通过字符串初始化自身
		bool LoadValueFormString(wstring& strJsonContent);
		bool LoadValueFormString(string& strJsonContent);
		bool LoadFromFile(const wstring& strFilePath);
		//获取指定标签的目标对象
		bool GetValue(const wstring strTagw, JsonValue*& pObject);
		bool GetValue(const wstring strTagw, wstring& strValue);
		bool GetValue(const wstring strTagw, ColorValue& color);
		bool GetValue(const wstring strTagw, double& dbValue);
		bool GetValue(const wstring strTagw, bool& bValue);
		bool GetValue(const wstring strTagw, float& fValue);
		bool GetValue(const wstring strTagw, int& nValue);
		bool GetValue(const wstring strTagw, Vector3d& vec3D);
		bool GetValue(const wstring strTagw, Vector4d& vec4D);
		bool GetValue(const wstring strTagw, Matrix4d& mat);
		bool GetXYZValue(const wstring strTagw, double& dbX, double& dbY, double& dbZ);
		//获取JsonValue数组的指定索引的JsonValue子对象
		bool GetAt(const int i, JsonValue* pObject);
		bool GetAt(const int i, double& dbValue);
		bool GetAt(const int i, int& nValue);
		int GetArraySize();

		void Add(const wstring strTagw, JsonValue& value);
		void Add(const wstring strTagw, const wstring& value);
		void Add(const wstring strTagw, ColorValue nColor);
		void Add(const wstring strTagw, const double value);
		void Add(const wstring strTagw, Vector3d& vec3D);
		void Add(const wstring strTagw, Vector4d& vec4D);
		void Add(const wstring strTagw, Matrix4d& mat);
		void Add(const wstring strTagw, Rect2D& rcBounds);
		void Add(const wstring strTagw, BoundingBox& bbox);

		void Add(const bool value);
		void Add(const wstring& value);
		void Add(const int value);
		void Add(const double value);
		void Add(JsonValue& value);

		void SetValue(void* pValue);
		void* GetValue();
	private:
		void* m_value;
		bool m_isManaged;
	};
}

#endif
