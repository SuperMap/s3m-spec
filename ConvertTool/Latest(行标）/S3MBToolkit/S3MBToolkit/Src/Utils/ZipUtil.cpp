#include "Utils/ZipUtil.h"
#include "S3MBUtils.h"

#include "minizip/unzip.h"
#include "zlib.h"

namespace S3MB
{
	// minizip在Windows上无法打开超过2G的文件，需要自己定义文件打开的方法，覆盖原有方法来解决。下面这一段都是为了覆盖之前的方法
#ifdef WIN32
	static void* ZCALLBACK fopen_file_func(void* opaque, const char* filename, int mode)
	{
		FILE* file = nullptr;
		const char* mode_fopen = nullptr;

		if ((mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) == ZLIB_FILEFUNC_MODE_READ)
			mode_fopen = "rb";
		else if (mode & ZLIB_FILEFUNC_MODE_EXISTING)
			mode_fopen = "r+b";
		else if (mode & ZLIB_FILEFUNC_MODE_CREATE)
			mode_fopen = "wb";

		if ((filename != nullptr) && (mode_fopen != nullptr))
			file = fopen(filename, mode_fopen);

		return file;
	}

	static unsigned long ZCALLBACK fread_file_func(void* opaque, void* stream, void* buf, unsigned long size)
	{
		return (unsigned long)fread(buf, 1, (size_t)size, (FILE*)stream);
	}

	static unsigned long ZCALLBACK fwrite_file_func(void* opaque, void* stream, const void* buf, unsigned long size)
	{
		return (unsigned long)fwrite(buf, 1, (size_t)size, (FILE*)stream);
	}

	static long ZCALLBACK ftell_file_func(void* opaque, void* stream)
	{
		return ftell((FILE*)stream);
	}

	static ZPOS64_T ZCALLBACK ftell64_file_func(void* opaque, void* stream)
	{
		return ftello64((FILE*)stream);
	}

	static long ZCALLBACK fseek_file_func(void* opaque, void* stream, unsigned long offset, int origin)
	{
		int fseek_origin = 0;
		long ret = 0;
		switch (origin)
		{
		case ZLIB_FILEFUNC_SEEK_CUR:
			fseek_origin = SEEK_CUR;
			break;
		case ZLIB_FILEFUNC_SEEK_END:
			fseek_origin = SEEK_END;
			break;
		case ZLIB_FILEFUNC_SEEK_SET:
			fseek_origin = SEEK_SET;
			break;
		default:
			return -1;
		}
		if (fseek((FILE*)stream, offset, fseek_origin) != 0)
			ret = -1;
		return ret;
	}

	static long ZCALLBACK fseek64_file_func(void* opaque, void* stream, ZPOS64_T offset, int origin)
	{
		int fseek_origin = 0;
		long ret = 0;
		switch (origin)
		{
		case ZLIB_FILEFUNC_SEEK_CUR:
			fseek_origin = SEEK_CUR;
			break;
		case ZLIB_FILEFUNC_SEEK_END:
			fseek_origin = SEEK_END;
			break;
		case ZLIB_FILEFUNC_SEEK_SET:
			fseek_origin = SEEK_SET;
			break;
		default:
			return -1;
		}
		if (fseeko64((FILE*)stream, offset, fseek_origin) != 0)
			ret = -1;
		return ret;
	}

	static int ZCALLBACK fclose_file_func(void* opaque, void* stream)
	{
		return fclose((FILE*)stream);
	}

	static int ZCALLBACK ferror_file_func(void* opaque, void* stream)
	{
		return ferror((FILE*)stream);
	}

	void fill_ioapi64_filefunc(zlib_filefunc64_def* pzlib_filefunc_def)
	{
		pzlib_filefunc_def->zopen64_file = (open64_file_func)fopen_file_func;
		pzlib_filefunc_def->zread_file = fread_file_func;
		pzlib_filefunc_def->zwrite_file = fwrite_file_func;
		pzlib_filefunc_def->ztell64_file = ftell64_file_func;
		pzlib_filefunc_def->zseek64_file = fseek64_file_func;
		pzlib_filefunc_def->zclose_file = fclose_file_func;
		pzlib_filefunc_def->zerror_file = ferror_file_func;
		pzlib_filefunc_def->opaque = nullptr;
	}
#endif

	bool ZipUtil::GetFile(const std::wstring& strZipFilePath, const std::wstring& strFileName,
		unsigned char*& pBuffer, unsigned long long& nBufferSize)
	{
		std::string zipFilePath = StringUtil::UnicodeToANSI(strZipFilePath);
		std::string fileName = StringUtil::UnicodeToANSI(strFileName);

		unzFile unzfile;
#ifdef WIN32
		zlib_filefunc64_def zlib_filefunc_def;
		fill_ioapi64_filefunc(&zlib_filefunc_def);
		unzfile = unzOpen2_64(zipFilePath.c_str(), &zlib_filefunc_def);
#else
		unzfile = unzOpen64(zipFilePath.c_str());
#endif
		if (!unzfile) {
			return false;
		}

		int err = unzLocateFile(unzfile, fileName.c_str(), 2);
		if (UNZ_OK != err)
		{
			unzClose(unzfile);
			return false;
		}

		unz_file_info file_info;
		err = unzGetCurrentFileInfo(unzfile, &file_info, nullptr, 1024,
			nullptr, 0, nullptr, 0);
		if (UNZ_OK != err)
		{
			unzClose(unzfile);
			return false;
		}

		err = unzOpenCurrentFile(unzfile);
		if (UNZ_OK != err)
		{
			unzClose(unzfile);
			return false;
		}

		nBufferSize = file_info.uncompressed_size;
		pBuffer = new unsigned char[file_info.uncompressed_size];
		if (pBuffer == nullptr)
		{
			unzCloseCurrentFile(unzfile);
			unzClose(unzfile);
			return false;
		}

		err = unzReadCurrentFile(unzfile, pBuffer, file_info.uncompressed_size);
		if (err < 0)
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			nBufferSize = 0;
			unzCloseCurrentFile(unzfile);
			unzClose(unzfile);
			return false;
		}
		unzCloseCurrentFile(unzfile);
		unzClose(unzfile);
		return true;
	}
}
