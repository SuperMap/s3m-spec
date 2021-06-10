#include <iostream>
#include <codecvt>
#include <fstream>
#include <string>

#include "S3MBReader.h"
#include "S3MBWriter.h"
#include "ROGroupLite.h"
//#include "cpl_config.h"
#include <iosfwd>

#include "S3MBSCP.h"
using namespace std;
using namespace S3MB;

int main(int argc, char* argv[])
{

	///************************************************************************/
	//*          1.创建一个立方体数据并写入S3MB文件
	///************************************************************************/

	RenderOperationGroup* pGroup = ROGroupLite::CreateROGroupLite(0, L"./1.png");
	S3MBWriter m_S3MBWriter;
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true,true);//DXT压缩
	wstring strS3mbFile = L"./立方体0.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(1, L"./2.jpg");
	pGroup->ReComputeBoundingBox();//当包围盒/球不对或无包围盒/球时可调用
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true,true);//DXT压缩
	strS3mbFile = L"./立方体1.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(2, L"./3.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	strS3mbFile = L"./立方体2.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLite(3, L"./5.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	strS3mbFile = L"./立方体3.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	m_S3MBWriter.Clear();
	///************************************************************************/
	//*        2:通过指定的s3mb文件，读取出来Ro信息，并进行输出                                                                     */
   ///************************************************************************/


	S3MBReader m_S3MBReaderRo;

	wstring strS3MBFileSrc = L"./立方体0.s3mb";
	m_S3MBReaderRo.SetFilePath(strS3MBFileSrc);
	m_S3MBReaderRo.ReadS3MBAsROGroup();
	pGroup = m_S3MBReaderRo.GetRenderOperationGroup();
	///************************************************************************/
	//*       3:通过指定的s3mb缓存文件夹，遍历读取s3md并输出                                                                */
   ///************************************************************************/


	//S3MBReader m_S3MBReaderAtt;

	//string strS3MBFolderSrc = "..\\SampleData\\CBD_part";
	//m_S3MBReaderAtt.SetFilePath(strS3MBFolderSrc);
	//m_S3MBReaderAtt.ReadS3MBAttribute();



	//////////////////////生成SCP文件//////////////////
	//插入点
	Point3D m_Position(118, 39, 0);
	Rect2D geoBounds(0, 0, 0, 0);
	int epsgCode = 4326;
	bool isDegree = true;

	BoundingBox m_Box;
	m_Box.SetNULL();
	for (int i = 0; i < pGroup->GetNumChildren(); i++)
	{
		if (NULL != pGroup->GetChild(i)->AsGeode())
		{
			m_Box.Merge(((RenderOperationGeode*)(pGroup->GetChild(i)))->GetBoundingBox());
		}
	}
	///生成SCP文件//////////////////
	S3MBSCP* pS3MBSCP = new S3MBSCP;
	pS3MBSCP->m_strAsset = L"SuperMap";

	pS3MBSCP->m_pntPosition = m_Position;
	pS3MBSCP->m_rcGeoBounds = geoBounds;
	pS3MBSCP->m_prjCoordEPSG = epsgCode;
	pS3MBSCP->m_bIsDegree = isDegree;
	pS3MBSCP->m_dbHeightMax = m_Box.GetMax().z + m_Position.z;
	pS3MBSCP->m_dbHeightMin = m_Box.GetMin().z + m_Position.z;
	//pS3MBSCP->m_enDataType = SDT_ObliquePhoto;

	pS3MBSCP->m_mapExtensions[L"s3m:FileType"] = L"OSGBCacheFile";
	pS3MBSCP->m_mapExtensions[L"s3m:RenderMode"] = L"Normal";
	pS3MBSCP->m_mapExtensions[L"s3m:TileSplitType"] = L"LOCAL";

	std::vector<wstring> vecRootFiles;//根结点
	std::vector<BoundingBox> vecRootBoundboxs;//根结点对应BoundingBox
	vecRootFiles.push_back(L"立方体0.s3mb");
	vecRootBoundboxs.push_back(m_Box);

	pS3MBSCP->SetTiles(vecRootFiles, vecRootBoundboxs);
	wstring strOutputFilePath = StringHelper::GetAbsolutePath(strS3MBFileSrc);
	wstring strFilePatch = StringHelper::GetAbsolutePath(strOutputFilePath, L"../sample.scp");
	pS3MBSCP->SaveToJsonFile(strFilePatch);
	delete pS3MBSCP;
	pS3MBSCP = NULL;
	system("pause");
	return 0;
}