#if !defined(PALETTE_H__1CABBE54_3EAD_4A5E_97D7_E0D9F49E0D8B__INCLUDED_)
#define PALETTE_H__1CABBE54_3EAD_4A5E_97D7_E0D9F49E0D8B__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBCommon.h"
#include <vector>

namespace S3MB
{
	class S3MB_API Palette
	{
	public:
		Palette();
		~Palette();

	public:
		static unsigned int RGB(unsigned int r, unsigned int g, unsigned int b);
		static unsigned int RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
		static unsigned char REDVAL(unsigned int clr);

		static unsigned char GREENVAL(unsigned int clr);

		static unsigned char BLUEVAL(unsigned int clr);

		static unsigned char ALPHAVAL(unsigned int clr);
		// 查找调色板的位置
		// paletteEntry 要查找的调色板
		// return 返回找到的调色板的位置，找不到返回 -1
		int Find(PaletteEntry paletteEntry) const;
		// 在数组最后添加一个调色板
		// paletteEntry 要添加的调色板
		// return 返回添加后,调色板在数组中的位置
		int Add(PaletteEntry paletteEntry);
		// 在数组中移出调色板
		// nIndex 要移出的调色板在数组的位置
		// 说明： 指定位置必须在数组范围内
		// nCount 移出的个数,默认为1
		// return 返回实际移出面板的个数
		// 说明： 如果 nCount>UGArray.size-nIndex 返回  nCount>UGArray.size-nIndex
		int RemoveAt(int nIndex, int nCount = 1);
		// 在数组指定位置插入一个调色板
		// 说明： 如果指定位置不在数组范围内,会自动调整到[0,size]
		// nIndex 要插入的位置
		// paletteEntry 要插入的调色板
		void InsertAt(int nIndex, PaletteEntry paletteEntry);
		// 在数组的指定位置替换调色板
		// nIndex 指定的位置
		// 说明： 指定位置必须在数组size范围之内
		// paletteEntry 要替换的调色板
		void SetAt(int nIndex, PaletteEntry paletteEntry);
		// 获取指定位置的调色板
		// nIndex 指定的位置
		// 说明： 指定位置必须在数组size范围之内
		// return 返回获得的调色板
		PaletteEntry GetAt(int nIndex)const;
		//! 移除所有元素,同时清空所有空间
		void RemoveAll();
		// 得到数组内部指针(地址连续)
		// return 返回数组内部指针(地址连续) 
		PaletteEntry* GetData();
		// 在数组最后增加一个数组
		// palette 要增加的数组
		// return 返回加入数组的起始位置
		int Append(const Palette& palette);
		// 数组拷贝,原来的内容被覆盖
		// palette 传入的数组
		void Copy(const Palette& palette);
		// 调整m_Palette的内存空间为 nSize, 
		// 说明： 如果原有空间不够, 内存空间会增加, 
		// nSize 指定的新的调色板个数
		void SetSize(int nSize);
		// 得到有用的调色板的个数
		// return 返回的调色板的个数
		int GetSize()const;
		// 判断数组是否为空
		// return 如果空返回 true,否则返回 false.
		bool IsEmpty()const;

	protected:
		// 将调色板用数组实现
		vector<PaletteEntry> m_Palette;
	};

	class S3MB_API ColorValue
	{
	public:
		ColorValue();
		ColorValue(double red, double green, double blue, double alpha);

		// 重载等值判断运算符。
		// rhs [in]。
		// return 是否相等。
		bool operator==(const ColorValue& rhs) const;

		// 重载不等值判断运算符。
		// rhs [in]。
		// return 是否不等。
		bool operator!=(const ColorValue& rhs) const;
		
		// 传入R、G、B、A分量构建三维颜色 。
		// nRed 红色分量，范围0-255[in]。
		// nGreen 绿色分量，范围0-255[in]。
		// nBlue 蓝色分量，范围0-255[in]。
		// nAlpha 不透明度分量，范围0-255[in]。
		void SetValue(int nRed, int nGreen, int nBlue, int nAlpha);
		// 传入R、G、B、A分量构建三维颜色。
		// dRed 红色分量，范围0-1.0[in]。
		// dGreen 绿色分量，范围0-1[in]。
		// dBlue 蓝色分量，范围0-1[in]。
		// dAlpha 不透明度分量，范围0-1[in]。
		void SetValue(double nRed, double nGreen, double nBlue, double nAlpha);
		//  将ColorValue转换整型值
		unsigned int GetValue() const;

	public:
		double r;
		double g;
		double b;
		double a;
	};
}

#endif



