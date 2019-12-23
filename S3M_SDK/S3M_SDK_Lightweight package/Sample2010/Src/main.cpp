#include <iostream>
#include <codecvt>
#include <fstream>
#include <string>

#include "S3MBReader.h"
#include "S3MBWriter.h"
#include "ROGroupLite.h"
#include <iosfwd>

using namespace std;
int main(int argc, char* argv[])
{

	///************************************************************************/
	//*          1.创建一个立方体数据并写入S3MB文件
    ///************************************************************************/	

	RenderOperationGroup* pGroup = ROGroupLite::CreateROGroupLite();

	S3MBWriter m_S3MBWriter;
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true);
	string strS3mbFile = "./立方体.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);

	///************************************************************************/
    //*        2:通过指定的s3mb文件，读取出来Ro信息，并进行输出                                                                     */
   ///************************************************************************/


	S3MBReader m_S3MBReaderRo;

	string strS3MBFileSrc = "./立方体.s3mb";
	//cout << "请输入需要显示的S3mb路径：" << endl;
	//string strS3MBFileSrc;
	//cin >> strS3MBFileSrc;
	m_S3MBReaderRo.SetFilePath(strS3MBFileSrc);
	m_S3MBReaderRo.ReadS3MBAsROGroup();

	///************************************************************************/
    //*       3:通过指定的s3mb缓存文件夹，遍历读取s3md并输出                                                                */
   ///************************************************************************/


	//S3MBReader m_S3MBReaderAtt;

	//string strS3MBFolderSrc = "..\\SampleData\\CBD_part";
	//m_S3MBReaderAtt.SetFilePath(strS3MBFolderSrc);
	//m_S3MBReaderAtt.ReadS3MBAttribute();
}