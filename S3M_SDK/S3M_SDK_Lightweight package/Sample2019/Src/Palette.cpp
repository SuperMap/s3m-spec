#include "Palette.h"
#include <assert.h>


	Palette::Palette()
	{

	}
	Palette::~Palette()
	{

	}

	UGColor Palette::UGRGB(unsigned int r, unsigned int g, unsigned int b)
	{
		UGColor nValue = 0;
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

	UGColor Palette::RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
	{
		UGColor nValue = 0;
		if (ISBIGENDIAN)
		{
			nValue = (((unsigned int)(unsigned char)(r) << 24) | ((unsigned int)(unsigned char)(g) << 16) | ((unsigned int)(unsigned char)(b) << 8) | (unsigned int)(unsigned char)(a));
		}
		else
		{
			nValue = (((unsigned int)(unsigned char)(r)) | ((unsigned int)(unsigned char)(g) << 8) | ((unsigned int)(unsigned char)(b) << 16) | (unsigned int)(unsigned char)(b) << 24);
		}
		return nValue;
	}

	unsigned char Palette::REDVAL(UGColor rgba)
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

	unsigned char Palette::GREENVAL(UGColor rgba)
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

	unsigned char Palette::BLUEVAL(UGColor rgba)
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

	unsigned char Palette::ALPHAVAL(UGColor rgba)
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

	int Palette::Find(Palette::PaletteEntry paletteEntry) const
	{
		int nCount = GetSize();
		Palette::PaletteEntry paletteEntrySrc;
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

	int Palette::Add(Palette::PaletteEntry paletteEntry)
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

	void Palette::InsertAt(int nIndex, Palette::PaletteEntry paletteEntry)
	{
		m_Palette.insert(m_Palette.begin() + nIndex, 1, paletteEntry);
	}

	void Palette::SetAt(int nIndex, Palette::PaletteEntry paletteEntry)
	{
		assert(nIndex >= 0 && nIndex < m_Palette.size());
		m_Palette.at(nIndex) = paletteEntry;
	}

	Palette::PaletteEntry Palette::GetAt(int nIndex)const
	{
		assert(nIndex >= 0 && nIndex < m_Palette.size());
		return m_Palette.at(nIndex);
	}

	void Palette::RemoveAll()
	{
		m_Palette.clear();
	}

	Palette::PaletteEntry* Palette::GetData()
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



