// pRaster比pResult多两行
#pragma OPENCL EXTENSION cl_khr_fp64: enable

__kernel void
Hillshade_Kernel
( 
	__global const float *pRaster,
	__global float* pResult,
	unsigned int nWidth,
	unsigned int nHeight,
	double dNoValue,
	double dCellSizeX, double dCellSizeY,
	double dAzimuth, double dAltitude,
	double dZFactor)
{
	float pdZ[8];
	
	int resultIndex;
	float dSlopewe = 0.0;
	float dSlopesn = 0.0;
	float dAspect = 0.0;

	resultIndex = get_global_id(0);


	if (resultIndex >= nWidth * nHeight)
	{
		return;
	}

	float dCurValue = pRaster[nWidth + resultIndex];
	// 要写的栅格对应的原栅格无值(pRaster比pResult多上下两行)
	if (dCurValue == dNoValue)
	{
		pResult[resultIndex] = dNoValue;
		return;
	}

	// 原栅格块内存取值的位置(pRaster比pResult多上下两行)
	int lLeft, lRight, lTop, lBottom;
	int lCol = resultIndex % nWidth;
	int lRow = resultIndex / nWidth;

	if(lCol == 0)
	{
		lLeft = 0;
	}
	else
	{
		lLeft = lCol - 1;
	}

	if(lCol == nWidth - 1 )
	{
		lRight = nWidth - 1;
	}
	else
	{
		lRight = lCol + 1;
	}
	lRow += 1;
	lTop = lRow + 1;
	lBottom = lRow - 1;

	float dTmpValue = 0.0;
	dTmpValue = pRaster[lRow * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[0] = dTmpValue;
	}
	else
	{
		pdZ[0] = dCurValue;
	}
	dTmpValue = pRaster[lBottom * nWidth + lCol];
	if (dTmpValue != dNoValue)
	{
		pdZ[1] = dTmpValue;
	}
	else
	{
		pdZ[1] = dCurValue;
	}
	dTmpValue = pRaster[lRow * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[2] = dTmpValue;
	}
	else
	{
		pdZ[2] = dCurValue;
	}
	dTmpValue = pRaster[lTop * nWidth + lCol];
	if (dTmpValue != dNoValue)
	{
		pdZ[3] = dTmpValue;
	}
	else
	{
		pdZ[3] = dCurValue;
	}
	dTmpValue = pRaster[lBottom * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[4] = dTmpValue;
	}
	else
	{
		pdZ[4] = dCurValue;
	}
	dTmpValue = pRaster[lBottom * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[5] = dTmpValue;
	}
	else
	{
		pdZ[5] = dCurValue;
	}
	dTmpValue = pRaster[lTop * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[6] = dTmpValue ;
	}
	else
	{
		pdZ[6] = dCurValue;
	}
	dTmpValue = pRaster[lTop * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[7] = dTmpValue;
	}
	else
	{
		pdZ[7] = dCurValue;
	}
	
	dSlopewe = ((pdZ[7] + 2 * pdZ[0] + pdZ[4]) - (pdZ[6] + 2 * pdZ[2] + pdZ[5])) * dCellSizeX;
	dSlopesn = ((pdZ[6] + 2 * pdZ[3] + pdZ[7]) - (pdZ[5] + 2 * pdZ[1] + pdZ[4])) * dCellSizeX;

	float dSlope = sqrt(dSlopewe * dSlopewe + dSlopesn * dSlopesn);
	dSlope *= dZFactor;
	dSlope = atan(dSlope);
	
	double PI = 3.1415926535897932384626433833;

	if(dSlopesn == 0.0)
	{
		if(dSlopewe == 0.0)
		{
			dAspect = -1.0f;
		}
		else
		{
			if(dSlopewe > 0.0)
			{  
				dAspect = PI * 0.5;
			}
			else if(dSlopewe < 0.0)
			{
				dAspect = PI * 1.5;						
			}
		}				
	}
	else
	{
		//地理上顺时针为正方向
		if(dSlopewe >= 0.0 && dSlopesn > 0.0)//1     0 ~ 90
		{
			dAspect = atan(dSlopewe / dSlopesn);	
		}
		else if(dSlopesn < 0) //90 ~ 270
		{
			dAspect = atan(dSlopewe / dSlopesn) + PI;
		}
		else     //4     270 ~ 360
		{
			//if(dAspect < 0)              //最大值不超过2 * PI
			dAspect = atan(dSlopewe / dSlopesn) + PI * 2.0;
		}
	}
	
	float dValue = 255*(cos(dAltitude)*sin(dSlope)*cos(dAspect - dAzimuth) + sin(dAltitude)*cos(dSlope));
	if(dValue < 0)
	{
		dValue = 0.0f;
	}

	pResult[resultIndex] = dValue;
}