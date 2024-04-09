#include "Palette.h"
#include <assert.h>


namespace S3MB
{
	Palette::Palette()
	{
	}

	Palette::~Palette()
	{
	}

	unsigned int Palette::RGB(unsigned int r, unsigned int g, unsigned int b)
	{
		unsigned int nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = (((unsigned int)(unsigned char)(r) << 24) | ((unsigned int)(unsigned char)(g) << 16) | ((unsigned int)(unsigned char)(b) << 8) | 0x000000ff);
		}
		else
		{
			nValue = (((unsigned int)(unsigned char)(r)) | ((unsigned int)(unsigned char)(g) << 8) | ((unsigned int)(unsigned char)(b) << 16) | 0xff000000);
		}

		return nValue;
	}

	unsigned int Palette::RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		unsigned int nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = (((unsigned int)(unsigned char)(r) << 24) | ((unsigned int)(unsigned char)(g) << 16) | ((unsigned int)(unsigned char)(b) << 8) | (unsigned int)(unsigned char)(a));
		}
		else
		{
			nValue = (((unsigned int)(unsigned char)(r)) | ((unsigned int)(unsigned char)(g) << 8) | ((unsigned int)(unsigned char)(b) << 16) | (unsigned int)(unsigned char)(a) << 24);
		}
		return nValue;
	}

	unsigned char Palette::REDVAL(unsigned int rgba)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((rgba) >> 24) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)((rgba) & 0xff));
		}
		return nValue;
	}

	unsigned char Palette::GREENVAL(unsigned int rgba)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((rgba) >> 16) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)(((rgba) >> 8) & 0xff));
		}
		return nValue;
	}

	unsigned char Palette::BLUEVAL(unsigned int rgba)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((rgba) >> 8) & 0xff));

		}
		else
		{
			nValue = ((unsigned char)(((rgba) >> 16) & 0xff));

		}
		return nValue;
	}

	unsigned char Palette::ALPHAVAL(unsigned int rgba)
	{
		unsigned char nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = ((unsigned char)(((rgba)) & 0xff));
		}
		else
		{
			nValue = ((unsigned char)(((rgba) >> 24) & 0xff));
		}
		return nValue;
	}

	int Palette::Find(PaletteEntry paletteEntry) const
	{
		int nCount = GetSize();
		PaletteEntry paletteEntrySrc;
		for (int i = 0; i < nCount; i++)
		{
			paletteEntrySrc = GetAt(i);
			if (paletteEntrySrc.peRed == paletteEntry.peRed &&
				paletteEntrySrc.peGreen == paletteEntry.peGreen &&
				paletteEntrySrc.peBlue == paletteEntry.peBlue)
			{
				return i;
			}
		}
		return -1;
	}

	int Palette::Add(PaletteEntry paletteEntry)
	{
		m_Palette.push_back(paletteEntry);
		return m_Palette.size() - 1;
	}

	int Palette::RemoveAt(int nIndex, int nCount /*= 1*/)
	{
		int nPaletteCount = GetSize();
		assert(nIndex >= 0 && nIndex < nPaletteCount);
		if (nCount > nPaletteCount - nIndex)
		{
			nCount = nPaletteCount - nIndex;
		}
		m_Palette.erase(m_Palette.begin() + nIndex, m_Palette.begin() + nIndex + nCount);
		return nCount;
	}

	void Palette::InsertAt(int nIndex, PaletteEntry paletteEntry)
	{
		m_Palette.insert(m_Palette.begin() + nIndex, 1, paletteEntry);
	}

	void Palette::SetAt(int nIndex, PaletteEntry paletteEntry)
	{
		assert(nIndex >= 0 && nIndex < m_Palette.size());
		m_Palette.at(nIndex) = paletteEntry;
	}

	PaletteEntry Palette::GetAt(int nIndex)const
	{
		assert(nIndex >= 0 && nIndex < m_Palette.size());
		return m_Palette.at(nIndex);
	}

	void Palette::RemoveAll()
	{
		m_Palette.clear();
	}

	PaletteEntry* Palette::GetData()
	{
		return &(*m_Palette.begin());
	}

	int Palette::Append(const Palette& palette)
	{
		m_Palette.insert(m_Palette.end(), palette.m_Palette.begin(), palette.m_Palette.end());
		return m_Palette.size();
	}

	void Palette::Copy(const Palette& palette)
	{
		m_Palette = palette.m_Palette;
	}

	void Palette::SetSize(int nSize)
	{
		assert(nSize >= 0);
		if ((nSize > m_Palette.capacity()))
			m_Palette.resize(nSize);
	}

	int Palette::GetSize()const
	{
		return (int)m_Palette.size();
	}

	bool Palette::IsEmpty()const
	{
		return (m_Palette.size() == 0) ? true : false;
	}

	ColorValue::ColorValue()
		:r(1.0f), g(1.0f), b(1.0f), a(1.0f)
	{
	}

	ColorValue::ColorValue(double red, double green, double blue, double alpha)
		:r(red), g(green), b(blue), a(alpha)
	{
	}

	bool ColorValue::operator==(const ColorValue& rhs) const
	{
		return (EQUAL(r, rhs.r) &&
			EQUAL(g, rhs.g) &&
			EQUAL(b, rhs.b) &&
			EQUAL(a, rhs.a));
	}

	bool ColorValue::operator!=(const ColorValue& rhs) const
	{
		return !(*this == rhs);
	}

	void ColorValue::SetValue(double dRed, double dGreen, double dBlue, double dAlpha)
	{
		r = dRed;
		g = dGreen;
		b = dBlue;
		a = dAlpha;
	}

	void ColorValue::SetValue(int nRed, int nGreen, int nBlue, int nAlpha)
	{
		r = nRed / 255.0;
		g = nGreen / 255.0;
		b = nBlue / 255.0;
		a = nAlpha / 255.0;
	}

	unsigned int ColorValue::GetValue() const
	{
        return Palette::RGBA((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));
	}
}


