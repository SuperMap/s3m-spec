#ifndef UGTHREAD_H
#define UGTHREAD_H

#include "Stream/ugdefs.h"
#include "Stream/ugplatform.h"

// 2007-3-26 by zengzm 不得已,还是要放到这里来,因为MFC很变态,把LoadString等都定义为宏
// 在MBCS下, 转变为LoadStringA,导致UGString的LoadString也变为LoadStringA了.
// 所以这些头文件都不能加到ugwin32.h中

#ifdef WIN32
//	#include <afxmt.h>
#endif

namespace UGC {


class UGCondition;

class TOOLKIT_API UGMutex {
  friend class UGCondition;
private:
	unsigned long dataArray[24];	WhiteBox_Ignore
private:
  UGMutex(const UGMutex&);
  UGMutex &operator=(const UGMutex&);
public:

  /// Initialize the mutex
	  //!  \brief Toolkit
  UGMutex(UGbool recursive=FALSE);

  /// Lock the mutex
	  //!  \brief Toolkit
  void lock();

  /// Return true if succeeded locking the mutex
	  //!  \brief Toolkit
  UGbool trylock();

  /// Return true if mutex is already locked
	  //!  \brief Toolkit
  UGbool locked();

  /// Unlock mutex
	  //!  \brief Toolkit
  void unlock();

  /// Delete the mutex
	  //!  \brief Toolkit
  ~UGMutex();
  };



/**
* An easy way to establish aValue correspondence between aValue C++ scope
* and aValue critical section is to simply declare an UGMutexLock
* at the beginning of the scope.
* The mutex will be automatically released when the scope is
* left (either by natural means or by means of an exception.
*/
class TOOLKIT_API UGMutexLock {
private:
  UGMutex& mtx;
private:
  UGMutexLock();
  UGMutexLock(const UGMutexLock&);
  UGMutexLock& operator=(const UGMutexLock&);
	  //!  \brief Toolkit
public:

  /// Construct & lock associated mutex
  
  //UGMutexLock(UGMutex& m):mtx(m){lock();}
  // 2007-5-28 by malq-以前的构造函中执行了lock(),外面使用的时候可能会出问题，所以这里取消，外面用的时候直接调用lock
	  //!  \brief Toolkit
  UGMutexLock(UGMutex& m):mtx(m){}

  /// Return reference to associated mutex
	  //!  \brief Toolkit
  UGMutex& mutex(){ return mtx; }

  
#if !defined SYMBIAN60 /*&& !defined  OS_ANDROID && !defined (IOS)*/
  /// Lock mutex
	  //!  \brief Toolkit
  void lock(){ mtx.lock(); }

  /// Return true if succeeded locking the mutex
	  //!  \brief Toolkit
  UGbool trylock(){ return mtx.trylock(); }

  /// Return true if mutex is already locked
	  //!  \brief Toolkit
  UGbool locked(){ return mtx.locked(); }

  /// Unlock mutex
	  //!  \brief Toolkit
  void unlock(){ mtx.unlock(); }
#else
  /// Lock mutex
  //!  \brief Toolkit
  void lock(){ }

  /// Return true if succeeded locking the mutex
  //!  \brief Toolkit
  UGbool trylock(){return TRUE;}

  /// Return true if mutex is already locked
  //!  \brief Toolkit
  UGbool locked(){ return TRUE;}

  /// Unlock mutex
  //!  \brief Toolkit
  void unlock(){ }
#endif
  /// Destroy and unlock associated mutex
	  //!  \brief Toolkit
  ~UGMutexLock(){ unlock(); }
  };



/**
* A read / write lock allows multiple readers but only aValue single
* writer.
*/
#ifndef SYMBIAN60
class TOOLKIT_API UGReadWriteLock {
private:
  UGuval dataArray[24];
private:
  UGReadWriteLock(const UGReadWriteLock&);
  UGReadWriteLock &operator=(const UGReadWriteLock&);
public:

  /// Initialize the read/write lock
	  //!  \brief Toolkit
  UGReadWriteLock();

  /// Acquire read lock for read/write lock
	  //!  \brief Toolkit
  void readLock();

  /// Try to acquire read lock for read/write lock
	  //!  \brief Toolkit
  bool tryReadLock();

  /// Unlock read lock
	  //!  \brief Toolkit
  void readUnlock();

  /// Acquire write lock for read/write lock
	  //!  \brief Toolkit
  void writeLock();

  /// Try to acquire write lock for read/write lock
	  //!  \brief Toolkit
  bool tryWriteLock();

  /// Unlock write lock
	  //!  \brief Toolkit
  void writeUnlock();

  /// Delete the read/write lock
	  //!  \brief Toolkit
  ~UGReadWriteLock();
  };
#endif

//! \brief 自动锁。
class UGAutoLock
{
public:
	explicit UGAutoLock(UGMutex& locker) : m_locker(locker) { m_locker.lock(); }
	virtual ~UGAutoLock() { m_locker.unlock(); }

private:
	UGMutex& m_locker;
};

//! \brief 读者操作锁。
class UGReadLock
{
public:
	explicit UGReadLock(UGReadWriteLock& locker) : m_locker(locker) { m_locker.readLock(); }
	virtual ~UGReadLock() { m_locker.readUnlock(); }

private:
	UGReadLock(const UGReadLock&);
	UGReadLock& operator= (const UGReadLock&);

private:
	UGReadWriteLock& m_locker;
};

//! \brief 作者操作锁。
class UGWriteLock
{
public:
	explicit UGWriteLock(UGReadWriteLock& locker) : m_locker(locker) { m_locker.writeLock(); }
	virtual ~UGWriteLock() { m_locker.writeUnlock(); }

private:
	UGWriteLock(const UGWriteLock&);
	UGWriteLock& operator= (const UGWriteLock&);

private:
	UGReadWriteLock& m_locker;
};

/**
* A condition allows one or more threads to synchronize
* to an event.  When aValue thread calls wait, the associated
* mutex is unlocked while the thread is blocked.  When the
* condition becomes signaled, the associated mutex is
* locked and the thread(s) are reawakened.
*/
class TOOLKIT_API UGCondition {
private:
  UGuval dataArray[12];
private:
  UGCondition(const UGCondition&);
  UGCondition& operator=(const UGCondition&);
public:

  /// Initialize the condition
	  //!  \brief Toolkit
  UGCondition();

  /**
  * Wait until condition becomes signalled, using given mutex,
  * which must already have been locked prior to this call.
  */
	  //!  \brief Toolkit
  void Wait(UGMutex& mtx);

  /**
  * Wait until condition becomes signalled, using given mutex,
  * which must already have been locked prior to this call.
  * Returns true if successful, false if timeout occurred.
  * Note that the wait-time is specified in nanoseconds
  * since the Epoch (Jan 1, 1970).
  */
	  //!  \brief Toolkit
  UGbool Wait(UGMutex& mtx,UGuint nsec);
  /**
  * Wake or unblock aValue single blocked thread
  */
	  //!  \brief Toolkit
  void signal();

  /**
  * Wake or unblock all blocked threads
  */
	  //!  \brief Toolkit
  void broadcast();

  /// Delete the condition
	  //!  \brief Toolkit
  ~UGCondition();
  };



/**
* A semaphore allows for protection of aValue resource that can
* be accessed by aValue fixed number of simultaneous threads.
*/
class TOOLKIT_API UGSemaphore {
private:
#if defined IOS 
    //IOS need named-Semaphore
    UGint* dataValue;
    char semaphoreName [10]; 
#else
    UGuval dataValues[16];
#endif

private:
  UGSemaphore(const UGSemaphore&);
  UGSemaphore& operator=(const UGSemaphore&);
public:

  /// Initialize semaphore with given count
	  //!  \brief Toolkit
  UGSemaphore(UGint initial=1);

  /// Decrement semaphore
	  //!  \brief Toolkit
  void Wait();

  /// Non-blocking semaphore decrement; return true if locked
	  //!  \brief Toolkit
  UGbool TryWait();

  /// Increment semaphore
	  //!  \brief Toolkit
  void Post();

  /// Delete semaphore
	  //!  \brief Toolkit
  ~UGSemaphore();
  };



/**
* UGThread provides system-independent support for threads.
* Subclasses must implement the run() function do implement
* the desired functionality of the thread.
* The storage of the UGThread object is to be managed by the
* calling thread, not by the thread itself.
*/
#ifndef SYMBIAN60
class TOOLKIT_API UGThread {
private:
  volatile UGThreadID tid;
  volatile UGuint thlid;
private:
  UGThread(const UGThread&);
  UGThread &operator=(const UGThread&);

  static UGHandle UGCALLBACK execute(void*);

public:
  // added by zengzm 2007-06-04 得到当前线程id
  //! \brief 得到当前线程的ID
  //! \return 返回当前线程的ID
#ifdef IOS
    static UGulong GetCurrentThreadId();
#else
    static UGuint GetCurrentThreadId();
#endif
public:

  /// 线程策略级别
  enum Priority {
    PRIORITY_ERROR=-1,  /// Failed to get priority
	  //!  \brief Toolkit
    PRIORITY_DEFAULT,  	/// Default scheduling priority
	  //!  \brief Toolkit
    PRIORITY_MINIMUM,   /// Minimum scheduling priority
	  //!  \brief Toolkit
    PRIORITY_LOWER,     /// Lower scheduling priority   
	  //!  \brief Toolkit
    PRIORITY_MEDIUM,    /// Medium priority           
	  //!  \brief Toolkit
    PRIORITY_HIGHER,    /// Higher scheduling priority  
	  //!  \brief Toolkit
    PRIORITY_MAXIMUM    /// Maximum scheduling priority
    };

  /// Thread scheduling policies
  enum Policy {
	  //!  \brief Toolkit
    POLICY_ERROR=-1,    /// Failed to get policy
	  //!  \brief Toolkit
    POLICY_DEFAULT,     /// Default scheduling     
	  //!  \brief Toolkit
    POLICY_FIFO,        /// First in, first out scheduling
	  //!  \brief Toolkit
    POLICY_ROUND_ROBIN 	/// Round-robin scheduling 
    };

protected:

  /**
  * All threads execute by deriving the run method of UGThread.
  * If an uncaught exception was thrown, this function returns -1.
  */
  virtual UGint run() = 0;

public:

  /// Initialize thread object.
	  //!  \brief Toolkit
  UGThread();

  /**
  * Return handle of this thread object.
  * This handle is valid in the context of the thread which
  * called start().
  */
	  //!  \brief Toolkit
  UGThreadID id() const;

  UGuint getcallingthreadid();

  /**
  *  返回表示线程是否属于运行状态
  */
	  //!  \brief Toolkit
  UGbool running() const;

  /**
  *  启动线程
  */
	  //!  \brief Toolkit
  UGbool start(UGulong stacksize=0);	

  /**
  * 挂起调用线程，直到线程结束，Suspend calling thread until thread is done.
  */
	  //!  \brief Toolkit
  UGbool join();

  /**
  * 挂起调用线程，直到线程结束 Suspend calling thread until thread is done, and set code to the return value of run() or the argument passed into exit().
  * If an exception happened in the thread, return -1.
  */	  
  UGbool join(UGint& code);

  /**
  * 取消线程 Cancel the thread
  */	  
  UGbool cancel();

  UGbool detach();

  /**
  * 退出线程
  */
	  //!  \brief Toolkit
  static void exit(UGint code=0);
  
  static void yield();

  /**
  * 返回时间，Return time in nanoseconds since Epoch (Jan 1, 1970).
  */
	  //!  \brief Toolkit
  static UGulong time();

  static void sleep(UGuint nsec);

  static void wakeat(UGuint nsec);

  static UGThread* self();

  static UGThreadID current();

  UGbool priority(Priority prio);

  Priority priority() const;

  UGbool policy(Policy plcy);

  Policy policy() const;
  
  virtual ~UGThread();
  };

#else
class TOOLKIT_API UGThread {
public:
  // added by zengzm 2007-06-04 得到当前线程id
  //! \brief 得到当前线程的ID
  //! \return 返回当前线程的ID
  static UGuint GetCurrentThreadId();
  };
#endif


class UGThreadEx;
class TOOLKIT_API UGThreadManager
{
	friend class UGThreadEx;
	//!  \brief 回调函数，线程的回调函数
	typedef  UGint (*ThreadFunCallBack)(void*);
public:
	~UGThreadManager();
private:
	UGThreadManager();
	UGThreadManager(const UGThreadManager&);
	UGThreadManager &operator=(const UGThreadManager&);
public:
	//获取工厂类的唯一实例
	static UGThreadManager& GetInstance();
	//!  \brief 创建一个线程，根据线程名字，线程函数，线程函数参数，是否托管
	//!  isManaged 主要线程如果设置为托管，则在线程结束的时候自动调用UGThreadFactory 类删除自己
	//! 如果 strName有重名或者是空的，pFun为空上述情况会创建失败
	UGThreadEx* CreateThread(const UGString& strName,ThreadFunCallBack pFun,void* pParameter,UGbool isManaged =FALSE);
	//!  \brief 删除一个线程
	//!  \param 如果线程没有终止，线程会强制杀死，会有一些不可知的问题，不推荐这样做，推荐自己实现
	//thread正常杀死和return
	void DestoryThread(const UGString& strName);
	//!  \brief 获得当前机器的最优线程个数
	UGuint GetOptimalCount() const;
	//!  \brief 获取当前有效的线程个数，指管理线程的个数
	UGint GetActiveCount();
	//!  \brief 获取线程
	//! \brief 如果线程不存在则返回NULL
	UGThreadEx* GetThread(const UGString& strName);
	//!  \brief 取消托管
	//! \brief 如果线程不存在则返回失败
	UGbool CancleManaged(const UGString& strName);
private:
	//!  \brief 删除一个线程,这个方法仅供UGThreadEx内部调用，和DestoryThread保持一致
	// 主要用于托管删除
	void DestoryManagedThread(const UGString& strName);

	UGMutex m_mutex;
	UGArray<UGThreadEx*> m_arrayThread;
};


class TOOLKIT_API UGThreadEx 

{
	friend class UGThreadManager;

	static UGHandle UGCALLBACK execute(void*);

private:
	UGThreadEx(const UGThreadEx&);
	UGThreadEx &operator=(const UGThreadEx&);
public:
	UGThreadEx();
	~UGThreadEx();

  /// 线程策略级别
  enum Priority {
    PRIORITY_ERROR=-1,  /// Failed to get priority
	  //!  \brief Toolkit
    PRIORITY_DEFAULT,  	/// Default scheduling priority
	  //!  \brief Toolkit
    PRIORITY_MINIMUM,   /// Minimum scheduling priority
	  //!  \brief Toolkit
    PRIORITY_LOWER,     /// Lower scheduling priority   
	  //!  \brief Toolkit
    PRIORITY_MEDIUM,    /// Medium priority           
	  //!  \brief Toolkit
    PRIORITY_HIGHER,    /// Higher scheduling priority  
	  //!  \brief Toolkit
    PRIORITY_MAXIMUM    /// Maximum scheduling priority
    };

  /// Thread scheduling policies
  enum Policy {
	  //!  \brief Toolkit
    POLICY_ERROR=-1,    /// Failed to get policy
	  //!  \brief Toolkit
    POLICY_DEFAULT,     /// Default scheduling     
	  //!  \brief Toolkit
    POLICY_FIFO,        /// First in, first out scheduling
	  //!  \brief Toolkit
    POLICY_ROUND_ROBIN 	/// Round-robin scheduling 
    };

	enum Thread_Status
	{
	  //至今尚未启动的线程处于这种状态。 
	  THREAD_NEW,
	  //正在执行的线程处于这种状态。 
	  THREAD_RUNNING,
	  //受阻塞并等待某个监视器锁的线程处于这种状态。
	  THREAD_BLOCKED, 
	  //无限期地等待另一个线程来执行某一特定操作的线程处于这种状态。 
	  THREAD_WAITING,
	  //等待另一个线程来执行取决于指定等待时间的操作的线程处于这种状态。 
	  THREAD_TIMED_WAITING,
	  //已退出的线程处于这种状态。
	  TERMINATED,
	  // 未知错误
	  THREAD_UNKNOWN=0xFFFFFFFF,
	};

public:
	//! \brief 获取线程的ID
	//! \breif如果线程已经终止，则线程ID返回不准确
	UGuint GetID(){return m_nID;};

	//! \brief 获取线程的Name;
	UGString GetName();

	//! \brief 设置线程启动时候的堆栈大小
	//!   必须在start之前调用
	void SetStackSize(UGuint nSize);

	//! \brief 设置线程启动时候的优先级大小
	//!   最好在start之前调用
	UGbool SetPriority(Priority prio);
	UGThreadEx::Priority GetPriority() const;

	//! \brief 设置线程的调度策略，这个方法只在linux上面有效
	void SetPolicy(Policy ploi);

	//! \brief 返回线程的状态
	UGThreadEx::Thread_Status GetStatus() const;
	//! \brief测试线程是否处于活动状态
	UGbool IsAlive() const;

	//!  \brief 开始线程
	UGbool Start();

	//!  \brief 挂起调用线程，直到线程结束
	UGbool Join();

	//!  \brief 获得线程结束的返回值
    //! \return 返回当前线程的ID
#ifdef IOS
    UGulong GetReturnValue();
    static UGulong GetCurrentThreadId();
#else
	UGint GetReturnValue();
    static UGuint GetCurrentThreadId();
#endif
	
	//!  \brief 让当前线程暂时睡眠一段时间，单位毫秒
	static void Sleep(UGulong nMillsecond);

	//!  \brief 让当前线程暂停一段时间，让系统调度其他线程，和yield线程的意思保持一致
	static UGbool SwitchToThread();
private:
	void Init();
	//线程ID
#ifdef IOS
    UGulong m_nID;
    UGulong m_nReturnValue;
#else
	UGuint m_nID;
    UGint m_nReturnValue;
#endif
	//线程名字
	UGString m_strName;
	//线程handle 在Linux中没有用到
	void* m_pHandle;
	//在Linux中没有handle的概念，m_nID不变，用下面这个变量表示状态 只在Linux下使用
	Thread_Status m_tSatus;

	UGbool m_bManaged;
	UGuint m_nStackSize;
	Priority m_nPriority;
	Policy m_nPolicy;

public:
	// 开放出来给线程池用
	UGThreadManager::ThreadFunCallBack m_pFun;
	void* m_pParameter;
 };




}

#endif




