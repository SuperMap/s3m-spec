using namespace std;
namespace S3MB
{
#ifdef WIN32
#ifdef STK_EXPORTS
#define STK_API __declspec(dllexport)
#else
#define STK_API __declspec(dllimport)
#endif
#else
#define STK_API
#endif
}
