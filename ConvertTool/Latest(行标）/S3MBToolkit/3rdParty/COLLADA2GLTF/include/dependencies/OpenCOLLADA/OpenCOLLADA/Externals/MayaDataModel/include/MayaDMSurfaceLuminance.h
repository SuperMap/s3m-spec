/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_SURFACELUMINANCE_H__
#define __MayaDM_SURFACELUMINANCE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMDependNode.h"
namespace MayaDM
{
class SurfaceLuminance : public DependNode
{
public:
public:

	SurfaceLuminance():DependNode(){}
	SurfaceLuminance(FILE* file,const std::string& name,const std::string& parent="",bool shared=false,bool create=true)
		:DependNode(file, name, parent, "surfaceLuminance", shared, create){}
	virtual ~SurfaceLuminance(){}

	void getNormalCamera()const
	{
		fprintf(mFile,"\"%s.n\"",mName.c_str());
	}
	void getNormalCameraX()const
	{
		fprintf(mFile,"\"%s.n.nx\"",mName.c_str());
	}
	void getNormalCameraY()const
	{
		fprintf(mFile,"\"%s.n.ny\"",mName.c_str());
	}
	void getNormalCameraZ()const
	{
		fprintf(mFile,"\"%s.n.nz\"",mName.c_str());
	}
	void getLightDataArray(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i]\"",mName.c_str(),ltd_i);
	}
	void getLightDataArray()const
	{

		fprintf(mFile,"\"%s.ltd\"",mName.c_str());
	}
	void getLightDirection(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ld\"",mName.c_str(),ltd_i);
	}
	void getLightDirection()const
	{

		fprintf(mFile,"\"%s.ltd.ld\"",mName.c_str());
	}
	void getLightDirectionX(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ld.ldx\"",mName.c_str(),ltd_i);
	}
	void getLightDirectionX()const
	{

		fprintf(mFile,"\"%s.ltd.ld.ldx\"",mName.c_str());
	}
	void getLightDirectionY(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ld.ldy\"",mName.c_str(),ltd_i);
	}
	void getLightDirectionY()const
	{

		fprintf(mFile,"\"%s.ltd.ld.ldy\"",mName.c_str());
	}
	void getLightDirectionZ(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ld.ldz\"",mName.c_str(),ltd_i);
	}
	void getLightDirectionZ()const
	{

		fprintf(mFile,"\"%s.ltd.ld.ldz\"",mName.c_str());
	}
	void getLightIntensity(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].li\"",mName.c_str(),ltd_i);
	}
	void getLightIntensity()const
	{

		fprintf(mFile,"\"%s.ltd.li\"",mName.c_str());
	}
	void getLightIntensityR(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].li.lir\"",mName.c_str(),ltd_i);
	}
	void getLightIntensityR()const
	{

		fprintf(mFile,"\"%s.ltd.li.lir\"",mName.c_str());
	}
	void getLightIntensityG(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].li.lig\"",mName.c_str(),ltd_i);
	}
	void getLightIntensityG()const
	{

		fprintf(mFile,"\"%s.ltd.li.lig\"",mName.c_str());
	}
	void getLightIntensityB(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].li.lib\"",mName.c_str(),ltd_i);
	}
	void getLightIntensityB()const
	{

		fprintf(mFile,"\"%s.ltd.li.lib\"",mName.c_str());
	}
	void getLightAmbient(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].la\"",mName.c_str(),ltd_i);
	}
	void getLightAmbient()const
	{

		fprintf(mFile,"\"%s.ltd.la\"",mName.c_str());
	}
	void getLightDiffuse(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ldf\"",mName.c_str(),ltd_i);
	}
	void getLightDiffuse()const
	{

		fprintf(mFile,"\"%s.ltd.ldf\"",mName.c_str());
	}
	void getLightSpecular(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].ls\"",mName.c_str(),ltd_i);
	}
	void getLightSpecular()const
	{

		fprintf(mFile,"\"%s.ltd.ls\"",mName.c_str());
	}
	void getLightShadowFraction(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].lsf\"",mName.c_str(),ltd_i);
	}
	void getLightShadowFraction()const
	{

		fprintf(mFile,"\"%s.ltd.lsf\"",mName.c_str());
	}
	void getPreShadowIntensity(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].psi\"",mName.c_str(),ltd_i);
	}
	void getPreShadowIntensity()const
	{

		fprintf(mFile,"\"%s.ltd.psi\"",mName.c_str());
	}
	void getLightBlindData(size_t ltd_i)const
	{
		fprintf(mFile,"\"%s.ltd[%i].lbd\"",mName.c_str(),ltd_i);
	}
	void getLightBlindData()const
	{

		fprintf(mFile,"\"%s.ltd.lbd\"",mName.c_str());
	}
	void getOutValue()const
	{
		fprintf(mFile,"\"%s.o\"",mName.c_str());
	}
protected:
	SurfaceLuminance(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool shared=false,bool create=true)
		:DependNode(file, name, parent, nodeType, shared, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_SURFACELUMINANCE_H__
