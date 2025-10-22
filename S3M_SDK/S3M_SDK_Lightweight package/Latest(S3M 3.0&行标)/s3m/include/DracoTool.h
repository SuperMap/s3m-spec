#pragma once
#ifndef S3MB_DRACOTOOL
#define S3MB_DRACOTOOL

#include "Skeleton.h"

namespace S3MB
{
	class S3MB_API DracoCompressParam
	{
	public:
		// darco压缩参数对象
		DracoCompressParam();
		DracoCompressParam(const DracoCompressParam& other);

		~DracoCompressParam() {};

		// 顶点位置的值在压缩时占用的位数，取值是1-30。默认是11
		void SetPosQuantizationBits(unsigned int bits);
		unsigned int GetPosQuantizationBits() const;

		// 顶点纹理坐标的值在压缩时占用的位数，取值是1-30。默认是10
		void SetTexcoordsQuantizationBits(unsigned int bits);
		unsigned int GetTexcoordsQuantizationBits() const;

		// 顶点的法线的值在压缩时占用的位数，取值是1-30。默认是7
		void SetNormalsQuantizationBits(unsigned int bits);
		unsigned int GetNormalsQuantizationBits() const;

		// 顶点其他通用属性的值在压缩时占用的位数，取值是1-30。默认是12
		void SetGenericQuantizationBits(unsigned int bits);
		unsigned int GetGenericQuantizationBits() const;

		// 压缩时采用的速度级别。0-10之间。越小压缩速度越慢，但压缩率越高。默认是0
		void SetEncodeSpeed(unsigned int nSpeedLevel);
		unsigned int GetEncodeSpeed() const;

		// 解压时想达到的速度级别。0-10之间。越小解压速度越慢，但压缩率越高。默认是0
		void SetDecodeSpeed(unsigned int nSpeedLevel);
		unsigned int GetDecodeSpeed() const;

		// 是否启用压缩(目前没用到)
		void SetEnable(bool bEnable);
		bool IsEnable() const;
	private:
		unsigned int m_nPosQuantizationBits;
		unsigned int m_nTexcoordsQuantizationBits;
		unsigned int m_nNormalsQuantizationBits;
		unsigned int m_nGenericQuantizationBits;
		unsigned int m_nEncodeSpeed;
		unsigned int m_nDecodeSpeed;
		bool m_bEnable;
	};

	// 在dracoMesh中的语义类型
	enum DracoAttributeType {
		DRACO_POSITION = 0,
		DRACO_NORMAL = 1,
		DRACO_COLOR = 2,
		DRACO_TEX_COORD = 3,
		DRACO_GENERIC = 4
	};

	class S3MB_API DracoAttributeInfo
	{
	public:
		DracoAttributeInfo();
		DracoAttributeInfo(int nUniqueId, DracoAttributeType enAttributeType, bool bNeedCompress);
		~DracoAttributeInfo() {}
	public:
		// 设置顶点属性的ID
		void SetUniqueID(int nUniqueID) { m_nUniqueID = nUniqueID; }
		int GetUniqueID() const { return m_nUniqueID; }

		// 设置对应的dracoMesh中的语义类型
		void SetDracoAttributeType(unsigned int dracoType) { m_enAttributeType = (DracoAttributeType)dracoType; }
		unsigned int GetDracoAttributeType() const { return m_enAttributeType; };

		// 是否启用压缩
		void SetEnable(bool bEnable) { m_bNeedCompress = bEnable; }
		bool IsEnable() const { return m_bNeedCompress; };
	private:
		// 压缩后在dracoMesh中顶点属性的UniqueId
		int m_nUniqueID;
		// 在dracoMesh中的语义类型
		DracoAttributeType m_enAttributeType;
		// 是否需要压缩
		bool m_bNeedCompress;
	};

	class S3MB_API DracoCompressedInfo
	{
	public:
		// draco压缩信息
		DracoCompressedInfo();

		~DracoCompressedInfo() {}
	public:
		// 压缩后在dracoMesh中顶点位置属性的UniqueId
		DracoAttributeInfo m_posInfo;
		// 压缩后在dracoMesh中顶点法线属性的UniqueId
		DracoAttributeInfo m_normalInfo;
		// 压缩后在dracoMesh中顶点颜色属性的UniqueId
		DracoAttributeInfo m_colorInfo;
		// 压缩后在dracoMesh中顶点secondColor属性的UniqueId
		DracoAttributeInfo m_secondColorInfo;
		// 压缩后在dracoMesh中顶点纹理坐标属性的UniqueId
		DracoAttributeInfo m_texCoordInfo[SMSCN_MAX_TEXTURE_COORD_SETS];
		// 压缩后在dracoMesh中自定义属性的UniqueId
		DracoAttributeInfo* m_pVertexAttInfo;
		// 压缩后在dracoMesh中自定义属性的UniqueId个数
		unsigned int m_nVertexAttInfoCount;
	};


	class S3MB_API DracoTool
	{
	public:
		DracoTool() {};
		virtual ~DracoTool() {};
	public:
		// 供外部调用的方法。对VertexDataPackage和IndexPackage的几何信息进行draco压缩
		// param 存储压缩参数的对象 [in]
		// pVertexDataPackge 顶点数据 [in]
		// arrIndexPackage 索引数据 [in]
		// pOutputData 用来存储压缩后数据的指针 [out]
		// sizeInBytes 压缩后的字节大小 [out]
		// return 成功返回true
		static bool DracoCompress(const DracoCompressParam& param, DracoCompressedInfo& convertParam, VertexDataPackage* pVertexDataPackage, std::vector<IndexPackage*> arrIndexPackage, char*& pOutputData, unsigned int& sizeInBytes);

		// 供外部调用的方法。对draco压缩后的数据进行解压缩存储到C的顶点和索引结构中返回
		// pInputData 输入draco压缩过的数据的指针 [in]
		// sizeInBytes 压缩数据的字节大小 [in]
		// isPointCloud 压缩数据是否是点云（无索引） [in]
		// pVertexDataPackge 用来存储解压后的顶点数据 [out]
		// arrIndexPackage 用来存储解压后的索引数据 [out]
		// return 成功返回true
		static bool DracoDecompress(const DracoCompressedInfo& convertParam, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, unsigned int nVertexAttCount, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& arrIndexPackage);

		// 供外部调用的方法。对draco压缩后的数据进行解压缩存储到UGC的顶点和索引结构中返回
		// vecVertexAttDescript 自定义属性的语义数组
		// pInputData 输入draco压缩过的数据的指针 [in]
		// sizeInBytes 压缩数据的字节大小 [in]
		// isPointCloud 压缩数据是否是点云（无索引） [in]
		// pVertexDataPackge 用来存储解压后的顶点数据 [out]
		// vecIndexPackage 用来存储解压后的索引数据 [out]
		// pFeatureIds id数据 [out]
		// nFeatureIdCount id数据的字节大小 [out]
		// 成功返回true
		static bool DracoDecompress(const std::vector<std::string>& vecVertexAttDescript, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& vecIndexPackage, unsigned int*& pFeatureIds, unsigned int& nFeatureIdCount);

	protected:
		// 供外部调用的方法。对draco压缩后的数据进行解压缩存储到UGC的顶点和索引结构中返回
		// pInputData 输入draco压缩过的数据的指针 [in]
		// sizeInBytes 压缩数据的字节大小 [in]
		// isPointCloud 压缩数据是否是点云（无索引） [in]
		// nPosUniqueID 顶点位置属性在dracoMesh中的ID [in]
		// nNormalUniqueID 顶点法线属性在dracoMesh中的ID [in]
		// nVertexColorUniqueID 顶点颜色属性在dracoMesh中的ID [in]
		// nSecondColorUniqueID 顶点secondColor属性在dracoMesh中的ID [in]
		// nTexCoordUniqueID 顶点纹理坐标属性在dracoMesh中的ID数组 [in]
		// pVertexAttUniqueID 自定义属性在dracoMesh中的ID数组 [in]
		// nVertexAttCount 自定义属性个数 [in]
		// pVertices 用来存储解压后的顶点位置数组 [out]
		// nVerticesCount 用来存储解压后的顶点位置数量 [out]
		// nVertexDim 用来存储解压后的顶点位置维度 [out]
		// pNormals 用来存储解压后的顶点法线数组 [out]
		// nNormalCount 用来存储解压后的顶点法线数量 [out]
		// nNormalDim 用来存储解压后的顶点法线维度 [out]
		// pVertexColor 用来存储解压后的顶点颜色数组 [out]
		// nVertexColorCount 用来存储解压后的顶点颜色数量 [out]
		// pSecondColor 用来存储解压后的顶点颜色数组 [out]
		// nSecondColorCount 用来存储解压后的顶点颜色数量 [out]
		// pTexCoords 用来存储解压后的顶点纹理坐标的指针数组 [out]
		// pTexCoordCount 用来存储解压后的顶点纹理坐标数量数组 [out]
		// pTexCoordDim 用来存储解压后的纹理坐标的维度数组 [out]
		// pVertexAttData 用来存储解压后的自定义属性的指针数组 [out]
		// pVertexAttCount 用来存储解压后的自定义属性数量数组 [out]
		// pVertexAttDim 用来存储解压后的自定义属性的维度数组 [out]
		// pIndex 用来存储解压后的索引数组 [out]
		// nIndexesCount 用来存储解压后的索引数量 [out]
		// bUseUint 用来存储解压后的索引是否使用Uint类型 [out]
		static bool DracoDecompressImpl(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, int nPosUniqueID, int nNormalUniqueID, int nVertexColorUniqueID,
			int pTexCoordUniqueID[8], int* pVertexAttUniqueID, unsigned int nVertexAttCount,
			float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim, float*& pNormals, unsigned int& nNormalCount,
			unsigned short& nNormalDim, unsigned int *& pVertexColor, unsigned int& nVertexColorCount, float**& pTexCoords,
			unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim, void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
			unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint);

		// 供外部调用的方法。对draco压缩后的数据进行解压缩存储到UGC的顶点和索引结构中返回
		// pInputData 输入draco压缩过的数据的指针 [in]
		// sizeInBytes 压缩数据的字节大小 [in]
		// isPointCloud 压缩数据是否是点云（无索引） [in]
		// vecVertexAttDescript 自定义属性在dracoMesh中的语义数组 [in]
		// pVertices 用来存储解压后的顶点位置数组 [out]
		// nVerticesCount 用来存储解压后的顶点位置数量 [out]
		// nVertexDim 用来存储解压后的顶点位置维度 [out]
		// pNormals 用来存储解压后的顶点法线数组 [out]
		// nNormalCount 用来存储解压后的顶点法线数量 [out]
		// nNormalDim 用来存储解压后的顶点法线维度 [out]
		// pVertexColor 用来存储解压后的顶点颜色数组 [out]
		// nVertexColorCount 用来存储解压后的顶点颜色数量 [out]
		// pSecondColor 用来存储解压后的顶点颜色数组 [out]
		// nSecondColorCount 用来存储解压后的顶点颜色数量 [out]
		// pTexCoords 用来存储解压后的顶点纹理坐标的指针数组 [out]
		// pTexCoordCount 用来存储解压后的顶点纹理坐标数量数组 [out]
		// pTexCoordDim 用来存储解压后的纹理坐标的维度数组 [out]
		// pVertexAttData 用来存储解压后的自定义属性的指针数组 [out]
		// pVertexAttDataType 用来存储解压后的自定义属性数据类型数组 [out]
		// pVertexAttCount 用来存储解压后的自定义属性数量数组 [out]
		// pVertexAttDim 用来存储解压后的自定义属性的维度数组 [out]
		// pIndex 用来存储解压后的索引数组 [out]
		// nIndexesCount 用来存储解压后的索引数量 [out]
		// bUseUint 用来存储解压后的索引是否使用Uint类型 [out]
		static bool DracoDecompressImpl_descript(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud,
			const std::vector<std::string>& vecVertexAttDescript,
			float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim,
			float*& pNormals, unsigned int& nNormalCount, unsigned short& nNormalDim,
			unsigned int *& pVertexColor, unsigned int& nVertexColorCount,
			unsigned int *& pSecondColor, unsigned int& nSecondColorCount,
			float**& pTexCoords, unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim,
			void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
			unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint);

		// 供外部调用的方法。对几何信息进行draco压缩
		// nPosQuantizationBits 压缩时顶点位置属性使用的值的位数 [in]
		// nTexcoordsQuantizationBits 压缩时顶点纹理坐标属性使用的值的位数 [in]
		// nNormalsQuantizationBits 压缩时顶点法线属性使用的值的位数 [in]
		// nGenericQuantizationBits 压缩时顶点其他属性使用的值的位数 [in]
		// nEncodeSpeed 压缩算法的执行速度级别 [in]
		// nDecodeSpeed 解压算法的执行速度级别 [in]
		// texCoordAttributeType 纹理坐标属性使用的语义类型 [in]
		// pVertices 待压缩的顶点位置数组 [in]
		// nVerticesCount 顶点位置数量 [in]
		// nVertexDim 顶点位置维度 [in]
		// pNormals 待顶点法线数组 [in]
		// nNormalCount 顶点法线数量 [in]
		// nNormalDim 顶点法线维度 [in]
		// pVertexColor 待压缩的顶点颜色数组 [in]
		// nVertexColorCount 顶点颜色数量 [in]
		// pTexCoords 待压缩的顶点纹理坐标的指针数组 [in]
		// pTexCoordCount 顶点纹理坐标数量数组 [in]
		// pTexCoordDim 纹理坐标的维度数组 [in]
		// pVertexAttData 待压缩的自定义属性的指针数组 [in]
		// pVertexAttCount 自定义属性数量数组 [in]
		// pVertexAttDim 自定义属性的维度数组 [in]
		// nVertexAttCount 自定义属性个数 [in]
		// pIndex 待压缩的索引数组 [in]
		// nIndexesCount 索引数组大小 [in]
		// bUseUint 索引是否使用unint类型 [in]
		// pOutputData 用来存储压缩后数据的指针 [out]
		// sizeInBytes 压缩后的字节大小 [out]
		// nPosUniqueID 顶点位置属性在dracoMesh中的ID [out]
		// nNormalUniqueID 顶点法线属性在dracoMesh中的ID [out]
		// nVertexColorUniqueID 顶点颜色属性在dracoMesh中的ID [out]
		// nTexCoordUniqueID 顶点纹理坐标属性在dracoMesh中的ID数组 [out]
		// pVertexAttUniqueID 自定义属性在dracoMesh中的ID数组 [out]
		static void DracoCompressImpl(
			int nPosQuantizationBits, int nTexcoordsQuantizationBits, int nNormalsQuantizationBits, int nGenericQuantizationBits, int nEncodeSpeed, int nDecodeSpeed,
			int texCoordAttributeType, float* pVertices, unsigned int nVerticesCount, unsigned short nVertexDim, float* pNormals, unsigned int nNormalCount, unsigned short nNormalDim,
			unsigned int * pVertexColor, unsigned int nVertexColorCount, float** pTexCoords, unsigned int* nTexCoordCount,
			unsigned short* pTexCoordDim, void** pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int* pVertexAttCount, unsigned short* pVertexAttDim, unsigned int nVertexAttCount,
			unsigned short* pIndex, unsigned int nIndexesCount, bool bUseUint, char*& pOutputData, unsigned int& sizeInBytes, int& nPosUniqueID, int& nNormalUniqueID,
			int& nVertexColorUniqueID, int*& pTexCoordUniqueID, int*& pVertexAttUniqueID);


	};

}

#endif
