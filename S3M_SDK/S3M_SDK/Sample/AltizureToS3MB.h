#pragma once
#include <vector>
#include "Stream/ugdefs.h"

#include "Base3D/UGBoundingBox.h"
#include "Base3D/UGBoundingSphere.h"
#include "Base3D/UGModelNode.h"
#include "Base3D/UGMathEngine.h"

#include "FileParser/UGFileParse.h"
#include "FileParser/UGFileParseModel.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGModelConfigParams.h"

#include "Toolkit/UGMarkup.h"

#include "Toolkit/UGHash.h"
#include "Toolkit/UGLogFile.h"
#include "Toolkit/UGInter.h"

#include "Toolkit3D/UGJsonUtils.h"

using namespace UGC;

class AltizureToS3MB
{
public:
	struct OBJLodQuarterTreeNode
	{
		UGuint nLod;
		UGuint nRow;
		UGuint nCol;
		// 节点名字
		UGString strFileName; 
		// 子节点
		std::vector<OBJLodQuarterTreeNode*> childs;	

		UGBoundingBox boundBox;
	};

public:
	AltizureToS3MB();
	~AltizureToS3MB();

	// 设置配置json文件名
	void SetFileName(const UGString & strFile);

	// 设置输出文件路径
	void SetOutputFilePath(const UGString & strOutputPath);

	// 解析json文件
	void ParseJson();

	// 生成SCP配置文件
	void GenerateScp();

	// 转换生成
	void GenerateS3MB();

	//设置LOD层数
	void SetLodNum(UGint LodNum)
	{
		nLodNum = LodNum;
	}
	//设置Tile长度
	void SetTileLength(UGint TileLength)
	{
		nTileLength = TileLength;
	}
	//设置插入掉位置
	void SetPostion(UGPoint3D Postion)
	{
		m_postion = Postion;
	}
	void SetProjection(EmPrjCoordSysType PriCoordSysType)
	{
		priCoordSysType = PriCoordSysType;
	}

private:
	void Convert(OBJLodQuarterTreeNode* pNode);
	void Convert2RO(OBJLodQuarterTreeNode* pNode);
	void AppendRO(UGRenderOperationGroup* pGroupSrc, 
		UGRenderOperationGroup* pGroupDes, UGRenderOperationGeode *pGeodeDes);
	UGRenderOperationGroup* GetRO(const UGString strObjFile);
	void WriteRO(UGRenderOperationGroup* pGroup, const UGString strDesFile);

	void MakeQuarterTree();
	void GetQuarterChild(UGuint nLod,OBJLodQuarterTreeNode* pNode,UGuint nMinRow, UGuint nMaxRow,UGuint nMinCol,UGuint nMaxCol);
	OBJLodQuarterTreeNode* CreateQTreeNode(UGuint nLod, UGuint nRow, UGuint nCol);

private:
	void deleteQTreeNode(OBJLodQuarterTreeNode* pNode);
private:
	// 配置源文件
	UGString strConfigJson;
	// 输出文件路径
	UGString strOutputFilePath;
	// LOD层数
	UGuint nLodNum;
	// Tile长度
	UGuint nTileLength;
	// 插入点
	UGPoint3D m_postion;
	// 四分树
	OBJLodQuarterTreeNode* m_pTreeNode;
	// 投影坐标系
	EmPrjCoordSysType priCoordSysType;

private:
	UGProgress *m_pProgress;
	UGint m_nTotalRecCount;
	UGint m_nPrecent;
	UGint m_nCurrentIndex;
};