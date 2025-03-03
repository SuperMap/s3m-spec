/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_SURFACEVARGROUP_H__
#define __MayaDM_SURFACEVARGROUP_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMBaseGeometryVarGroup.h"
namespace MayaDM
{
class SurfaceVarGroup : public BaseGeometryVarGroup
{
public:
public:

	SurfaceVarGroup():BaseGeometryVarGroup(){}
	SurfaceVarGroup(FILE* file,const std::string& name,const std::string& parent="",bool shared=false,bool create=true)
		:BaseGeometryVarGroup(file, name, parent, "surfaceVarGroup", shared, create){}
	virtual ~SurfaceVarGroup(){}

	void getCreate(size_t cr_i)const
	{
		fprintf(mFile,"\"%s.cr[%i]\"",mName.c_str(),cr_i);
	}
	void getCreate()const
	{

		fprintf(mFile,"\"%s.cr\"",mName.c_str());
	}
	void getLocal(size_t l_i)const
	{
		fprintf(mFile,"\"%s.l[%i]\"",mName.c_str(),l_i);
	}
	void getLocal()const
	{

		fprintf(mFile,"\"%s.l\"",mName.c_str());
	}
protected:
	SurfaceVarGroup(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool shared=false,bool create=true)
		:BaseGeometryVarGroup(file, name, parent, nodeType, shared, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_SURFACEVARGROUP_H__
