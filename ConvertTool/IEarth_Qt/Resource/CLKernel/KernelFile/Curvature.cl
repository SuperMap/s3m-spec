// pRaster比pResult多两行
#pragma OPENCL EXTENSION cl_khr_fp64: enable

__kernel void
Curvature_Kernel
( 
	__global const float *pRaster,
	__global float* pResult,
	unsigned int nWidth,
	unsigned int nHeight,
	double dNoValue,
	double dCellSizeX, double dCellSizeY,
	double dZFactor,
	__global float* pProfileResult,
	__global float* pPlanResult)
{
	double pdZ[9];
	
	int resultIndex = get_global_id(0);

	if (resultIndex >= nWidth * nHeight)
	{
		return;
	}

	float dCurValue = pRaster[nWidth + resultIndex];
	// 要写的栅格对应的原栅格无值(pRaster比pResult多上下两行)
	if (dCurValue == dNoValue)
	{
		pResult[resultIndex] = dNoValue;
		if (pProfileResult != 0)
		{
			pProfileResult[resultIndex] = dNoValue;
		}
		if (pPlanResult != 0)
		{
			pPlanResult[resultIndex] = dNoValue;
		}
		return;
	}
	pdZ[4] = dCurValue * dZFactor;

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
	dTmpValue = pRaster[lTop * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[0] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[0] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lTop * nWidth + lCol];
	if (dTmpValue != dNoValue)
	{
		pdZ[1] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[1] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lTop * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[2] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[2] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lRow * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[3] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[3] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lRow * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[5] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[5] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lBottom * nWidth + lLeft];
	if (dTmpValue != dNoValue)
	{
		pdZ[6] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[6] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lBottom * nWidth + lCol];
	if (dTmpValue != dNoValue)
	{
		pdZ[7] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[7] = dCurValue * dZFactor;
	}
	dTmpValue = pRaster[lBottom * nWidth + lRight];
	if (dTmpValue != dNoValue)
	{
		pdZ[8] = dTmpValue * dZFactor;
	}
	else
	{
		pdZ[8] = dCurValue * dZFactor;
	}

	double dFx = ((pdZ[0] + 2*pdZ[3] + pdZ[6]) - (pdZ[2] + 2*pdZ[5] + pdZ[8])) / (8*dCellSizeX);
	double dFy = ((pdZ[6] + 2*pdZ[7] + pdZ[8]) - (pdZ[0] + 2*pdZ[1] + pdZ[2])) / (8*dCellSizeY);

	double dS4 = pdZ[0] + pdZ[2] + pdZ[6] + pdZ[8] - 8*pdZ[4];
	double dFxx = (dS4 + 4*pdZ[3] + 4*pdZ[5] - 2*pdZ[7] - 2*pdZ[1]) / (6*dCellSizeX*dCellSizeX);
	double dFyy = (dS4 + 4*pdZ[7] + 4*pdZ[1] - 2*pdZ[3] - 2*pdZ[5]) / (6*dCellSizeY*dCellSizeY);

	double dFxy = (pdZ[6] - pdZ[8] + pdZ[2] - pdZ[0]) / (4*dCellSizeX*dCellSizeY);

	// 这个数值后面三种剖面都在用，先计算出来
	double dS2 = dFx * dFx + dFy * dFy;

	// 计算平均曲率
	double dCurvature = -((1 + dFy*dFy)*dFxx - 2*dFxy*dFx*dFy + (1 + dFx*dFx)*dFyy)
		/ (2 * pow(dS2 + 1, 1.5));

	pResult[resultIndex] = dCurvature;
	
	if (pProfileResult != 0)
	{
		double dProfileCurvature = 0.0f;
		if (dS2 > 1e-10 ||  dS2 < -1e-10)
		{
			dProfileCurvature = -(dFxx*dFx*dFx + 2*dFxy*dFx*dFy + dFyy*dFy*dFy)
				/ (dS2 * pow(dS2 + 1, 1.5));
		}
		pProfileResult[resultIndex] = dProfileCurvature;
	}
	
	if (pPlanResult != 0)
	{
		double dPlanCurvature = 0.0f;
		if (dS2 > 1e-10 ||  dS2 < -1e-10)
		{
			dPlanCurvature = -(dFxx*dFy*dFy - 2*dFxy*dFx*dFy + dFyy*dFx*dFx) 
				/ pow(dS2, 1.5);
		}
		pPlanResult[resultIndex] = dPlanCurvature;
	}
}