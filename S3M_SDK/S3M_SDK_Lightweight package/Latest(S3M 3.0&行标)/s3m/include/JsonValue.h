#if !defined(JSONVALUE_H__70C95238_8477_4144_B8F9_60E15D4D654E__INCLUDED_)
#define JSONVALUE_H__70C95238_8477_4144_B8F9_60E15D4D654E__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBCommon.h"
#include "Matrix.h"
#include "Vector.h"
#include "BoundingBox.h"
#include "Palette.h"

namespace S3MB
{
	class S3MB_API JsonValue
	{
	public:
		JsonValue();
		JsonValue(JsonValueType type, bool isManaged = true);
		// 通过Json字符串初始化自身
		JsonValue(wstring& strJsonContent);
		~JsonValue(void);

		// 通过字符串初始化自身
		bool LoadValueFromString(wstring& strJsonContent);
		bool LoadValueFromString(string& strJsonContent);
		bool LoadFromFile(const wstring& strFilePath);
		// 保存成字符串
		void SaveToString(wstring& strContent);
		void SaveToString(string& strContent);
		void SaveToFile(const wstring& strFilePath);

		// 获取指定标签的目标对象
		bool GetValue(const wstring& strTagw, JsonValue& object);
		bool GetValue(const wstring& strTagw, JsonValue*& pObject);
		bool GetValue(const wstring& strTagw, wstring& strValue);
		bool GetValue(const wstring& strTagw, ColorValue& color);
		bool GetValue(const wstring& strTagw, double& dbValue);
		bool GetValue(const wstring& strTagw, bool& bValue);
		bool GetValue(const wstring& strTagw, float& fValue);
		bool GetValue(const wstring& strTagw, int& nValue);
		bool GetValue(const wstring& strTagw, Vector3d& vec3D);
		bool GetValue(const wstring& strTagw, Vector4d& vec4D);
		bool GetValue(const wstring& strTagw, Matrix4d& mat);
		bool GetValue(const wstring& strTagw, Rect2D& rcBounds);
		bool GetValue(const wstring& strTagw, BoundingBox& bbox);
		bool GetValue(const wstring& strTagw, OrientedBoundingBox& box);
		bool GetXYZValue(const wstring& strTagw, double& dbX, double& dbY, double& dbZ);
		// 获取JsonValue数组的指定索引的JsonValue子对象
		bool GetAt(const int& i, JsonValue* pObject);
		bool GetAt(const int& i, std::wstring& strValue);
		bool GetAt(const int& i, double& dbValue);
		bool GetAt(const int& i, int& nValue);
		bool GetAt(const int& i, Matrix4d& mat);
		bool GetAt(const int& i, Vector3d& vec3D);
		bool GetAt(const int& i, Vector4d& vec4D);
		int GetArraySize();
		// 从Json对象取出所有的Key
		bool GetAllKeys(vector<wstring>& arr);

		void Add(const wstring& strTagw, JsonValue& value);
		void Add(const wstring& strTagw, const wstring& value);
		void Add(const wstring& strTagw, ColorValue nColor);
		void Add(const wstring& strTagw, const double& value);
		void Add(const wstring& strTagw, const int& value);
		void Add(const wstring& strTagw, Vector3d& vec3D);
		void Add(const wstring& strTagw, Vector4d& vec4D);
		void Add(const wstring& strTagw, Matrix4d& mat);
		void Add(const wstring& strTagw, Rect2D& rcBounds);
		void Add(const wstring& strTagw, BoundingBox& bbox);
		void Add(const wstring& strTagw, OrientedBoundingBox& bbox);

		void Update(const std::wstring& strTag, const bool newValue);
		void Update(const std::wstring& strTag, const int newValue);
		void Update(const std::wstring& strTag, const long newValue);
		void Update(const std::wstring& strTag, const double newValue);
		void Update(const std::wstring& strTag, const float newValue);
		void Update(const std::wstring& strTag, const std::wstring& newValue);
		void Update(const std::wstring& strTag, JsonValue& newValue);
		void Update(const std::wstring& strTag, BoundingBox& newBbox);

		void Add(const bool& value);
		void Add(const wstring& value);
		void Add(const int& value);
		void Add(const double& value);
		void Add(JsonValue& value);
		void Add(const Matrix4d& mat);
		void Add(const Vector3d& vec3D);
		void Add(const Vector4d& vec4D);

		// 删除对象
		bool Remove(const std::wstring& strTag);

		void SetValue(void* pValue);
		void* GetValue();
		// 包含对象
		bool Contains(const wstring& strTagw);

	private:
		void* m_value;
		bool m_isManaged;
	};
}

#endif
