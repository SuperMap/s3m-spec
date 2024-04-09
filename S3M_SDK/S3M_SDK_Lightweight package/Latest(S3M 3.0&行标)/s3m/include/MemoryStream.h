#if !defined(MEMORYSTREAM_H__E8796474_8DB7_4AAE_BAD5_171A8EDFC823__INCLUDED_)
#define MEMORYSTREAM_H__E8796474_8DB7_4AAE_BAD5_171A8EDFC823__INCLUDED_

#pragma once
#include "stdafx.h"
#include <string>
#include "S3MBCommon.h"
#include "S3MBUtils.h"

namespace S3MB
{
	class S3MB_API MemoryStream
	{
	public:
		// 构造函数
		MemoryStream();
		~MemoryStream();

		void Close();

	public:
		// 重载操作符
		template<typename T>
		MemoryStream& operator <<(const T& value)
		{
			if (m_pBuffer && m_WtPos + sizeof(T) <= m_nSize)
			{
				*(T*)(m_pBuffer + m_WtPos) = value;
				m_WtPos += sizeof(T);
			}
			else if (m_bManaged)
			{
				ResizeSpace();
				*(T*)(m_pBuffer + m_WtPos) = value;
				m_WtPos += sizeof(T);
			}
			return *this;
		}
		template<typename T>
		MemoryStream& operator >>(T& value)
		{
			if (m_pBuffer && m_RdPos < m_nSize)
			{
				value = *((T*)(m_pBuffer + m_RdPos));
				m_RdPos += sizeof(T);
			}
			return *this;
		}

		MemoryStream& operator <<(const char* pBuffer);
		MemoryStream& operator <<(const std::string str);
		MemoryStream& operator <<(const wchar_t* pBuffer);
		MemoryStream& operator <<(const std::wstring str);

		MemoryStream& operator >>(std::string& str);
		MemoryStream& operator >>(std::wstring& str);

	public:
		// 用于读取流
		void Init(void* buffer, std::size_t size, bool bManaged = false, StringEncodeType typeName = UTF8);
		// 用于写入流
		void Init(std::size_t size = 4096, std::size_t sizeAdd = 4096, StringEncodeType typeName = UTF8);
		// 深拷贝，从此流中固定大小的内存数据到目标内存中
		template<typename T>
		void Load(T* buffer, std::size_t size)
		{
			if (m_pBuffer && m_RdPos + size * sizeof(T) <= m_nSize)
			{
				for (int i = 0; i < size; i++)
				{
					*buffer = *(T*)(m_pBuffer + m_RdPos);
					buffer++;
					m_RdPos += sizeof(T);
				}
			}
		}
		// 深拷贝，从给定内存块中拷贝固定大小的数据到此流中
		template<typename T>
		void Save(T* buffer, std::size_t size)
		{
			if (size == 0)
			{
				return;
			}
			if (m_pBuffer && m_WtPos + size * sizeof(T) < m_nSize)
			{
				for (int i = 0; i < size; i++)
				{
					*(T*)(m_pBuffer + m_WtPos) = *buffer;
					buffer++;
					m_WtPos += sizeof(T);
				}
			}
			else
			{
				do
				{
					ResizeSpace(size * sizeof(T));
				} while (m_WtPos + size * sizeof(T) > m_nSize);

				for (int i = 0; i < size; i++)
				{
					*(T*)(m_pBuffer + m_WtPos) = *buffer;
					buffer++;
					m_WtPos += sizeof(T);
				}
			}
		}
		// 浅拷贝，仅移动指针位置
		unsigned char* TakeStream(std::size_t size);
		// 获取内存流指针
		unsigned char* GetDataPtr();
		// 获取开始/读位置
		std::size_t GetReadPosition();
		// 获取结束/写位置
		std::size_t GetWritePosition();
		// 获取数据长度
		std::size_t GetLength();
		// 设置数据读取位置
		void SetReadPosition(std::size_t lPos);	
		// 重新分配内存大小
		bool ResizeSpace(std::size_t sizeAdd = 4096);

	private:
		unsigned char* m_pBuffer;
		std::size_t m_nSize;// 分配内存总大小
		std::size_t m_SizeAdd;// 内存自动增长大小
		std::size_t m_WtPos;
		std::size_t m_RdPos;
		bool m_bManaged;// 是否托管内存
		StringEncodeType m_type;// 字符串保存编码
	};
}
#endif