#if !defined(PROCESSPARAMS_H__06EECAD6_9472_40F0_B21F_AA8CBCB15077__INCLUDED_)
#define PROCESSPARAMS_H__06EECAD6_9472_40F0_B21F_AA8CBCB15077__INCLUDED_

#pragma once
#include "Common.h"
#include "stdafx.h"

namespace S3MB
{
	class STK_API ProcessParams
	{
	public:
		ProcessParams();

		virtual ProcessType GetProcessType();
	};
}

#endif