#include <iostream>
#include <codecvt>
#include <fstream>
#include <string>

#include "S3MBReader.h"
#include "S3MBWriter.h"
#include "ROGroupLite.h"

#include <iosfwd>

#include "S3MBSCP.h"
using namespace std;
using namespace S3MB;

int main(int argc, char* argv[])
{	
	float fVersionNumber = 2.0;

	if (fVersionNumber == 1.0)
	{
		ROGroupLite::sampleV1(fVersionNumber);
	}
	else if (fVersionNumber == 2.0)
	{
		ROGroupLite::sampleV2(fVersionNumber);
	}
	system("pause");
	return 0;
}