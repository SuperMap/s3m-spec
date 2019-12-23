#include "JsonParser.h"
#include <assert.h>
#include <codecvt>
#include "Skeleton.h"
#include <string>
#include <fstream>
#include "StringHelper.h"

JsonValue::JsonValue(ValueType type, bool isManaged)
{
	m_value = new Value(type);
	m_isManaged = isManaged;
}

JsonValue::JsonValue(string & strJsonContent)
{
	LoadValueFormString(strJsonContent);
}

JsonValue::~JsonValue(void)
{
	if (m_isManaged)
	{
		delete m_value;
		m_value = NULL;
	}
}

bool JsonValue::LoadValueFormString(string & strJsonContent)
{
	//strJsonContent = StringHelper::UTF8_to_ANSI(strJsonContent);
	this->m_value = new Value(ValueType::objectValue);
	Reader reader;
	if (!reader.parse(strJsonContent, *(Value *)this->m_value))
	{
		return false;
	}
	this->m_isManaged = true;
	return true;
}

bool JsonValue::LoadFromFile(const string& strFilePath)
{
	std::ifstream ifs(strFilePath, ios::in | ios::binary);
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
	this->LoadValueFormString(strJson);
	return true;
}

bool JsonValue::GetValue(const string strTag, JsonValue*& pObject)
{
	Value* object = (Value*)m_value;
	if (object == NULL)
	{
		return false;
	}
	if (!object || !object->isObject() || !object->isMember(strTag) || pObject)
	{
		return false;
	}
	Value &val = (*object)[strTag];
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

bool JsonValue::GetValue(const string strTag, string& strValue)
{
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
	Value &val = (*object)[strTag];
	if (!val.isString())
	{
		return false;
	}
	strValue = val.asString();
	return true;
}

bool JsonValue::GetValue(const string strTag, ColorValue& color)
{
	JsonValue *pObject = NULL;
	GetValue(strTag, pObject);
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

bool JsonValue::GetValue(const string strTag, double& dbValue)
{
	Value* object = (Value*)m_value;
	if (!object || !object->isObject() || !object->isMember(strTag))
	{
		return false;
	}
	Value &val = (*object)[strTag];
	if (!val.isDouble())
	{
		return false;
	}
	dbValue = val.asDouble();
	return true;
}

bool JsonValue::GetValue(const string strTag, bool& bValue)
{
	Value* object = (Value*)m_value;
	if (!object || !object->isObject() || !object->isMember(strTag))
	{
		return false;
	}
	Value &val = (*object)[strTag];
	if (!val.isBool())
	{
		return false;
	}
	bValue = val.asBool();
	return true;
}

bool JsonValue::GetValue(const string strTag, float& fValue)
{
	Value* object = (Value*)m_value;
	if (!object || !object->isObject() || !object->isMember(strTag))
	{
		return false;
	}
	Value &val = (*object)[strTag];
	if (!val.isDouble())
	{
		return false;
	}
	fValue = val.asDouble();
	return true;
}

bool JsonValue::GetValue(const string strTag, Vector3d& vec3D)
{
	return GetXYZValue(strTag, vec3D.x, vec3D.y, vec3D.z);
}

bool JsonValue::GetValue(const string strTag, Vector4d& vec4D)
{
	JsonValue *pObject = NULL;
	GetValue(strTag, pObject);
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

bool JsonValue::GetValue(const string strTag, int& nValue)
{
	Value* object = (Value*)m_value;
	if (!object || !object->isObject() || !object->isMember(strTag))
	{
		return false;
	}
	Value &val = (*object)[strTag];
	if (!val.isInt())
	{
		return false;
	}
	nValue = val.asInt();
	return true;
}

bool JsonValue::GetValue(const string strTag, Matrix4d& mat)
{
	JsonValue *pObject = NULL;
	GetValue(strTag, pObject);
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
		mat[i][0] = pValue->GetValue().asDouble();

		pObject->GetAt(i * 4 + 1, pValue);
		mat[i][1] = pValue->GetValue().asDouble();

		pObject->GetAt(i * 4 + 2, pValue);
		mat[i][2] = pValue->GetValue().asDouble();

		pObject->GetAt(i * 4 + 3, pValue);
		mat[i][3] = pValue->GetValue().asDouble();
	}

	delete pValue;
	pValue = NULL;
	delete pObject;
	pObject = NULL;
	return true;
}

bool JsonValue::GetXYZValue(const string strTag, double& dbX, double& dbY, double& dbZ)
{
	JsonValue *pObject = NULL;
	GetValue(strTag, pObject);
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

bool JsonValue::GetAt(const int i, JsonValue* pObject)
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

bool JsonValue::GetAt(const int i, double& dbValue)
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

bool JsonValue::GetAt(const int i, int& nValue)
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

int JsonValue::GetArraySize()
{
	if (!(Value*)m_value || !((Value*)m_value)->isArray())
	{
		return false;
	}

	return ((Value*)m_value)->size();
}

void JsonValue::Add(const bool value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isArray())
	{
		return;
	}

	pObject->append(Value((bool)value));
}

void JsonValue::Add(const string& value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isArray())
	{
		return;
	}
	pObject->append(Value(value));
}

void JsonValue::Add(const int value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isArray())
	{
		return;
	}

	pObject->append(Value(value));
}

void JsonValue::Add(const double value)
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
	pObject->append(value.GetValue());
}

void JsonValue::Add(const string strTag, JsonValue& value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isObject())
	{
		return;
	}

	(*pObject)[strTag] = value.GetValue();
}

void JsonValue::Add(const string strTag, const string &value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isObject())
	{
		return;
	}
	(*pObject)[strTag] = value;
}

void JsonValue::Add(const string strTag, ColorValue nColor)
{
	JsonValue jsonColor(objectValue);
	jsonColor.Add(JSON_COLOR_R, nColor.r);
	jsonColor.Add(JSON_COLOR_G, nColor.g);
	jsonColor.Add(JSON_COLOR_B, nColor.b);
	jsonColor.Add(JSON_COLOR_A, nColor.a);

	Add(strTag, jsonColor);
}

void JsonValue::Add(const string strTag, const double value)
{
	Value* pObject = (Value*)m_value;
	if (!pObject || !pObject->isObject())
	{
		return;
	}
	(*pObject)[strTag] = value;
}

void JsonValue::Add(const string strTag, Vector3d& vec3D)
{
	//"x" "y" "z" 
	JsonValue valPnt(objectValue);
	valPnt.Add(JSON_POS_X, vec3D.x);
	valPnt.Add(JSON_POS_Y, vec3D.y);
	valPnt.Add(JSON_POS_Z, vec3D.z);

	Add(strTag, valPnt);
}

void JsonValue::Add(const string strTag, Vector4d& vec4D)
{
	//"x" "y" "z" 
	JsonValue valPnt(objectValue);
	valPnt.Add(JSON_POS_X, vec4D.x);
	valPnt.Add(JSON_POS_Y, vec4D.y);
	valPnt.Add(JSON_POS_Z, vec4D.z);
	valPnt.Add(JSON_POS_W, vec4D.w);
	Add(strTag, valPnt);
}

void JsonValue::Add(const string strTag, Matrix4d& mat)
{
	JsonValue jsonArrary(arrayValue);
	jsonArrary.Add(mat[0][0]);	jsonArrary.Add(mat[0][1]);	jsonArrary.Add(mat[0][2]);	jsonArrary.Add(mat[0][3]);
	jsonArrary.Add(mat[1][0]);	jsonArrary.Add(mat[1][1]);	jsonArrary.Add(mat[1][2]);	jsonArrary.Add(mat[1][3]);
	jsonArrary.Add(mat[2][0]);	jsonArrary.Add(mat[2][1]);	jsonArrary.Add(mat[2][2]);	jsonArrary.Add(mat[2][3]);
	jsonArrary.Add(mat[3][0]);	jsonArrary.Add(mat[3][1]);	jsonArrary.Add(mat[3][2]);	jsonArrary.Add(mat[3][3]);

	Add(strTag, jsonArrary);
}

void JsonValue::Add(const string strTag, Rect2D& rcBounds)
{
	JsonValue valGeoBounds(objectValue);
	valGeoBounds.Add(JSON_RECT_LEFT, rcBounds.left);
	valGeoBounds.Add(JSON_RECT_TOP, rcBounds.top);
	valGeoBounds.Add(JSON_RECT_RIGHT, rcBounds.right);
	valGeoBounds.Add(JSON_RECT_BOTTOM, rcBounds.bottom);

	Add(strTag, valGeoBounds);
}

void JsonValue::Add(const string strTag, BoundingBox& bbox)
{
	JsonValue valBBox(objectValue);
	//"min" "max" 
	Vector3d vec = bbox.GetMin();
	valBBox.Add(JSON_BBOX_MIN, vec);
	vec = bbox.GetMax();
	valBBox.Add(JSON_BBOX_MAX, vec);

	Add(strTag, valBBox);
}

void JsonValue::SetValue(Value* pValue)
{
	if (m_isManaged)
	{
		delete m_value;
		m_value = NULL;
	}
	m_value = pValue;
	m_isManaged = false;
}

Value& JsonValue::GetValue()
{
	return *(Value*)m_value;
}

void ColorValue::SetValue(double dRed, double dGreen, double dBlue, double dAlpha)
{
	r = dRed;
	g = dGreen;
	b = dBlue;
	a = dAlpha;
}

void ColorValue::SetValue(int nRed, int nGreen, int nBlue, int nAlpha)
{
	r = nRed / 255.0;
	g = nGreen / 255.0;
	b = nBlue / 255.0;
	a = nAlpha / 255.0;
}

UGColor ColorValue::GetValue() const
{
	return Palette::RGBA(unsigned char(r * 255), unsigned char(g * 255), unsigned char(b * 255), unsigned char(a * 255));
}
