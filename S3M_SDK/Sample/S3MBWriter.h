#include <vector>
#include "Projection/UGPjCon.h"
#include "Base3D/UGMathEngine.h"
#include "Stream/ugdefs.h"
#include "Geometry/UGDataCodec.h"

#include "Base3D/UGBoundingSphere.h"
#include "Base3D/UGMatrix4d.h"
#include "Toolkit/UGMarkup.h"
#include "FileParser/UGExchangeFileType.h"

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
#include "Toolkit/UGInter.h"
#include "Toolkit3D/UGJsonUtils.h"
using namespace UGC;

class S3MBWriter
{
public:
	UGbool AltizureTOS3MB();

	void SetJsonFileName(const UGString & jsonFileName);
	
	void SetOutPutFolder(const UGString& outPutFolder);

	S3MBWriter();
	~S3MBWriter();

private:
	UGString m_jsonFileName;
    UGString m_outputFolder;
};