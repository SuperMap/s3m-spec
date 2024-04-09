#pragma once
#ifndef S3MB_SKIN
#define S3MB_SKIN

#pragma once
#include "Matrix.h"
#include <string>
#include <vector>
#include "Quaternion.h"

namespace S3MB
{
	class S3MB_API Skin
	{
	public:
		// 构造函数
		Skin() {};

		// 析构函数
		~Skin(void);

		// 重载等号
		void operator=(const Skin& other);

		// 设置/获取蒙皮的名称
		void SetName(const wstring& strName) { m_strName = strName; };
		const wstring& GetName() const { return m_strName; };

		// 设置/获取节点的索引数组
		void SetJointNodeIndexes(const std::vector<int>& vecIndex) { m_vecJointNodeIndex = vecIndex; };
		const std::vector<int>& GetJointNodeIndexes() const { return m_vecJointNodeIndex; };

		// 设置/获取逆矩阵的数组
		void SetInverseBindMatrices(const std::vector<Matrix4d>& vecMatrix) { m_vecInverseBindMatrix = vecMatrix; };
		const std::vector<Matrix4d>& GetInverseBindMatrices() const { return m_vecInverseBindMatrix; };


	private:
		// 名称
		wstring m_strName;

		// 节点的索引数组
		vector<int> m_vecJointNodeIndex;

		// 逆矩阵的数组
		vector<Matrix4d> m_vecInverseBindMatrix;
	};

	class S3MB_API Node
	{
	public:
		// 构造函数。
		Node();

		// 构造函数。
		~Node(void);

		// 重载等号。
		void operator=(const Node& other);

		// 设置/获取结点的名称。
		void SetName(const wstring& strName) { m_strName = strName; };
		wstring& GetName(void) { return m_strName; };

		// 设置/获取父结点。
		void SetParent(Node* pParent) { m_pParent = pParent; };
		Node* GetParent() { return m_pParent; };

		// 获取子节点个数。
		int GetNumChildren(void) { return m_nNumChildren; };
		// 获取子节点。
		Node* GetChild(int nIndex) { return m_vecChildren.size() > nIndex ? m_vecChildren[nIndex] : nullptr; };

		// 设置/获取蒙皮的索引
		void SetSkinIndex(int nIndex) { m_nSkinIndex = nIndex; };
		int GetSkinIndex() const { return m_nSkinIndex; };

		// 设置/获取Geode的索引
		void SetPatchIndex(int nIndex) { m_nPatchIndex = nIndex; };
		int GetPatchIndex() const { return m_nPatchIndex; };

		// 设置/获取子节点在nodes数组中的索引构成的数组
		void SetChildNodeIndexes(const std::vector<int>& vecIndex) { m_vecChildNodeIndex = vecIndex; };
		const std::vector<int>& GetChildNodeIndexes() const { return m_vecChildNodeIndex; };

	protected:
		// 释放子结点
		void ReleaseChild(Node* pNode);

		// 结点名称，作为结点的唯一标识
		wstring m_strName;

		// 子结点个数
		int  m_nNumChildren;

		// 子结点数组
		vector<Node*> m_vecChildren;

		// 父结点
		Node* m_pParent;

		// 蒙皮的索引
		int m_nSkinIndex;

		// Geode的索引
		int m_nPatchIndex;

		// 子节点在nodes数组中的索引构成的数组
		vector<int> m_vecChildNodeIndex;
	};

}

#endif