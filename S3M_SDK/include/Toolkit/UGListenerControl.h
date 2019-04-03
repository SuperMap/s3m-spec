

#if !defined(UGC_UGLISTENERCONTROL_H)
#define UGC_UGLISTENERCONTROL_H

#include "Stream/ugdefs.h"

namespace UGC{

	// 所有UGC通过侦听者模式提供回调功能的参数类型
	class TOOLKIT_API UGEventArgs
	{
	public:
		UGEventArgs()
		{
			// do nothing
		}

		virtual ~UGEventArgs()
		{
			// do nothing
		}
	};

	// 所有UGC通过侦听者模式提供回调功能的Listener类型
	class TOOLKIT_API UGListener
	{
	protected:
		UGListener()
		{
			// do nothing
		}
	public:
		virtual ~UGListener()
		{
			// do nothing
		}

		virtual void Run(void* pSender,UGEventArgs* pArgs) = 0;


	};
class  TOOLKIT_API UGListenerControler
{
public:
	UGListenerControler();
	~UGListenerControler();
	void  AddListener(UGListener* lis);
	void  RemoveListener(UGListener*& lis);
	void  RemoveAllListener();
	void Fire(void* pSender,UGEventArgs* pEventArgs);

	UGbool IsEmpty();
	
private:
	UGList<UGListener*> m_ListListener;
	
	
};

}//namespace UGC



#endif

