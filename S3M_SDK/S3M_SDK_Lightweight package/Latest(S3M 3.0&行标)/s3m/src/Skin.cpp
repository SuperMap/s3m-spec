#include "Skin.h"
#include "Vector.h"
#include "Quaternion.h"

S3MB::Skin::~Skin(void)
{
	m_vecJointNodeIndex.clear();
	m_vecInverseBindMatrix.clear();
}

void S3MB::Skin::operator=(const Skin& other)
{
	m_strName = other.m_strName;
	m_vecJointNodeIndex = other.m_vecJointNodeIndex;
	m_vecInverseBindMatrix = other.m_vecInverseBindMatrix;
}

S3MB::Node::Node()
{
	m_nNumChildren = 0;
	m_pParent = NULL;
	m_nSkinIndex = -1;
	m_nPatchIndex = -1;
}

S3MB::Node::~Node(void)
{
	int i;
	for (i = 0; i < m_vecChildren.size(); i++)
	{
		if (m_vecChildren[i] != NULL)
		{
			ReleaseChild(m_vecChildren[i]);
		}
	}
	m_nNumChildren = 0;
	m_vecChildren.clear();

	m_nSkinIndex = -1;
	m_nPatchIndex = -1;
	m_vecChildNodeIndex.clear();
}

void S3MB::Node::ReleaseChild(Node* pNode)
{
	if (pNode != NULL)
	{
		int i;
		for (i = 0; i < pNode->GetNumChildren(); ++i)
		{
			Node* pChildNode = pNode->GetChild(i);
			if (pChildNode != NULL)
			{
				ReleaseChild(pChildNode);
			}
		}
		m_nNumChildren = 0;
		pNode->m_vecChildren.clear();
		pNode->m_nSkinIndex = -1;
		pNode->m_nPatchIndex = -1;
		pNode->m_vecChildNodeIndex.clear();
		delete pNode;
		pNode = NULL;
	}
}

void S3MB::Node::operator=(const Node& other)
{
	m_strName = other.m_strName;
	m_pParent = other.m_pParent;

	m_nNumChildren = other.m_nNumChildren;
	for (int i = 0; i < m_nNumChildren; ++i)
	{
		Node* pChildren = new Node();
		*pChildren = *other.m_vecChildren[i];
		m_vecChildren.push_back(pChildren);
	}
	m_nSkinIndex = other.m_nSkinIndex;
	m_nPatchIndex = other.m_nPatchIndex;
	m_vecChildNodeIndex = other.m_vecChildNodeIndex;
}
