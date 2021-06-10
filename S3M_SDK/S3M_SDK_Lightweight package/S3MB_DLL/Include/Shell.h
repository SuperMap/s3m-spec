#ifndef  _SHELL_H_
#define  _SHELL_H_
#include "Texture.h"
#include "Material.h"
#include "Skeleton.h"
#include <map>
using namespace std;
namespace S3MB
{

	//! \brief LOD切换模式
	enum RangeMode
	{
		DISTANCE_FROM_EYE_POINT, // 根据到相机的距离切换
		PIXEL_SIZE_ON_SCREEN	 // 根据屏幕像素大小切换
	};
	class S3MB_API BoundingSphere
	{
	public:
		BoundingSphere()
			:m_center(0.0, 0.0, 0.0), m_radius(-1) {}

		BoundingSphere(const Vector3d& center, double radius)
			:m_center(center), m_radius(radius) {}

		BoundingSphere(const BoundingSphere& bs)
			:m_center(bs.m_center), m_radius(bs.m_radius) {}

		BoundingSphere(const BoundingBox& box)
			:m_center(box.GetCenter()), m_radius((box.GetMax() - box.GetMin()).Length() / 2) {}

		~BoundingSphere(void) {}

		//! \brief 判断包围球是否有效
		inline bool valid() const
		{
			return m_radius > 0.0;
		}

		//! \brief 获取包围盒
		inline BoundingBox GetBoundingBox() const
		{
			Vector3d vMinVec(m_center.x - m_radius, m_center.y - m_radius, m_center.z - m_radius);
			Vector3d vMaxVec(m_center.x + m_radius, m_center.y + m_radius, m_center.z + m_radius);
			return BoundingBox(vMinVec, vMaxVec);
		}

		inline void init()
		{
			m_center = Vector3d(0.0, 0.0, 0.0);
			m_radius = -1;
		}

		inline Vector3d GetCenter() const
		{
			return m_center;
		}

		inline double GetRadius() const
		{
			return m_radius;
		}

		//！ \brief 给据给定的包围球来扩展，球心、半径都改变
		//!  \param 待扩展的球
		void ExpandBy(const BoundingSphere& bs);
	public:
		Vector3d m_center;
		double m_radius;
	};

	class RenderOperationGroup;
	class RenderOperationGeode;

	class S3MB_API RenderOperationNode
	{
	public:
		RenderOperationNode() {}
		virtual ~RenderOperationNode()
		{
			RemoveAllMaterial();
			RemoveAllTexData();
			RemoveAllGeometry();
		}

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup() { return NULL; };

		//! \brief 如果是Geode对象就返回this，否则NULL
		virtual RenderOperationGeode* AsGeode() { return NULL; };

		//! \brief 类名
		virtual wstring ClassName() { return L"Node"; }

		//! \brief 名字
		void SetName(wstring strName) { m_strName = strName; };

		//! \brief 获得名字
		wstring GetName() { return m_strName; };

		//! \brief 获得包围球
		BoundingSphere GetBoudingSphere() { return m_BoundingSphere; };

		//! \brief 设置包围球
		void SetBoudingSphere(Vector3d vecCenter, double dRadius) {
			m_BoundingSphere.m_center = vecCenter;
			m_BoundingSphere.m_radius = dRadius;
		};

		//! \brief 设置位置
		void SetPosition(Matrix4d mat) { m_matLocalView = mat; };

		//! \brief 获得位置
		Matrix4d GetPosition() { return m_matLocalView; };

		//! \brief 材质是否存在
		bool IsExistToMaterial(wstring strMaterialName);

		//! \brief 纹理是否存在
		bool IsExistToTexData(wstring strTextureName);

		//! \brief Node是否存在
		bool IsExistToGeometry(wstring strName);

		//! \brief 添加一个材质
		//! \param strMaterialName 材质名，可以和Material3D里的名字不对应[in]
		//! \param pMaterial3D 材质信息[in]
		//! \return 重复材质返回false[out]
		bool AddMaterial(wstring strMaterialName, Material* pMaterial);

		//! \brief 添加一个纹理信息
		//! \param strTextureName 纹理名[in]
		//! \param pTexData 纹理信息[in]
		//! \return 重复纹理返回false[out]
		bool AddTextureData(wstring strTextureName, TextureDataInfo* pTexData);

		//! \brief 添加一个Geometry信息
		//! \param strName Geometry名[in]
		//! \param pGeometry Geometry信息[in]
		//! \return 重复返回false[out]
		bool AddGeometry(wstring strName, Geometry* pGeometry);

		//! \brief 获得材质
		Material* GetMaterial(wstring strMaterialName);
		//! \brief 获得材质
		std::map<wstring, Material*>& GetMaterials();

		//! \brief 获得纹理
		TextureDataInfo* GetTextureData(wstring strTextureName);
		//! \brief 获得所有纹理
		std::map<wstring, TextureDataInfo*>& GetTextureData();

		//! \brief 获得Geometry
		Geometry* GetGeometry(wstring strName);

		//! \brief 获得Geometry
		std::map<wstring, Geometry*>& GetGeometry();

		//! \brief 移除所有材质
		void RemoveAllMaterial();

		//! \brief 移除所有纹理
		void RemoveAllTexData();

		//! \brief 移除所有Geometry
		void RemoveAllGeometry();

		//! \brief 移除指定Geometry
		virtual void RemoveGeometry(wstring strName);

		//! \brief 清空所有材质
		void ClearMaterial3D() { m_mapMaterial.clear(); };

		//! \brief 清空所有纹理
		void ClearTexData() { m_mapTextureData.clear(); };

		//! \brief 清空所有Geometry
		void ClearGeometry() { m_mapGeometry.clear(); };
	protected:
		//! \brief 包围球
		BoundingSphere m_BoundingSphere;

		//! \brief 名字
		wstring m_strName;

		//! \brief 位置
		Matrix4d m_matLocalView;

		//! \brief 材质信息
		std::map<wstring, Material*> m_mapMaterial;

		//! \brief 纹理信息
		std::map<wstring, TextureDataInfo*> m_mapTextureData;

		//! \brief Node信息
		std::map<wstring, Geometry*> m_mapGeometry;
	};

	class S3MB_API RenderOperationGroup : public RenderOperationNode
	{
	public:
		RenderOperationGroup()
			:m_pParentNode(NULL) {}
		virtual ~RenderOperationGroup()
		{
			RemoveAllChildren();
			m_pParentNode = NULL;
		}

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup() { return this; };

		//! \brief 类名
		virtual wstring ClassName() { return L"Group"; }

		//! \brief 设置父节点
		void SetParentNode(RenderOperationNode* pNode) { m_pParentNode = pNode; }

		//! \brief 获得父节点
		RenderOperationNode* GetParentNode() { return m_pParentNode; }

		//! \brief 添加子节点
		void AddChild(RenderOperationNode* pNode);

		//! \brief 获得子节点数量
		int GetNumChildren() { return m_vecChildNode.size(); }

		//! \brief 按索引获得子节点
		//! \param nIndex 索引[in]
		//! \return Node[out]
		RenderOperationNode* GetChild(int nIndex);

		//! \brief 移除所有节点
		void RemoveAllChildren();

		//! \brief 重新计算所有child包围盒
		//! \param bReCalGeometryBBOX 是否重新计算geometry的包围盒
		virtual void ReComputeBoundingBox(bool bReCalGeometryBBOX = true);
	private:
		//! \brief 父节点
		RenderOperationNode* m_pParentNode;

		//! \brief 子节点
		std::vector<RenderOperationNode*> m_vecChildNode;
	};

	class S3MB_API RenderOperationPagedLOD : public RenderOperationGroup
	{
	public:
		RenderOperationPagedLOD()
			:m_nRangeMode(PIXEL_SIZE_ON_SCREEN) {}

		virtual ~RenderOperationPagedLOD();

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGroup* AsGroup() { return this; };

		//! \brief 类名
		virtual wstring ClassName() { return L"PagedLOD"; }

		//! \brief 获得范围
		std::vector<std::pair<float, float> >& GetRanges() { return m_vecRangeList; }

		//! \brief 获得子切片
		std::vector<wstring>& GetFileNames() { return m_vecFileName; }

		//! \brief 获取子切片的包围球
		std::map<wstring, BoundingSphere>& GetLODSphere() {
			return m_mapBoundingSphere;
		}

		//! \brief 切换范围模式
		void SetRangeMode(RangeMode nMode) { m_nRangeMode = nMode; };

		//! \brief 获得切换范围模式
		RangeMode GetRangeMode() { return m_nRangeMode; };

		//! \brief 重新计算所有child包围盒，并计算自身包围球
		//! \param bReCalGeometryBBOX 是否重新计算geometry的包围盒
		void ReComputeBoundingBox(bool bReCalGeometryBBOX = true) override;

	private:
		//! \brief 切换范围
		std::vector<std::pair<float, float> > m_vecRangeList;

		//! \brief LOD切片名
		std::vector<wstring> m_vecFileName;

		//! \brief LOD切片包围盒
		std::map<wstring, BoundingSphere> m_mapBoundingSphere;

		//! \brief 切换范围模式
		RangeMode m_nRangeMode;
	};

	//属性表
	struct GeodeInfo
	{
		wstring m_strTableName;
		unsigned int m_nID;
	};

	class S3MB_API RenderOperationGeode : public RenderOperationNode
	{
	public:
		RenderOperationGeode() {}
		virtual ~RenderOperationGeode() { Release(); }

		//! \brief 如果是Group对象就返回this，否则NULL
		virtual RenderOperationGeode* AsGeode() { return this; };

		//! \brief 类名
		virtual wstring ClassName() { return L"Geode"; }

		//! \brief 添加Geometry
		//! \param pROGeoemtry Geometry[in]
		void AddGeometry(Geometry* pROGeoemtry);

		//! \brief 获得Geoemtry总数
		int GetNumGeometry() { return m_vecROGeometry.size(); }

		//! \brief 获得Geometry
		//! \param nIndex 索引[in]
		//! \return Geometry[out]
		Geometry* GetGeometry(int nIndex);

		//! \brief 替换Geometry
		//! \param 替换对象索引[in]
		//! \param 替换的Geometry[in]
		//! \param 是否删除原对象
		void ReplaceGeometry(int nIndex, Geometry* pGeometry, bool bRelease = false);

		//! \brief 移除指定Geometry
		virtual void RemoveGeometry(wstring strName);

		//! \brief 清理Geometry
		void Release();

		//! \brief 计算包围盒
		void ComputerBoundingBox();

		//! \brief 获得包围盒
		BoundingBox GetBoundingBox() { return m_BoundingBox; }

	private:
		//! \brief 包围盒
		BoundingBox m_BoundingBox;

		//! \brief Geoemtry数据
		std::vector<Geometry*> m_vecROGeometry;

	public:
		//! \brief 属性表
		GeodeInfo m_GeodeInfo;

		//! \brief ID
		wstring m_strUniqueID;
	};
}

#endif