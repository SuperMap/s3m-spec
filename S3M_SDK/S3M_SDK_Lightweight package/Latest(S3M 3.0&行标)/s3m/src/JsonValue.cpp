
#include "JsonValue.h"
#include "S3MBUtils.h"
#include "./json/json.h"
#include <assert.h>
#include <fstream>

// 通用名称定义
#define JSON_POS_X					U("x")
#define JSON_POS_Y					U("y")
#define JSON_POS_Z					U("z")
#define JSON_POS_W					U("w")
#define JSON_POS_RADIUS			    U("r")
#define JSON_BBOX_MAX			    U("max")
#define JSON_RECT_LEFT				U("left")
#define JSON_RECT_TOP				U("top")
#define JSON_RECT_RIGHT			    U("right")
#define JSON_RECT_BOTTOM		    U("bottom")
#define JSON_BBOX_MIN				U("min")
#define JSON_BBOX_CENTER			U("center")
#define JSON_BBOX_XEXTENT			U("xExtent")
#define JSON_BBOX_YEXTENT			U("yExtent")
#define JSON_BBOX_ZEXTENT			U("zExtent")
#define JSON_COLOR_R			    U("r")
#define JSON_COLOR_G				U("g")
#define JSON_COLOR_B				U("b")
#define JSON_COLOR_A			    U("a")

using namespace Json;
namespace S3MB
{
	JsonValue::JsonValue()
	{
		m_isManaged = true;
		m_value = nullptr;
	}

	JsonValue::JsonValue(JsonValueType type, bool isManaged)
	{
		ValueType valueType = (ValueType)type;
		m_value = new Value(valueType);
		m_isManaged = isManaged;
	}

	JsonValue::JsonValue(wstring& strJsonContent)
	{
		LoadValueFromString(strJsonContent);
	}

	JsonValue::~JsonValue(void)
	{
		if (m_isManaged)
		{
			delete (Value*)m_value;
			m_value = NULL;
		}
	}

	bool JsonValue::LoadValueFromString(wstring& strJsonContent)
	{
#ifdef WIN32
        string strContent = StringUtil::UNICODE_to_UTF8(strJsonContent);
#else
        setlocale(LC_ALL,"");
        string strContent = StringUtil::UnicodeToANSI(strJsonContent);
#endif
		return LoadValueFromString(strContent);
	}

	bool JsonValue::LoadValueFromString(string& strJsonContent)
	{
		this->m_value = new Value(ValueType::objectValue);
		Reader reader;
		if (!reader.parse(strJsonContent, *(Value*)this->m_value))
		{
			return false;
		}
		this->m_isManaged = true;
		return true;
	}

	bool JsonValue::LoadFromFile(const wstring& strFilePath)
	{
#ifdef __linux__
        string FilePath = StringUtil::UnicodeToANSI(strFilePath);
        std::ifstream ifs(FilePath, ios::in | ios::binary);
#else
        std::ifstream ifs(strFilePath, ios::in | ios::binary);
#endif
		if (!ifs)
		{
			return false;
		}
		string strLine;
		string strJson;
		while (getline(ifs, strLine))
		{
			strJson.append(strLine);
		}
		ifs.close();
		this->LoadValueFromString(strJson);
		return true;
	}

	void JsonValue::SaveToString(wstring& strContent)
	{
		std::string strTemp;
		SaveToString(strTemp);
		strContent = StringUtil::UTF8_to_UNICODE(strTemp);
	}

	void JsonValue::SaveToString(string& strContent)
	{
		Value* pValue = (Value*)m_value;
		if (pValue != NULL)
		{
			FastWriter writer;
			strContent = writer.write(*pValue);
		}
	}

	void JsonValue::SaveToFile(const wstring& strFilePath)
	{
		string strContent;
		Value* pValue = (Value*)m_value;
		if (pValue != NULL)
		{
			StyledWriter writer;
			strContent = writer.write(*pValue);
		}
#ifdef __linux__
        string FilePath = StringUtil::UnicodeToANSI(strFilePath);
        std::ofstream ofs(FilePath, ios::out);
#else
		std::ofstream ofs(strFilePath, ios::out);
#endif
		ofs.write(strContent.c_str(), strContent.size());
		ofs.close();
	}

	bool JsonValue::GetValue(const wstring& strTagw, JsonValue& object)
	{
		Value* value = (Value*)m_value;
		if (value == NULL)
		{
			return false;
		}
#ifdef WIN32
		string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (!value || !value->isObject() || !value->isMember(strTag))
		{
			return false;
		}
		Value& objValue = (*value)[strTag];
		if (objValue.isObject() ||
			objValue.isArray())
		{
			object = JsonValue();
		}
		else
		{
			assert(false);
			return false;
		}
		object.SetValue(&objValue);
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagW, JsonValue*& pObject)
	{
		Value* object = (Value*)m_value;
		if (object == NULL)
		{
			return false;
		}
#ifdef WIN32
         string strTag = StringUtil::UNICODE_to_UTF8(strTagW);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagW);
#endif
		if (!object || !object->isObject() || !object->isMember(strTag) || pObject)
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (val.isObject())
		{
			pObject = new JsonValue(objectValue);
		}
		else if (val.isArray())
		{
			pObject = new JsonValue(arrayValue);
		}
		else
		{
			assert(false);
			return false;
		}
		pObject->SetValue(&val);
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, wstring& strValue)
	{
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (strTag == "")
		{
			return false;
		}
		Value* object = (Value*)m_value;
		if (!object || !object->isObject() || !object->isMember(strTag))
		{
			return false;
		}
		if (!object->isMember(strTag))
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (!val.isString())
		{
			return false;
		}
		strValue = StringUtil::UTF8_to_UNICODE(val.asString());
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, ColorValue& color)
	{

		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}

		pObject->GetValue(JSON_COLOR_R, color.r);
		pObject->GetValue(JSON_COLOR_G, color.g);
		pObject->GetValue(JSON_COLOR_B, color.b);
		pObject->GetValue(JSON_COLOR_A, color.a);

		delete pObject;
		pObject = NULL;
		return false;
	}

	bool JsonValue::GetValue(const wstring& strTagw, double& dbValue)
	{
		Value* object = (Value*)m_value;
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (!object || !object->isObject() || !object->isMember(strTag))
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (!val.isDouble())
		{
			return false;
		}
		dbValue = val.asDouble();
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, bool& bValue)
	{
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		Value* object = (Value*)m_value;
		if (!object || !object->isObject() || !object->isMember(strTag))
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (!val.isBool())
		{
			return false;
		}
		bValue = val.asBool();
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, float& fValue)
	{

#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		Value* object = (Value*)m_value;
		if (!object || !object->isObject() || !object->isMember(strTag))
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (!val.isDouble())
		{
			return false;
		}
		fValue = (float)val.asDouble();
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, Vector3d& vec3D)
	{
		return GetXYZValue(strTagw, vec3D.x, vec3D.y, vec3D.z);
	}

	bool JsonValue::GetValue(const wstring& strTagw, Vector4d& vec4D)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}

		pObject->GetValue(JSON_POS_X, vec4D.x);
		pObject->GetValue(JSON_POS_Y, vec4D.y);
		pObject->GetValue(JSON_POS_Z, vec4D.z);
		pObject->GetValue(JSON_POS_W, vec4D.w);
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, int& nValue)
	{
		Value* object = (Value*)m_value;
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (!object || !object->isObject() || !object->isMember(strTag))
		{
			return false;
		}
		Value& val = (*object)[strTag];
		if (!val.isInt())
		{
			return false;
		}
		nValue = val.asInt();
		return true;
	}

	bool JsonValue::GetValue(const wstring& strTagw, Matrix4d& mat)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject || pObject->GetArraySize() == 0)
		{
			return false;
		}

		int nSize = pObject->GetArraySize();
		assert(nSize == 16);
		JsonValue* pValue = new JsonValue(realValue);
		for (int i = 0; i < 4; i++)
		{
			pObject->GetAt(i * 4 + 0, pValue);
			mat[i][0] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 1, pValue);
			mat[i][1] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 2, pValue);
			mat[i][2] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 3, pValue);
			mat[i][3] = ((Value*)(pValue->GetValue()))->asDouble();
		}

		delete pValue;
		pValue = NULL;
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetValue(const wstring & strTagw, Rect2D & rcBounds)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}
		pObject->GetValue(JSON_RECT_LEFT, rcBounds.left);
		pObject->GetValue(JSON_RECT_TOP, rcBounds.top);
		pObject->GetValue(JSON_RECT_RIGHT, rcBounds.right);
		pObject->GetValue(JSON_RECT_BOTTOM, rcBounds.bottom);
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetValue(const wstring & strTagw, BoundingBox & bbox)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}

		bool bResult = false;
		Vector3d vecMin, vecMax;
		if (pObject->GetValue(JSON_BBOX_MIN, vecMin) &&
			pObject->GetValue(JSON_BBOX_MAX, vecMax))
		{
			bbox.SetMin(vecMin);
			bbox.SetMax(vecMax);
			bResult = true;
		}

		delete pObject;
		pObject = NULL;

		return bResult;
	}

	bool JsonValue::GetValue(const wstring& strTagw, OrientedBoundingBox& box)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}

		bool bResult = false;
		Vector3d vCenter, vXExtent, vYExtent, vZExtent;
		if (pObject->GetValue(JSON_BBOX_CENTER, vCenter) &&
			pObject->GetValue(JSON_BBOX_XEXTENT, vXExtent) &&
			pObject->GetValue(JSON_BBOX_YEXTENT, vYExtent) &&
			pObject->GetValue(JSON_BBOX_ZEXTENT, vZExtent))
		{
			Matrix3d matHalfAxes;
			matHalfAxes.SetColumn(0, vXExtent);
			matHalfAxes.SetColumn(1, vYExtent);
			matHalfAxes.SetColumn(2, vZExtent);
			box = OrientedBoundingBox(vCenter, matHalfAxes);
			bResult = true;
		}

		delete pObject;
		pObject = nullptr;
		return bResult;
	}

	bool JsonValue::GetXYZValue(const wstring& strTagw, double& dbX, double& dbY, double& dbZ)
	{
		JsonValue* pObject = NULL;
		GetValue(strTagw, pObject);
		if (!pObject)
		{
			return false;
		}

		pObject->GetValue(JSON_POS_X, dbX);
		pObject->GetValue(JSON_POS_Y, dbY);
		pObject->GetValue(JSON_POS_Z, dbZ);
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetAt(const int& i, JsonValue* pObject)
	{
		Value* object = (Value*)m_value;
		if (!object || !(object->isArray()))
		{
			assert(false);
			return false;
		}
		if (!(object->isValidIndex(i)))
		{
			assert(false);
			return false;
		}
		pObject->SetValue(&(*object)[i]);
		return true;
	}

	bool JsonValue::GetAt(const int& i, std::wstring& strValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return false;
		}

		if (!pObject->isValidIndex(i))
		{
			return false;
		}

		Value& val = (*pObject)[i];
		if (!val.isString())
		{
			return false;
		}

		strValue = StringUtil::UTF8_to_UNICODE(val.asString());
		return true;
	}

	bool JsonValue::GetAt(const int& i, double& dbValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return false;
		}

		if (!pObject->isValidIndex(i))
		{
			return false;
		}

		Value& val = (*pObject)[i];
		if (!val.isDouble())
		{
			return false;
		}

		dbValue = val.asDouble();
		return true;
	}

	bool JsonValue::GetAt(const int& i, int& nValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return false;
		}

		if (!pObject->isValidIndex(i))
		{
			return false;
		}

		Value& val = (*pObject)[i];
		if (!val.isInt())
		{
			return false;
		}
		nValue = val.asInt();
		return true;
	}

	bool JsonValue::GetAt(const int& i, Matrix4d& mat)
	{
		JsonValue *pObject = new JsonValue;
		GetAt(i, pObject);
		if (!pObject || pObject->GetArraySize() == 0)
		{
			return false;
		}

		int nSize = pObject->GetArraySize();
		assert(nSize == 16);
		JsonValue *pValue = new JsonValue(realValue);

		for (int i = 0; i < 4; i++)
		{
			pObject->GetAt(i * 4 + 0, pValue);
			mat[i][0] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 1, pValue);
			mat[i][1] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 2, pValue);
			mat[i][2] = ((Value*)(pValue->GetValue()))->asDouble();

			pObject->GetAt(i * 4 + 3, pValue);
			mat[i][3] = ((Value*)(pValue->GetValue()))->asDouble();
		}

		delete pValue;
		pValue = NULL;
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetAt(const int& i, Vector3d& vec3D)
	{
		JsonValue *pObject = new JsonValue;
		GetAt(i, pObject);
		if (!pObject)
		{
			return false;
		}

		pObject->GetValue(JSON_POS_X, vec3D.x);
		pObject->GetValue(JSON_POS_Y, vec3D.y);
		pObject->GetValue(JSON_POS_Z, vec3D.z);
		delete pObject;
		pObject = NULL;
		return true;
	}

	bool JsonValue::GetAt(const int& i, Vector4d& vec4D)
	{
		JsonValue *pObject = new JsonValue;
		GetAt(i, pObject);
		if (!pObject)
		{
			return false;
		}

		pObject->GetValue(JSON_POS_X, vec4D.x);
		pObject->GetValue(JSON_POS_Y, vec4D.y);
		pObject->GetValue(JSON_POS_Z, vec4D.z);
		pObject->GetValue(JSON_POS_W, vec4D.w);
		delete pObject;
		pObject = NULL;
		return false;
	}

	int JsonValue::GetArraySize()
	{
		if (!(Value*)m_value || !((Value*)m_value)->isArray())
		{
			return false;
		}

		return ((Value*)m_value)->size();
	}

	bool JsonValue::GetAllKeys(vector<wstring>& arr)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return false;
		}

		Value::Members members = pObject->getMemberNames();
		Value::Members::iterator iterator;
		for (iterator = members.begin(); iterator != members.end(); iterator++)
		{
			arr.emplace_back(StringUtil::UTF8_to_UNICODE(*iterator));
		}
		return true;
	}

	void JsonValue::Add(const bool& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return;
		}

		pObject->append(Value((bool)value));
	}

	void JsonValue::Add(const wstring& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return;
		}
#ifdef WIN32
        string strValue = StringUtil::UNICODE_to_UTF8(value);
#else
        string strValue = StringUtil::UnicodeToANSI(value);
#endif
		pObject->append(Value(strValue));
	}

	void JsonValue::Add(const int& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return;
		}

		pObject->append(Value(value));
	}

	void JsonValue::Add(const double& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return;
		}

		pObject->append(Value(value));
	}

	void JsonValue::Add(JsonValue& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isArray())
		{
			return;
		}
		pObject->append(*(Value*)value.GetValue());
	}


	void JsonValue::Add(const Matrix4d& mat)
	{
		JsonValue jsonArrary(arrayValue);
		jsonArrary.Add(mat[0][0]);	jsonArrary.Add(mat[0][1]);	jsonArrary.Add(mat[0][2]);	jsonArrary.Add(mat[0][3]);
		jsonArrary.Add(mat[1][0]);	jsonArrary.Add(mat[1][1]);	jsonArrary.Add(mat[1][2]);	jsonArrary.Add(mat[1][3]);
		jsonArrary.Add(mat[2][0]);	jsonArrary.Add(mat[2][1]);	jsonArrary.Add(mat[2][2]);	jsonArrary.Add(mat[2][3]);
		jsonArrary.Add(mat[3][0]);	jsonArrary.Add(mat[3][1]);	jsonArrary.Add(mat[3][2]);	jsonArrary.Add(mat[3][3]);

		Add(jsonArrary);
	}


	void JsonValue::Add(const wstring& strTagw, JsonValue& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		(*pObject)[strTag] = *(Value*)value.GetValue();
	}

	void JsonValue::Add(const wstring& strTagw, const wstring& value)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
        string strValue = StringUtil::UNICODE_to_UTF8(value);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
        string strValue;
        if(value.size() == 0){
            strValue = "";
        }else{
           strValue = StringUtil::UnicodeToANSI(value);
        }

#endif
		(*pObject)[strTag] = strValue;
	}
	void JsonValue::Add(const wstring& strTagw, ColorValue nColor)
	{
		JsonValue jsonColor(objectValue);
		jsonColor.Add(JSON_COLOR_R, nColor.r);
		jsonColor.Add(JSON_COLOR_G, nColor.g);
		jsonColor.Add(JSON_COLOR_B, nColor.b);
		jsonColor.Add(JSON_COLOR_A, nColor.a);

		Add(strTagw, jsonColor);
	}

	void JsonValue::Add(const wstring& strTagw, const double& value)
	{
		Value* pObject = (Value*)m_value;
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (!pObject || !pObject->isObject())
		{
			return;
		}
		(*pObject)[strTag] = value;
	}
	void JsonValue::Add(const wstring& strTagw, const int& value)
	{
		Value* pObject = (Value*)m_value;
#ifdef WIN32
        string strTag = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        string strTag = StringUtil::UnicodeToANSI(strTagw);
#endif
		if (!pObject || !pObject->isObject())
		{
			return;
		}
		(*pObject)[strTag] = value;
	}

	void JsonValue::Add(const Vector3d& vec3D)
	{
		JsonValue valPnt(objectValue);
		valPnt.Add(JSON_POS_X, vec3D.x);
		valPnt.Add(JSON_POS_Y, vec3D.y);
		valPnt.Add(JSON_POS_Z, vec3D.z);

		Add(valPnt);
	}

	void JsonValue::Add(const Vector4d& vec4D)
	{
		JsonValue valPnt(objectValue);
		valPnt.Add(JSON_POS_X, vec4D.x);
		valPnt.Add(JSON_POS_Y, vec4D.y);
		valPnt.Add(JSON_POS_Z, vec4D.z);
		valPnt.Add(JSON_POS_W, vec4D.w);
		Add(valPnt);
	}

	bool JsonValue::Contains(const wstring& strTagw)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return false;
		}

		std::string stdkey;
#ifdef WIN32
        stdkey = StringUtil::UNICODE_to_UTF8(strTagw);
#else
        stdkey = StringUtil::UnicodeToANSI(strTagw);
#endif
		return pObject->isMember(stdkey);
	}

	void JsonValue::Add(const wstring& strTagw, Vector3d& vec3D)
	{
		// "x" "y" "z" 
		JsonValue valPnt(objectValue);
		valPnt.Add(JSON_POS_X, vec3D.x);
		valPnt.Add(JSON_POS_Y, vec3D.y);
		valPnt.Add(JSON_POS_Z, vec3D.z);

		Add(strTagw, valPnt);
	}

	void JsonValue::Add(const wstring& strTagw, Vector4d& vec4D)
	{
		// "x" "y" "z" 
		JsonValue valPnt(objectValue);
		valPnt.Add(JSON_POS_X, vec4D.x);
		valPnt.Add(JSON_POS_Y, vec4D.y);
		valPnt.Add(JSON_POS_Z, vec4D.z);
		valPnt.Add(JSON_POS_W, vec4D.w);
		Add(strTagw, valPnt);
	}

	void JsonValue::Add(const wstring& strTagw, Matrix4d& mat)
	{
		JsonValue jsonArrary(arrayValue);
		jsonArrary.Add(mat[0][0]);	jsonArrary.Add(mat[0][1]);	jsonArrary.Add(mat[0][2]);	jsonArrary.Add(mat[0][3]);
		jsonArrary.Add(mat[1][0]);	jsonArrary.Add(mat[1][1]);	jsonArrary.Add(mat[1][2]);	jsonArrary.Add(mat[1][3]);
		jsonArrary.Add(mat[2][0]);	jsonArrary.Add(mat[2][1]);	jsonArrary.Add(mat[2][2]);	jsonArrary.Add(mat[2][3]);
		jsonArrary.Add(mat[3][0]);	jsonArrary.Add(mat[3][1]);	jsonArrary.Add(mat[3][2]);	jsonArrary.Add(mat[3][3]);

		Add(strTagw, jsonArrary);
	}

	void JsonValue::Add(const wstring& strTagw, Rect2D& rcBounds)
	{
		JsonValue valGeoBounds(objectValue);
		valGeoBounds.Add(JSON_RECT_LEFT, rcBounds.left);
		valGeoBounds.Add(JSON_RECT_TOP, rcBounds.top);
		valGeoBounds.Add(JSON_RECT_RIGHT, rcBounds.right);
		valGeoBounds.Add(JSON_RECT_BOTTOM, rcBounds.bottom);

		Add(strTagw, valGeoBounds);
	}

	void JsonValue::Add(const wstring& strTagw, BoundingBox& bbox)
	{
		JsonValue valBBox(objectValue);
		// "min" "max" 
		Vector3d vec = bbox.GetMin();
		valBBox.Add(JSON_BBOX_MIN, vec);
		vec = bbox.GetMax();
		valBBox.Add(JSON_BBOX_MAX, vec);

		Add(strTagw, valBBox);
	}

	void JsonValue::Add(const wstring& strTagw, OrientedBoundingBox& bbox)
	{
		JsonValue jsonBox(objectValue);
        Vector3d vec = bbox.GetCenter();
        jsonBox.Add(JSON_BBOX_CENTER, vec);
        vec = bbox.GetHalfAxes().GetColumn(0);
        jsonBox.Add(JSON_BBOX_XEXTENT, vec);
        vec = bbox.GetHalfAxes().GetColumn(1);
        jsonBox.Add(JSON_BBOX_YEXTENT, vec);
        vec = bbox.GetHalfAxes().GetColumn(2);
        jsonBox.Add(JSON_BBOX_ZEXTENT, vec);
		Add(strTagw, jsonBox);
	}

	void JsonValue::Update(const std::wstring& strTag, const bool newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		(*pObject)[stdkey] = newValue ? true : false;
	}

	void JsonValue::Update(const std::wstring& strTag, const int newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		(*pObject)[stdkey] = newValue;
	}

	void JsonValue::Update(const std::wstring& strTag, const long newValue)
	{
		Update(strTag, (double)newValue);
	}

	void JsonValue::Update(const std::wstring& strTag, const double newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		(*pObject)[stdkey] = newValue;
	}

	void JsonValue::Update(const std::wstring& strTag, const float newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		(*pObject)[stdkey] = newValue;
	}

	void JsonValue::Update(const std::wstring& strTag, const std::wstring& newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
		string stdValue = StringUtil::UNICODE_to_UTF8(newValue);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
		string stdValue;
		if (newValue.size() == 0) {
			stdValue = "";
		}
		else {
			stdValue = StringUtil::UnicodeToANSI(newValue);
		}

#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		(*pObject)[stdkey] = stdValue;
	}

	void JsonValue::Update(const std::wstring& strTag, JsonValue& newValue)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		if (!pObject->isMember(stdkey))
		{
			return;
		}

		Value* pValue = (Value*)newValue.GetValue();
		(*pObject)[stdkey] = *pValue;
	}

	void JsonValue::Update(const std::wstring& strTag, BoundingBox& newBbox)
	{
		JsonValue valBBox(objectValue);
		// "min" "max" 
		Vector3d vec = newBbox.GetMin();
		valBBox.Add(JSON_BBOX_MIN, vec);
		vec = newBbox.GetMax();
		valBBox.Add(JSON_BBOX_MAX, vec);

		Update(strTag, valBBox);
	}

	bool JsonValue::Remove(const std::wstring& strTag)
	{
		Value* pObject = (Value*)m_value;
		if (!pObject || !pObject->isObject())
		{
			return false;
		}

#ifdef WIN32
		string stdkey = StringUtil::UNICODE_to_UTF8(strTag);
#else
		string stdkey = StringUtil::UnicodeToANSI(strTag);
#endif

		pObject->removeMember(stdkey);
		return !pObject->isNull();
	}

	void JsonValue::SetValue(void* pValue)
	{
		if (m_isManaged)
		{
			delete (Value*)m_value;
			m_value = NULL;
		}
		m_value = pValue;
		m_isManaged = false;
	}

	void* JsonValue::GetValue()
	{
		return m_value;
	}
}
