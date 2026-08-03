#include "ROGroupLite.h"

int main(int argc, char* argv[])
{
	bool bHasPBR = false;
	if (bHasPBR)
	{
		// PBR材质范例
		ROGroupLite::sampleV2();
	}
	else 
	{
		// 普通材质范例
		ROGroupLite::sampleV1(true);
	}

	// 高斯溅射数据范例
	//ROGroupLite::SampleGaussianSplatting();

	system("pause");
	return 0;
}