#include "ROGroupLite.h"

int main(int argc, char* argv[])
{	
	bool bHasPBR = false;
	if (bHasPBR)
	{
		// PBR²ÄÖÊ·¶Àı
		ROGroupLite::sampleV2();
	}
	else 
	{
		// ÆÕÍ¨²ÄÖÊ·¶Àı
		ROGroupLite::sampleV1(true);
	}
	system("pause");
	return 0;
}