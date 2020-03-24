#include <iostream>
#include <fstream>
#include "AltizureToS3MB.h"
#include "S3MBReader.h"
#include "S3MBWriter.h"

using namespace std;

int main(int argc, char* argv[])
{

	/************************************************************************/
	/*   1:通过指定的s3mb文件，读取出来Ro信息，并进行输出                                                                     */
	/************************************************************************/
	
	
	S3MBReader m_S3MBReaderRo;

	UGString strS3MBFileSrc = _U("..\\SampleData\\CBD_part\\Tile_-14620_42663_0000\\Tile_-14620_42663_0000.s3mb");	
	m_S3MBReaderRo.SetFilePath(strS3MBFileSrc);
	m_S3MBReaderRo.ReadS3MBAsROGroup();
	

	/************************************************************************/
	/*   2:通过指定的s3mb缓存文件夹，遍历读取s3md并输出                                                                */
	/************************************************************************/

	
	S3MBReader m_S3MBReaderAtt;

	UGString strS3MBFolderSrc =  _U("..\\SampleData\\CBD_part");
	m_S3MBReaderAtt.SetFilePath(strS3MBFolderSrc);
	m_S3MBReaderAtt.ReadS3MBAttribute();
	

	/************************************************************************/
	/*   3:设置Altizure数据作为源路径，设置s3mb缓路径，
	       将Altizure以Ro的方式写入到s3mb中 */
	/************************************************************************/
	
	UGString jsonFileName = _U("..\\SampleData\\cyt_model\\config.json");
	UGString outputFolder = _U("..\\SampleData\\cyt_models3mb");
	S3MBWriter m_S3MBWriter;
	m_S3MBWriter.SetJsonFileName(jsonFileName);
	m_S3MBWriter.SetOutPutFolder(outputFolder);
	m_S3MBWriter.AltizureTOS3MB();
	
	system("pause");
	return 0;
}