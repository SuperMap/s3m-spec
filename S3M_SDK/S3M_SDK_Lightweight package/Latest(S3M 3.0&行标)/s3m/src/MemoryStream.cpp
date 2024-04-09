#include "MemoryStream.h"
#include <string.h>

namespace S3MB
{
	MemoryStream::MemoryStream()
	{
		m_WtPos = 0;
		m_RdPos = 0;
		m_bManaged = false;
		m_type = UTF8;
		m_SizeAdd = 4096;
	}

	MemoryStream::~MemoryStream()
	{
		Close();
	}

	void MemoryStream::Close()
	{
		if (m_bManaged)
		{
			delete[] m_pBuffer;
		}
		m_pBuffer = NULL;
		m_WtPos = 0;
	}

	MemoryStream& MemoryStream::operator <<(const char* pBuffer)
	{
		std::size_t size = strlen(pBuffer);
		if (m_pBuffer && m_WtPos + size <= m_nSize)
		{
			memcpy(m_pBuffer + m_WtPos, pBuffer, size);
			m_WtPos += size;
		}
		else if (m_bManaged)
		{
			do
			{
				ResizeSpace();
			} while (m_WtPos + size > m_nSize);
			memcpy(m_pBuffer + m_WtPos, pBuffer, size);
			m_WtPos += size;
		}
		return *this;
	}

	MemoryStream& MemoryStream::operator <<(const std::string str)
	{
		string temp;
		if (m_type == UTF8)
		{
			temp = StringUtil::ANSI_to_UTF8(str);
		}
		else
		{
			temp = str;
		}
		*this << (unsigned int)temp.size();
		return operator <<(temp.c_str());
	}

	MemoryStream& MemoryStream::operator <<(const wchar_t* pBuffer)
	{
		std::size_t size = wcslen(pBuffer) * sizeof(wchar_t);
		if (m_pBuffer && m_WtPos + size <= m_nSize)
		{
			memcpy(m_pBuffer + m_WtPos, pBuffer, size);
			m_RdPos += size;
		}
		else if (m_bManaged)
		{
			do
			{
				ResizeSpace();
			} while (m_WtPos + size > m_nSize);
			memcpy(m_pBuffer + m_WtPos, pBuffer, size);
			m_RdPos += size;
		}
		return *this;
	}

	MemoryStream& MemoryStream::operator <<(const std::wstring str)
	{
		if (m_type == UTF8)
		{
			std::string utf8String = StringUtil::UNICODE_to_UTF8(str);
			*this << (unsigned int)utf8String.size();
			return operator <<(utf8String.c_str());
		}
		else
		{
			return operator <<(str.c_str());
		}
	}

	MemoryStream& MemoryStream::operator >>(std::string& str)
	{
		unsigned int size = 0;
		*this >> size;
		if (size == 0)
		{
			str = "";
		}
		else if (m_pBuffer && m_RdPos < m_nSize)
		{
			const char* p = (const char*)(m_pBuffer + m_RdPos);
			if (size + m_RdPos <= m_nSize)
			{
				if (m_type == UTF8)
				{
					string temp;
					temp.assign(p, p + size);
					str = StringUtil::UTF8_to_ANSI(temp);
				}
				else
				{
					str.assign(p, p + size);
				}
				m_RdPos += size;
			}
		}
		return *this;
	}

	MemoryStream& MemoryStream::operator >>(std::wstring& str)
	{
		unsigned int size = 0;
		*this >> size;
		if (size == 0)
		{
			str = U("");
		}
		else if (m_pBuffer && m_RdPos < m_nSize)
		{
			if (m_type == UTF8)
			{
				const char* p = (const char*)(m_pBuffer + m_RdPos);
				if (size + m_RdPos <= m_nSize)
				{

					string temp;
					temp.assign(p, p + size);
					str = StringUtil::UTF8_to_UNICODE(temp);
				}
				m_RdPos += size;
			}
		}
		return *this;
	}

	void MemoryStream::Init(void* buffer, std::size_t size, bool bManaged, StringEncodeType typeName)
	{
		m_pBuffer = (unsigned char*)buffer;
		m_nSize = size;
		m_WtPos = size;
		m_RdPos = 0;
		m_bManaged = bManaged;
		m_type = typeName;
	}

	void MemoryStream::Init(std::size_t size, std::size_t sizeAdd, StringEncodeType typeName)
	{
		m_pBuffer = new unsigned char[size];
		memset(m_pBuffer, 0, size);
		m_nSize = size;
		m_WtPos = 0;
		m_RdPos = 0;
		m_bManaged = true;
		m_type = typeName;
		m_SizeAdd = sizeAdd;
	}

	unsigned char* MemoryStream::TakeStream(std::size_t size)
	{
		if (m_pBuffer && m_RdPos + size <= m_nSize)
		{
			unsigned char* p = m_pBuffer + m_RdPos;
			m_RdPos += size;
			return p;
		}
		return nullptr;
	}

	unsigned char* MemoryStream::GetDataPtr()
	{
		return m_pBuffer;
	}

	std::size_t MemoryStream::GetReadPosition()
	{
		return m_RdPos;
	}

	std::size_t MemoryStream::GetWritePosition()
	{
		return m_WtPos;
	}

	std::size_t MemoryStream::GetLength()
	{
		return m_WtPos;
	}

	void MemoryStream::SetReadPosition(std::size_t lPos)
	{
		m_RdPos = lPos;
		return;
	}

	bool MemoryStream::ResizeSpace(std::size_t sizeAdd)
	{
		if (sizeAdd < m_SizeAdd)
		{
			void* p = realloc(m_pBuffer, m_nSize + m_SizeAdd);
			if (p == NULL)return false;
			m_pBuffer = (unsigned char*)p;
			m_nSize += m_SizeAdd;
		}
		else
		{
			void* p = realloc(m_pBuffer, m_nSize + sizeAdd + m_SizeAdd);
			if (p == NULL)return false;
			m_pBuffer = (unsigned char*)p;
			m_nSize += m_SizeAdd + sizeAdd;
		}
		return true;
	}
}