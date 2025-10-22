#pragma once
#ifndef S3MB_DRACOTOOL
#define S3MB_DRACOTOOL

#include "Skeleton.h"

namespace S3MB
{
	class S3MB_API DracoCompressParam
	{
	public:
		// darcoѹ����������
		DracoCompressParam();
		DracoCompressParam(const DracoCompressParam& other);

		~DracoCompressParam() {};

		// ����λ�õ�ֵ��ѹ��ʱռ�õ�λ����ȡֵ��1-30��Ĭ����11
		void SetPosQuantizationBits(unsigned int bits);
		unsigned int GetPosQuantizationBits() const;

		// �������������ֵ��ѹ��ʱռ�õ�λ����ȡֵ��1-30��Ĭ����10
		void SetTexcoordsQuantizationBits(unsigned int bits);
		unsigned int GetTexcoordsQuantizationBits() const;

		// ����ķ��ߵ�ֵ��ѹ��ʱռ�õ�λ����ȡֵ��1-30��Ĭ����7
		void SetNormalsQuantizationBits(unsigned int bits);
		unsigned int GetNormalsQuantizationBits() const;

		// ��������ͨ�����Ե�ֵ��ѹ��ʱռ�õ�λ����ȡֵ��1-30��Ĭ����12
		void SetGenericQuantizationBits(unsigned int bits);
		unsigned int GetGenericQuantizationBits() const;

		// ѹ��ʱ���õ��ٶȼ���0-10֮�䡣ԽСѹ���ٶ�Խ������ѹ����Խ�ߡ�Ĭ����0
		void SetEncodeSpeed(unsigned int nSpeedLevel);
		unsigned int GetEncodeSpeed() const;

		// ��ѹʱ��ﵽ���ٶȼ���0-10֮�䡣ԽС��ѹ�ٶ�Խ������ѹ����Խ�ߡ�Ĭ����0
		void SetDecodeSpeed(unsigned int nSpeedLevel);
		unsigned int GetDecodeSpeed() const;

		// �Ƿ�����ѹ��(Ŀǰû�õ�)
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

	// ��dracoMesh�е���������
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
		// ���ö������Ե�ID
		void SetUniqueID(int nUniqueID) { m_nUniqueID = nUniqueID; }
		int GetUniqueID() const { return m_nUniqueID; }

		// ���ö�Ӧ��dracoMesh�е���������
		void SetDracoAttributeType(unsigned int dracoType) { m_enAttributeType = (DracoAttributeType)dracoType; }
		unsigned int GetDracoAttributeType() const { return m_enAttributeType; };

		// �Ƿ�����ѹ��
		void SetEnable(bool bEnable) { m_bNeedCompress = bEnable; }
		bool IsEnable() const { return m_bNeedCompress; };
	private:
		// ѹ������dracoMesh�ж������Ե�UniqueId
		int m_nUniqueID;
		// ��dracoMesh�е���������
		DracoAttributeType m_enAttributeType;
		// �Ƿ���Ҫѹ��
		bool m_bNeedCompress;
	};

	class S3MB_API DracoCompressedInfo
	{
	public:
		// dracoѹ����Ϣ
		DracoCompressedInfo();

		~DracoCompressedInfo() {}
	public:
		// ѹ������dracoMesh�ж���λ�����Ե�UniqueId
		DracoAttributeInfo m_posInfo;
		// ѹ������dracoMesh�ж��㷨�����Ե�UniqueId
		DracoAttributeInfo m_normalInfo;
		// ѹ������dracoMesh�ж�����ɫ���Ե�UniqueId
		DracoAttributeInfo m_colorInfo;
		// ѹ������dracoMesh�ж���secondColor���Ե�UniqueId
		DracoAttributeInfo m_secondColorInfo;
		// ѹ������dracoMesh�ж��������������Ե�UniqueId
		DracoAttributeInfo m_texCoordInfo[SMSCN_MAX_TEXTURE_COORD_SETS];
		// ѹ������dracoMesh���Զ������Ե�UniqueId
		DracoAttributeInfo* m_pVertexAttInfo;
		// ѹ������dracoMesh���Զ������Ե�UniqueId����
		unsigned int m_nVertexAttInfoCount;
	};


	class S3MB_API DracoTool
	{
	public:
		DracoTool() {};
		virtual ~DracoTool() {};
	public:
		// ���ⲿ���õķ�������VertexDataPackage��IndexPackage�ļ�����Ϣ����dracoѹ��
		// param �洢ѹ�������Ķ��� [in]
		// pVertexDataPackge �������� [in]
		// arrIndexPackage �������� [in]
		// pOutputData �����洢ѹ�������ݵ�ָ�� [out]
		// sizeInBytes ѹ������ֽڴ�С [out]
		// return �ɹ�����true
		static bool DracoCompress(const DracoCompressParam& param, DracoCompressedInfo& convertParam, VertexDataPackage* pVertexDataPackage, std::vector<IndexPackage*> arrIndexPackage, char*& pOutputData, unsigned int& sizeInBytes);

		// ���ⲿ���õķ�������dracoѹ��������ݽ��н�ѹ���洢��C�Ķ���������ṹ�з���
		// pInputData ����dracoѹ���������ݵ�ָ�� [in]
		// sizeInBytes ѹ�����ݵ��ֽڴ�С [in]
		// isPointCloud ѹ�������Ƿ��ǵ��ƣ��������� [in]
		// pVertexDataPackge �����洢��ѹ��Ķ������� [out]
		// arrIndexPackage �����洢��ѹ����������� [out]
		// return �ɹ�����true
		static bool DracoDecompress(const DracoCompressedInfo& convertParam, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, unsigned int nVertexAttCount, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& arrIndexPackage);

		// ���ⲿ���õķ�������dracoѹ��������ݽ��н�ѹ���洢��UGC�Ķ���������ṹ�з���
		// vecVertexAttDescript �Զ������Ե���������
		// pInputData ����dracoѹ���������ݵ�ָ�� [in]
		// sizeInBytes ѹ�����ݵ��ֽڴ�С [in]
		// isPointCloud ѹ�������Ƿ��ǵ��ƣ��������� [in]
		// pVertexDataPackge �����洢��ѹ��Ķ������� [out]
		// vecIndexPackage �����洢��ѹ����������� [out]
		// pFeatureIds id���� [out]
		// nFeatureIdCount id���ݵ��ֽڴ�С [out]
		// �ɹ�����true
		static bool DracoDecompress(const std::vector<std::string>& vecVertexAttDescript, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& vecIndexPackage, unsigned int*& pFeatureIds, unsigned int& nFeatureIdCount);

	protected:
		// ���ⲿ���õķ�������dracoѹ��������ݽ��н�ѹ���洢��UGC�Ķ���������ṹ�з���
		// pInputData ����dracoѹ���������ݵ�ָ�� [in]
		// sizeInBytes ѹ�����ݵ��ֽڴ�С [in]
		// isPointCloud ѹ�������Ƿ��ǵ��ƣ��������� [in]
		// nPosUniqueID ����λ��������dracoMesh�е�ID [in]
		// nNormalUniqueID ���㷨��������dracoMesh�е�ID [in]
		// nVertexColorUniqueID ������ɫ������dracoMesh�е�ID [in]
		// nSecondColorUniqueID ����secondColor������dracoMesh�е�ID [in]
		// nTexCoordUniqueID ������������������dracoMesh�е�ID���� [in]
		// pVertexAttUniqueID �Զ���������dracoMesh�е�ID���� [in]
		// nVertexAttCount �Զ������Ը��� [in]
		// pVertices �����洢��ѹ��Ķ���λ������ [out]
		// nVerticesCount �����洢��ѹ��Ķ���λ������ [out]
		// nVertexDim �����洢��ѹ��Ķ���λ��ά�� [out]
		// pNormals �����洢��ѹ��Ķ��㷨������ [out]
		// nNormalCount �����洢��ѹ��Ķ��㷨������ [out]
		// nNormalDim �����洢��ѹ��Ķ��㷨��ά�� [out]
		// pVertexColor �����洢��ѹ��Ķ�����ɫ���� [out]
		// nVertexColorCount �����洢��ѹ��Ķ�����ɫ���� [out]
		// pSecondColor �����洢��ѹ��Ķ�����ɫ���� [out]
		// nSecondColorCount �����洢��ѹ��Ķ�����ɫ���� [out]
		// pTexCoords �����洢��ѹ��Ķ������������ָ������ [out]
		// pTexCoordCount �����洢��ѹ��Ķ������������������� [out]
		// pTexCoordDim �����洢��ѹ������������ά������ [out]
		// pVertexAttData �����洢��ѹ����Զ������Ե�ָ������ [out]
		// pVertexAttCount �����洢��ѹ����Զ��������������� [out]
		// pVertexAttDim �����洢��ѹ����Զ������Ե�ά������ [out]
		// pIndex �����洢��ѹ����������� [out]
		// nIndexesCount �����洢��ѹ����������� [out]
		// bUseUint �����洢��ѹ��������Ƿ�ʹ��Uint���� [out]
		static bool DracoDecompressImpl(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, int nPosUniqueID, int nNormalUniqueID, int nVertexColorUniqueID,
			int pTexCoordUniqueID[8], int* pVertexAttUniqueID, unsigned int nVertexAttCount,
			float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim, float*& pNormals, unsigned int& nNormalCount,
			unsigned short& nNormalDim, unsigned int *& pVertexColor, unsigned int& nVertexColorCount, float**& pTexCoords,
			unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim, void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
			unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint);

		// ���ⲿ���õķ�������dracoѹ��������ݽ��н�ѹ���洢��UGC�Ķ���������ṹ�з���
		// pInputData ����dracoѹ���������ݵ�ָ�� [in]
		// sizeInBytes ѹ�����ݵ��ֽڴ�С [in]
		// isPointCloud ѹ�������Ƿ��ǵ��ƣ��������� [in]
		// vecVertexAttDescript �Զ���������dracoMesh�е��������� [in]
		// pVertices �����洢��ѹ��Ķ���λ������ [out]
		// nVerticesCount �����洢��ѹ��Ķ���λ������ [out]
		// nVertexDim �����洢��ѹ��Ķ���λ��ά�� [out]
		// pNormals �����洢��ѹ��Ķ��㷨������ [out]
		// nNormalCount �����洢��ѹ��Ķ��㷨������ [out]
		// nNormalDim �����洢��ѹ��Ķ��㷨��ά�� [out]
		// pVertexColor �����洢��ѹ��Ķ�����ɫ���� [out]
		// nVertexColorCount �����洢��ѹ��Ķ�����ɫ���� [out]
		// pSecondColor �����洢��ѹ��Ķ�����ɫ���� [out]
		// nSecondColorCount �����洢��ѹ��Ķ�����ɫ���� [out]
		// pTexCoords �����洢��ѹ��Ķ������������ָ������ [out]
		// pTexCoordCount �����洢��ѹ��Ķ������������������� [out]
		// pTexCoordDim �����洢��ѹ������������ά������ [out]
		// pVertexAttData �����洢��ѹ����Զ������Ե�ָ������ [out]
		// pVertexAttDataType �����洢��ѹ����Զ������������������� [out]
		// pVertexAttCount �����洢��ѹ����Զ��������������� [out]
		// pVertexAttDim �����洢��ѹ����Զ������Ե�ά������ [out]
		// pIndex �����洢��ѹ����������� [out]
		// nIndexesCount �����洢��ѹ����������� [out]
		// bUseUint �����洢��ѹ��������Ƿ�ʹ��Uint���� [out]
		static bool DracoDecompressImpl_descript(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud,
			const std::vector<std::string>& vecVertexAttDescript,
			float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim,
			float*& pNormals, unsigned int& nNormalCount, unsigned short& nNormalDim,
			unsigned int *& pVertexColor, unsigned int& nVertexColorCount,
			unsigned int *& pSecondColor, unsigned int& nSecondColorCount,
			float**& pTexCoords, unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim,
			void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
			unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint);

		// ���ⲿ���õķ������Լ�����Ϣ����dracoѹ��
		// nPosQuantizationBits ѹ��ʱ����λ������ʹ�õ�ֵ��λ�� [in]
		// nTexcoordsQuantizationBits ѹ��ʱ����������������ʹ�õ�ֵ��λ�� [in]
		// nNormalsQuantizationBits ѹ��ʱ���㷨������ʹ�õ�ֵ��λ�� [in]
		// nGenericQuantizationBits ѹ��ʱ������������ʹ�õ�ֵ��λ�� [in]
		// nEncodeSpeed ѹ���㷨��ִ���ٶȼ��� [in]
		// nDecodeSpeed ��ѹ�㷨��ִ���ٶȼ��� [in]
		// texCoordAttributeType ������������ʹ�õ��������� [in]
		// pVertices ��ѹ���Ķ���λ������ [in]
		// nVerticesCount ����λ������ [in]
		// nVertexDim ����λ��ά�� [in]
		// pNormals �����㷨������ [in]
		// nNormalCount ���㷨������ [in]
		// nNormalDim ���㷨��ά�� [in]
		// pVertexColor ��ѹ���Ķ�����ɫ���� [in]
		// nVertexColorCount ������ɫ���� [in]
		// pTexCoords ��ѹ���Ķ������������ָ������ [in]
		// pTexCoordCount �������������������� [in]
		// pTexCoordDim ���������ά������ [in]
		// pVertexAttData ��ѹ�����Զ������Ե�ָ������ [in]
		// pVertexAttCount �Զ��������������� [in]
		// pVertexAttDim �Զ������Ե�ά������ [in]
		// nVertexAttCount �Զ������Ը��� [in]
		// pIndex ��ѹ������������ [in]
		// nIndexesCount ���������С [in]
		// bUseUint �����Ƿ�ʹ��unint���� [in]
		// pOutputData �����洢ѹ�������ݵ�ָ�� [out]
		// sizeInBytes ѹ������ֽڴ�С [out]
		// nPosUniqueID ����λ��������dracoMesh�е�ID [out]
		// nNormalUniqueID ���㷨��������dracoMesh�е�ID [out]
		// nVertexColorUniqueID ������ɫ������dracoMesh�е�ID [out]
		// nTexCoordUniqueID ������������������dracoMesh�е�ID���� [out]
		// pVertexAttUniqueID �Զ���������dracoMesh�е�ID���� [out]
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
