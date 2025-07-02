#if !defined(SHELL_H__01180357_13FC_4EA5_965C_09325C4EA952__INCLUDED_)
#define SHELL_H__01180357_13FC_4EA5_965C_09325C4EA952__INCLUDED_

#pragma once
#include "Texture.h"
#include "Material.h"
#include "Skeleton.h"
#include "S3MBCommon.h"
#include "BoundingSphere.h"
#include <map>
#include "Skin.h"
#include "Animation.h"

using namespace std;
namespace S3MB
{
	class RenderOperationGroup;
	class RenderOperationGeode;

	class S3MB_API RenderOperationNode
	{
	public:
		RenderOperationNode();
		virtual ~RenderOperationNode();

		// 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup();

		// 如果是Geode对象就返回this，否则NULL
		virtual RenderOperationGeode* AsGeode();

		// 类名
		virtual wstring ClassName();

		// 名字
		void SetName(wstring strName);

		// 获得名字
		wstring GetName();

		// 获得包围球
		BoundingSphere GetBoundingSphere();

		// 设置包围球
		void SetBoundingSphere(Vector3d vecCenter, double dRadius);

		// 获得方向包围盒
		OrientedBoundingBox GetOrientedBoundingBox();

		// 设置方向包围盒
		void SetOrientedBoundingBox(OrientedBoundingBox orientedBoundingBox);

		BoundingBox GetBoundingBox();

		void SetBoundingBox(BoundingBox boundingBox);

		// 设置位置
		void SetPosition(Matrix4d mat);

		// 获得位置
		Matrix4d GetPosition();

		// 材质是否存在
		bool IsMaterialExist(wstring strMaterialName);

		// 纹理是否存在
		bool IsTexDataExist(wstring strTextureName);

		// Node是否存在
		bool IsSkeletonExist(wstring strName);
		  
		// 添加一个材质
		// strMaterialName 材质名，可以和Material3D里的名字不对应[in]
		// pMaterial3D 材质信息[in]
		// return 重复材质返回false[out]
		bool AddMaterial(wstring strMaterialName, Material* pMaterial);

		// 添加一个纹理信息
		// strTextureName 纹理名[in]
		// pTexData 纹理信息[in]
		// return 重复纹理返回false[out]
		bool AddTextureData(wstring strTextureName, TextureDataInfo* pTexData);

		// 添加一个Skeleton信息
		// strName Skeleton名[in]
		// pSkeleton Skeleton信息[in]
		// return 重复返回false[out]
		bool AddSkeleton(wstring strName, Skeleton* pSkeleton);

		// 获得材质
		Material* GetMaterial(wstring strMaterialName);
		// 获得材质
		std::map<wstring, Material*>& GetMaterials();

		// 获得纹理
		TextureDataInfo* GetTextureData(wstring strTextureName);
		// 获得所有纹理
		std::map<wstring, TextureDataInfo*>& GetTextureData();

		// 获得Skeleton
		Skeleton* GetSkeleton(wstring strName);

		// 获得Skeleton
		std::map<wstring, Skeleton*>& GetSkeleton();

		// 移除所有材质
		void RemoveAllMaterial();

		// 移除所有纹理
		void RemoveAllTexData();

		// 移除所有Skeleton
		void RemoveAllSkeleton();

		// 移除指定Skeleton
		virtual void RemoveSkeleton(wstring strName);

		// 清空所有材质
		void ClearMaterial3D();

		// 清空所有纹理
		void ClearTexData();

		// 清空所有Skeleton
		void ClearSkeleton();

	protected:
		// 包围球
		BoundingSphere m_BoundingSphere;

		// 方向包围盒
		OrientedBoundingBox m_OBB;

		// 包围盒
		BoundingBox m_BoundingBox;

		// 名字
		wstring m_strName;

		// 位置
		Matrix4d m_matLocalView;

		// 材质信息
		std::map<wstring, Material*> m_mapMaterial;

		// 纹理信息
		std::map<wstring, TextureDataInfo*> m_mapTextureData;

		// Node信息
		std::map<wstring, Skeleton*> m_mapSkeleton;
	};

	class S3MB_API RenderOperationGroup : public RenderOperationNode
	{
	public:
		RenderOperationGroup();

		virtual ~RenderOperationGroup();

		// 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup();

		// 类名
		virtual wstring ClassName();

		// 设置父节点
		void SetParentNode(RenderOperationNode* pNode);

		// 获得父节点
		RenderOperationNode* GetParentNode();

		// 添加子节点
		void AddChild(RenderOperationNode* pNode);

		// 获得子节点数量
		int GetNumChildren();

		// 按索引获得子节点
		// nIndex 索引[in]
		// return Node[out]
		RenderOperationNode* GetChild(int nIndex);

		// 移除所有节点
		void RemoveAllChildren();

		// 重新计算所有child包围盒
		// bReCalSkeletonBBOX 是否重新计算Skeleton的包围盒
		virtual void ReComputeBoundingBox(bool bReCalSkeletonBBOX = true);

	private:
		// 父节点
		RenderOperationNode* m_pParentNode;

		// 子节点
		std::vector<RenderOperationNode*> m_vecChildNode;
	};

	class S3MB_API RenderOperationPagedLOD : public RenderOperationGroup
	{
	public:
		RenderOperationPagedLOD();

		virtual ~RenderOperationPagedLOD();

		// 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup();

		// 类名
		virtual wstring ClassName();

		// 获得范围
		std::vector<std::pair<float, float> >& GetRanges();

		// 获得子切片
		std::vector<wstring>& GetFileNames();

		// 切换范围模式
		void SetRangeMode(RangeMode nMode);

		// 获得切换范围模式
		RangeMode GetRangeMode();

		// 重新计算所有child包围盒，并计算自身包围球
		// bReCalSkeletonBBOX 是否重新计算Skeleton的包围盒
		void ReComputeBoundingBox(bool bReCalSkeletonBBOX = true) override;

		// 设置/获取节点对象的数组
		void SetNodes(const std::vector<Node*>& vecNode, bool bOwn = true);
		const std::vector<Node*>& GetNodes() const { return m_vecNode; }

		// 设置/获取蒙皮对象的数组
		void SetSkins(const std::vector<Skin*>& vecSkin, bool bOwn = true);
		const std::vector<Skin*>& GetSkins() const { return m_vecSkin; };

		// 设置/获取所有动画的整体信息
		void SetAnimationStates(const std::vector<Animation*>& vecAnimation, bool bOwn = true);
		const std::vector<Animation*>& GetAnimationStates() const { return m_vecAnimationState; };

		// 设置/获取所有动画信息
		void SetAnimationCurves(const std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve, bool bOwn = true);
		const std::vector<std::map<wstring, AnimationCurve*> >& GetAnimationCurves() const { return m_vecAnimationCurve; };

	private:
		// 切换范围
		std::vector<std::pair<float, float> > m_vecRangeList;

		// LOD切片名
		std::vector<wstring> m_vecFileName;

		// LOD切片包围盒
		std::map<wstring, BoundingSphere> m_mapBoundingSphere;

		// 切换范围模式
		RangeMode m_nRangeMode;

		// 节点对象的数组
		std::vector<Node*> m_vecNode;
		bool m_bOwnNode;

		// 蒙皮对象的数组
		std::vector<Skin*> m_vecSkin;
		bool m_bOwnSkin;

		// 动画状态
		std::vector<Animation*> m_vecAnimationState;
		bool m_bOwnAnimationState;

		// 动画信息，vector和动画状态的vector对应，map的key值是动画对应的Geode的名字
		std::vector<std::map<wstring, AnimationCurve*> > m_vecAnimationCurve;
		bool m_bOwnAnimationCurve;
	};

	class S3MB_API RenderOperationGeode : public RenderOperationNode
	{
	public:
		RenderOperationGeode();
		virtual ~RenderOperationGeode();

		// 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGeode* AsGeode();

		// 类名
		virtual wstring ClassName();

		// 添加Skeleton
		// pROGeoemtry Skeleton[in]
		void AddSkeleton(Skeleton* pROGeoemtry);

		// 获得Geoemtry总数
		int GetNumSkeleton();

		// 获得Skeleton
		// nIndex 索引[in]
		// return Skeleton[out]
		Skeleton* GetSkeleton(int nIndex);

		// 替换Skeleton
		// 替换对象索引[in]
		// 替换的Skeleton[in]
		// 是否删除原对象
		void ReplaceSkeleton(int nIndex, Skeleton* pSkeleton, bool bRelease = false);

		// 移除指定Skeleton
		virtual void RemoveSkeleton(wstring strName);

		// 清理Skeleton
		void Release();

		// 计算包围盒
		void ComputerBoundingBox();

	private:
		// Skeleton数据
		std::vector<Skeleton*> m_vecSkeleton;

	public:
		// 属性表
		GeodeInfo m_GeodeInfo;

		// ID
		wstring m_strUniqueID;
	};
}

#endif