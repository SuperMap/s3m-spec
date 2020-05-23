#ifndef  _JSONPARSER_H_
#define  _JSONPARSER_H_

//通用名称定义
#define JSON_POS_X					"x"
#define JSON_POS_Y					"y"
#define JSON_POS_Z					"z"
#define JSON_POS_W					"w"
#define JSON_POS_RADIUS			    "r"
#define JSON_BBOX_MAX			    "max"
#define JSON_RECT_LEFT				"left"
#define JSON_RECT_TOP				"top"
#define JSON_RECT_RIGHT			    "right"
#define JSON_RECT_BOTTOM		    "bottom"
#define JSON_BBOX_MIN				"min"
#define JSON_COLOR_R			    "r"
#define JSON_COLOR_G				"g"
#define JSON_COLOR_B				"b"
#define JSON_COLOR_A			    "a"

#include "JSON//include/json.h"
#include "Skeleton.h"
#include "Palette.h"

using namespace std;
using namespace Json;
class S3MBSCP;
class ColorValue
{
public:
	ColorValue():
		r(0), g(0), b(0), a(0) {}
	ColorValue(double red, double green, double blue, double alpha):
		r(red),g(green),b(blue),a(alpha){}
	double r;
	double g;
	double b;
	double a;
	//! \brief 传入R、G、B、A分量构建三维颜色 。
	//! \param nRed 红色分量，范围0-255[in]。
	//! \param nGreen 绿色分量，范围0-255[in]。
	//! \param nBlue 蓝色分量，范围0-255[in]。
	//! \param nAlpha 不透明度分量，范围0-255[in]。
	void SetValue(int nRed, int nGreen, int nBlue, int nAlpha);
	//! \brief 传入R、G、B、A分量构建三维颜色。
	//! \param dRed 红色分量，范围0-1.0[in]。
	//! \param dGreen 绿色分量，范围0-1[in]。
	//! \param dBlue 蓝色分量，范围0-1[in]。
	//! \param dAlpha 不透明度分量，范围0-1[in]。
	void SetValue(double nRed, double nGreen, double nBlue, double nAlpha);
	//! \brief  将ColorValue转换为UGColor
	UGColor GetValue() const;
};

class JsonValue
{
public:
	JsonValue(){}
	JsonValue(ValueType type, bool isManaged = false);
   //通过Json字符串初始化自身
	JsonValue(string & strJsonContent);
	~JsonValue(void);

	//通过字符串初始化自身
	bool LoadValueFormString(string & strJsonContent);
	bool LoadFromFile(const string& strFilePath);
	//获取指定标签的目标对象
	bool GetValue(const string strTag, JsonValue*& pObject);
	bool GetValue(const string strTag, string& strValue);
	bool GetValue(const string strTag, ColorValue& color);
	bool GetValue(const string strTag, double& dbValue);
	bool GetValue(const string strTag, bool& bValue);
	bool GetValue(const string strTag, float& fValue);
	bool GetValue(const string strTag, int& nValue);
	bool GetValue(const string strTag, Vector3d& vec3D);
	bool GetValue(const string strTag, Vector4d& vec4D);
	bool GetValue(const string strTag, Matrix4d& mat);
	bool GetXYZValue(const string strTag, double& dbX, double& dbY, double& dbZ);
	//获取JsonValue数组的指定索引的JsonValue子对象
	bool GetAt(const int i, JsonValue* pObject);
	bool GetAt(const int i, double& dbValue);
	bool GetAt(const int i, int& nValue);
	int GetArraySize();

	void Add(const string strTag, JsonValue& value);
	void Add(const string strTag, const string &value);
	void Add(const string strTag, ColorValue nColor);
	void Add(const string strTag, const double value);
	void Add(const string strTag, Vector3d& vec3D);
	void Add(const string strTag, Vector4d& vec4D);
	void Add(const string strTag, Matrix4d& mat);
	void Add(const string strTag, Rect2D& rcBounds);
	void Add(const string strTag, BoundingBox& bbox);

	void Add(const bool value);
	void Add(const string& value);
	void Add(const int value);
	void Add(const double value);
	void Add(JsonValue& value);

	void SetValue(Value* pValue);
	Value& GetValue();
private:
	void* m_value;
	bool m_isManaged;
};

#endif
