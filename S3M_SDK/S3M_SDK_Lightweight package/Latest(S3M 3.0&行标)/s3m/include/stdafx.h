using namespace std;
namespace S3MB
{
#ifdef WIN32
#ifndef S3MB_LIB
#ifdef S3MB_EXPORTS
#define S3MB_API __declspec(dllexport)
#else
#define S3MB_API __declspec(dllimport)
#endif
#else
#define S3MB_API
#endif
#else
#define S3MB_API
#endif
}
