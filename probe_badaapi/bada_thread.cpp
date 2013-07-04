/*
 *  DA probe
 *
 * Copyright (Thread::*c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Yeongtaik Byeon <yeongtaik.byeon@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

//#include <FBase.h>
#include <FBaseDataType.h>
#include <FBaseRtIRunnable.h>
#include <FBaseCol.h>
//#include <FIo.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"

#include "binproto.h"

static enum DaOptions _sopt = OPT_THREAD;

extern	__thread unsigned int gProbeDepth;

namespace Tizen {
namespace Base {
namespace Runtime {
enum ThreadType {
	THREAD_TYPE_WORKER = 0, /**< The worker thread mode */
	THREAD_TYPE_EVENT_DRIVEN, /**< The event-driven thread mode */
	THREAD_TYPE_MAIN
// This enum value is for internal use only. Using this enum value can cause behavioral,
// security-related, and consistency-related issues in the application.
// The main thread mode
};

enum ThreadPriority {
	THREAD_PRIORITY_HIGH, /**< The high priority*/
	THREAD_PRIORITY_MID, /**< The mid priority*/
	THREAD_PRIORITY_LOW,
/**< The low priority*/
};

class Thread {
public:
	const static unsigned long DEFAULT_STACK_SIZE = 64 * 1024;

	static result Sleep(long milliSeconds);
	static Thread* GetCurrentThread(void);
	static result Yield(void);
	static result Exit(int exitCode=0x00);

	result Construct(ThreadType threadType, long stackSize, // deprecated
			ThreadPriority priority = THREAD_PRIORITY_MID);
	result Construct(long stackSize = DEFAULT_STACK_SIZE,
			ThreadPriority priority = THREAD_PRIORITY_MID);
	result Construct(const Tizen::Base::String &name, long stackSize =
			DEFAULT_STACK_SIZE, ThreadPriority priority = THREAD_PRIORITY_MID);
	result Construct(const Tizen::Base::String &name, ThreadType threadType, // deprecated
			long stackSize = DEFAULT_STACK_SIZE, ThreadPriority priority =
					THREAD_PRIORITY_MID);
	result Construct(IRunnable &target, long stackSize = DEFAULT_STACK_SIZE,
			ThreadPriority priority = THREAD_PRIORITY_MID);
	result Construct(const Tizen::Base::String &name, IRunnable &target,
			long stackSize = DEFAULT_STACK_SIZE, ThreadPriority priority =
					THREAD_PRIORITY_MID);
	result GetExitCode(int &exitCode) const;
	const Tizen::Base::String & GetName(void) const;
	result Join(void);
//	virtual bool OnStart(void); // deprecated
//	virtual void OnStop(void); // deprecated
//	virtual void OnUserEventReceivedN(RequestId requestId, // deprecated
//			Tizen::Base::Collection::IList *pArgs);
//	virtual Tizen::Base::Object * Run(void);
//	virtual result SendUserEvent(RequestId requestId, // deprecated
//			const Tizen::Base::Collection::IList *pArgs);
	result Start(void);
	result Stop(void); // deprecated

//	Thread(void);
//	virtual ~Thread(void);

//private:
//	Thread(const Thread& rhs);
//	Thread& operator =(const Thread& rhs);
//private:
//	friend class _ThreadImpl;
//	class _ThreadImpl* __pThreadImpl;
};

class _ThreadImpl {
//public:
//	virtual result Stop(void);
protected:
//	virtual result Finalize(void);
	const Thread* GetThread(void) const;
private:
	static void* ThreadProc(void* pParam);
//protected:
//	Thread* _pThread;
};
//const Thread*
//_ThreadImpl::GetThread(void) const
//{
//	typedef const Thread*
//	(_ThreadImpl::*methodType)(void) const;
//	static methodType GetThreadp = 0;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = false;
//	void *tmpPtr;
//
//	if (!GetThreadp) {
//		probeBlockStart();
//		void* lib_handle = dlopen("libosp-appfw.so", RTLD_LAZY);
//		if (lib_handle == NULL) {
//			perror("dlopen failed : libosp-appfw.so");
//			exit(0);
//		}
//		tmpPtr = dlsym(lib_handle, "_ZNK5Tizen4Base7Runtime11_ThreadImpl9GetThreadEv");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			perror("dlsym failed : Tizen::Base::Runtime::_ThreadImpl::GetThreadp");
//			exit(0);
//		}
//
//		memcpy(&GetThreadp, &tmpPtr, sizeof(tmpPtr));
//
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
//	//
//	const Thread* ret = (this->*GetThreadp)();
//	result res = GetLastResult();
//	//
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "_ThreadImpl::GetThread", probeInfo.currentTime,
//				probeInfo.pID, probeInfo.tID);
//
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,0x%x",ret);
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//		log.length += sprintf(log.data + log.length,
//				"`,0`,%ld`,%d`,0`,%u`,0x%x`,%d`,%d`,`,", res,blockresult,
//				(unsigned int)CALLER_ADDRESS, (unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//	return ret;
//}

class EventDrivenThread : public Thread {
public:
	result Construct(long stackSize = DEFAULT_STACK_SIZE,
			ThreadPriority priority = THREAD_PRIORITY_MID);
	result Construct(const Tizen::Base::String &name, long stackSize =
			DEFAULT_STACK_SIZE, ThreadPriority priority = THREAD_PRIORITY_MID);

	virtual result Quit();
//	virtual bool OnStart(void);
//	virtual void OnStop(void);
//	virtual void OnUserEventReceivedN(RequestId requestId,
//			Tizen::Base::Collection::IList *pArgs);
//	virtual Tizen::Base::Object * Run(void);
//	virtual result SendUserEvent(RequestId requestId,
//			const Tizen::Base::Collection::IList *pArgs);

//	EventDrivenThread(void);
//	virtual ~EventDrivenThread(void);

//private:
//	EventDrivenThread(const EventDrivenThread& rhs);
//	EventDrivenThread& operator = (const EventDrivenThread& rhs);
//private:
//	class _EventDrivenThreadImpl* __pEventDrivenThreadImpl;
//	friend class _EventDrivenThreadImpl;
};


void*
_ThreadImpl::ThreadProc(void* params) {
	typedef void*
	(*methodType)(void*);
	static methodType ThreadProcp = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = false;
	void *tmpPtr;

	if (!ThreadProcp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime11_ThreadImpl10ThreadProcEPv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::_ThreadImpl::ThreadProc");
			exit(0);
		}

		memcpy(&ThreadProcp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		_ThreadImpl* pSelf =(_ThreadImpl*)params;
		const Thread* pThread = NULL;
		if(pSelf != null){
			pThread = pSelf->GetThread();
		}

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", params);
		PACK_COMMON_END((unsigned int)pSelf, 0, blockresult);
		PACK_THREAD((unsigned int)pThread, THREAD_OSPTHREAD_WORKER, THREAD_API_INTERNAL_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	// all probe should be reachable inside thread start_routine (user implemented Thread::Run)
	gProbeDepth--;
	TRACE_STATE_UNSET(TS_ENTER_PROBE_BLOCK);
	(ThreadProcp)(params);
	TRACE_STATE_SET(TS_ENTER_PROBE_BLOCK);
	gProbeDepth++;
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		_ThreadImpl* pSelf =(_ThreadImpl*)params;
		const Thread* pThread = NULL;
		if(pSelf != null){
			pThread = pSelf->GetThread();
		}

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "p", params);
		PACK_COMMON_END((unsigned int)pSelf, 0, blockresult);
		PACK_THREAD((unsigned int)pThread, THREAD_OSPTHREAD_WORKER, THREAD_API_INTERNAL_STOP);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return null;
}
//result _ThreadImpl::Stop(void) {
//	typedef result
//	(_ThreadImpl::*methodType)(void);
//	static methodType Stopp = 0;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = true;
//	void *tmpPtr;
//
//	if (!Stopp) {
//		probeBlockStart();
//		void* lib_handle = dlopen("libosp-appfw.so", RTLD_LAZY);
//		if (lib_handle == NULL) {
//			perror("dlopen failed : libosp-appfw.so");
//			exit(0);
//		}
//		tmpPtr = dlsym(lib_handle, "_ZN5Tizen4Base7Runtime11_ThreadImpl4StopEv");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			perror("dlsym failed : Tizen::Base::Runtime::_ThreadImpl::Stopp");
//			exit(0);
//		}
//
//		memcpy(&Stopp, &tmpPtr, sizeof(tmpPtr));
//
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
//	//
//	result ret= (this->*Stopp)();
//	//
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "_ThreadImpl::Stop", probeInfo.currentTime,
//				probeInfo.pID, probeInfo.tID);
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,%ld", ret);
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//		log.length += sprintf(log.data + log.length,
//				"`,0`,%ld`,%d`,0`,%u`,0x%x`,%d`,%d`,`,", ret, blockresult,
//				(unsigned int)CALLER_ADDRESS, (unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_CLOSE);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//	return ret;
//}
//
//result _ThreadImpl::Finalize(void) {
//	typedef result
//	(_ThreadImpl::*methodType)(void);
//	static methodType Finalizep = 0;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = false;
//	void *tmpPtr;
//
//	if (!Finalizep) {
//		probeBlockStart();
//		void* lib_handle = dlopen("libosp-appfw.so", RTLD_LAZY);
//		if (lib_handle == NULL) {
//			perror("dlopen failed : libosp-appfw.so");
//			exit(0);
//		}
//		tmpPtr = dlsym(lib_handle, "_ZN5Tizen4Base7Runtime11_ThreadImpl8FinalizeEv");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			perror("dlsym failed : Tizen::Base::Runtime::_ThreadImpl::Finalizep");
//			exit(0);
//		}
//
//		memcpy(&Finalizep, &tmpPtr, sizeof(tmpPtr));
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
//	//
//	result ret= (this->*Finalizep)();
//	//
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "_ThreadImpl::Finalize", probeInfo.currentTime,
//				probeInfo.pID, probeInfo.tID);
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,%ld", ret);
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//		log.length += sprintf(log.data + log.length,
//				"`,0`,%ld`,%d`,0`,%u`,0x%x`,%d`,%d`,`,", ret, blockresult,
//				(unsigned int)CALLER_ADDRESS, (unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_CLOSE);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//	return ret;
//}
//Thread::Thread(void) {
//	typedef void (Thread::*methodType)();
//	static methodType Threadp = 0;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = true;
//	void *tmpPtr;
//
//	if (!Threadp) {
//		probeBlockStart();
//
//		tmpPtr = dlsym(RTLD_NEXT, "_ZN5Tizen4Base7Runtime6ThreadC1Ev");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			log.type = 0;
//											log.length = 0;
//											log.data[0] = '\0';
//											log.length = sprintf(log.data,"dlsym failed : Tizen::Base::Runtime::Thread::Thread");
//			perror("dlsym failed : Tizen::Base::Runtime::Thread::Thread");
//			printLog(&log, MSG_MSG);
//			return;
//		}
//
//		memcpy(&Threadp, &tmpPtr, sizeof(tmpPtr));
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
////
//	(this->*Threadp)();
////
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "Thread::Thread", probeInfo.currentTime,
//				probeInfo.pID, probeInfo.tID);
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,");
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//
//		log.length += sprintf(log.data + log.length,
//				"`,0`,0`,%d`,0`,0x%x`,%d`,%d`,`,", blockresult,
//				(unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_OPEN);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//}
//
//Thread::~Thread(void) {
//	typedef void (Thread::*methodType)();
//	static methodType ThreadDp = 0;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = true;
//	void *tmpPtr;
//
//	if (!ThreadDp) {
//		probeBlockStart();
//
//		void* lib_handle = dlopen("libosp-appfw.so", RTLD_LAZY);
//		if (lib_handle == NULL) {
//			perror("dlopen failed : libosp-appfw.so");
//			exit(0);
//		}
//		tmpPtr = dlsym(lib_handle, "_ZN5Tizen4Base7Runtime6ThreadD0Ev");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			perror("dlsym failed : Tizen::Base::Runtime::Thread::~Thread");
//			exit(0);
//		}
//
//		memcpy(&ThreadDp, &tmpPtr, sizeof(tmpPtr));
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
////
//	(this->*ThreadDp)();
////
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "Thread::~Thread", probeInfo.currentTime,
//				probeInfo.pID, probeInfo.tID);
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,");
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//
//		log.length += sprintf(log.data + log.length,
//				"`,0`,0`,%d`,0`,0x%x`,%d`,%d`,`,", blockresult,
//				(unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_CLOSE);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//}
result Thread::Sleep(long milliSeconds) {
	typedef result
	(*methodType)(long);
	static methodType Sleepp = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Sleepp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread5SleepEl");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Sleep");
			exit(0);
		}

		memcpy(&Sleepp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		Thread *currentThread;
		currentThread = GetCurrentThread();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "x", milliSeconds);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int)currentThread, THREAD_OSPTHREAD_WORKER, THREAD_API_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	result ret = (Sleepp)(milliSeconds);
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		Thread *currentThread;
		currentThread = GetCurrentThread();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "x", milliSeconds);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int)currentThread, THREAD_OSPTHREAD_WORKER, THREAD_API_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

Thread* Thread::GetCurrentThread(void) {
	typedef Thread*
		(*methodType)(void);
	static methodType GetCurrentThreadp = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!GetCurrentThreadp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread16GetCurrentThreadEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::GetCurrentThread");
			exit(0);
		}

		memcpy(&GetCurrentThreadp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	Thread *currentThread = (GetCurrentThreadp)();
	result res = GetLastResult();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END((unsigned int)currentThread, res, blockresult);
		PACK_THREAD((unsigned int)currentThread, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return currentThread;
}

result Thread::Yield(void) {
	typedef result
		(*methodType)(void);
	static methodType Yieldp = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Yieldp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread5YieldEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Yield");
			exit(0);
		}

		memcpy(&Yieldp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	result ret = (Yieldp)();
	//
	if (postBlockBegin(blockresult)) {
		Thread *currentThread;
		currentThread = GetCurrentThread();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int)currentThread, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Thread::Exit(int exitCode) {
	typedef result
		(*methodType)(int exitCode);
	static methodType Exitp = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Exitp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread4ExitEi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Exit");
			exit(0);
		}

		memcpy(&Exitp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		Thread *currentThread;
		currentThread = GetCurrentThread();

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "d", exitCode);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int)currentThread, THREAD_OSPTHREAD_WORKER, THREAD_API_EXIT);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	result ret = (Exitp)(exitCode);
	//
	return ret;
}

result Thread::Construct(ThreadType threadType, long stackSize,
		ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(ThreadType threadType, long stackSize,
			ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime6Thread9ConstructENS1_10ThreadTypeElNS1_14ThreadPriorityE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(threadType, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "dxd", threadType, stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Construct(long stackSize, ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(long stackSize, ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW],
				"_ZN5Tizen4Base7Runtime6Thread9ConstructElNS1_14ThreadPriorityE");
		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "xd", stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Construct(const Tizen::Base::String &name, long stackSize,
		ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(const Tizen::Base::String &name, long stackSize,
			ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime6Thread9ConstructERKNS0_6StringElNS1_14ThreadPriorityE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(name, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "sxd", temp, stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Construct(const Tizen::Base::String &name, ThreadType threadType,
		long stackSize, ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(const Tizen::Base::String &name, ThreadType threadType,
			long stackSize, ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}

		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime6Thread9ConstructERKNS0_6StringENS1_10ThreadTypeElNS1_14ThreadPriorityE");
		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(name, threadType, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, name.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "sdxd", temp, threadType, stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Construct(IRunnable &target, long stackSize,
		ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(IRunnable &target, long stackSize,
			ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime6Thread9ConstructERNS1_9IRunnableElNS1_14ThreadPriorityE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(target, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "xxd", (unsigned int) &target, stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Construct(const Tizen::Base::String &name, IRunnable &target,
		long stackSize, ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(const Tizen::Base::String &name, IRunnable &target,
			long stackSize, ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime6Thread9ConstructERKNS0_6StringERNS1_9IRunnableElNS1_14ThreadPriorityE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(name, target, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, name.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "sxxd", temp, (unsigned int) &target, stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Thread::GetExitCode(int &exitCode) const {
	typedef result
	(Thread::*methodType)(int &exitCode) const;
	static methodType GetExitCodep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
//	int exitOld = exitCode;

	if (!GetExitCodep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW],
				"_ZNK5Tizen4Base7Runtime6Thread11GetExitCodeERi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::GetExitCode");
			exit(0);
		}

		memcpy(&GetExitCodep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*GetExitCodep)(exitCode);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "d", exitCode);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

const Tizen::Base::String & Thread::GetName(void) const {
	typedef const Tizen::Base::String &
	(Thread::*methodType)(void) const;
	static methodType GetNamep = 0;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!GetNamep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZNK5Tizen4Base7Runtime6Thread7GetNameEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::GetName");
			exit(0);
		}

		memcpy(&GetNamep, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	const Tizen::Base::String& ret = (this->*GetNamep)();
	result res = GetLastResult();
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, ret.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret.GetPointer(), res, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Thread::Join(void) {
	typedef result
	(Thread::*methodType)(void);
	static methodType Joinp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Joinp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread4JoinEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Join");
			exit(0);
		}

		memcpy(&Joinp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	ret = (this->*Joinp)();
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
//Tizen::Base::Object * Thread::Run(void) {
//	typedef Tizen::Base::Object * (Thread::*methodType)(void);
//	static methodType Runp = 0;
//	Tizen::Base::Object * ret;
//	probeInfo_t probeInfo;
//	log_t log;
//	int blockresult;
//	bool bfiltering = true;
//	void *tmpPtr;
//	log.type = 0;
//				log.length = 0;
//				log.data[0] = '\0';
//				log.length = sprintf(log.data, "call Thread::Run");
//				printLog(&log, MSG_MSG);
//				return null;
//	if (!Runp) {
//		probeBlockStart();
//		void* lib_handle = dlopen("libosp-appfw.so", RTLD_LAZY);
//		if (lib_handle == NULL) {
//			perror("dlopen failed : libosp-appfw.so");
//			log.type = 0;
//			log.length = 0;
//			log.data[0] = '\0';
//			log.length = sprintf(log.data, "dlopen failed :libosp-appfw.so");
//			printLog(&log, MSG_MSG);
//			return null;
//		}
//		tmpPtr =
//				dlsym(
//						RTLD_NEXT,
//						"_ZN5Tizen4Base7Runtime6Thread3RunEv");
//
//		if (tmpPtr == NULL || dlerror() != NULL) {
//			perror("dlsym failed : Thread::Run");
//			log.type = 0;
//			log.length = 0;
//			log.data[0] = '\0';
//			log.length = sprintf(log.data, "dlsym failed : Thread::Run");
//			printLog(&log, MSG_MSG);
//			return null;
//		}
//
//		memcpy(&Runp, &tmpPtr, sizeof(tmpPtr));
//		probeBlockEnd();
//	}
//
//	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
//		setProbePoint(&probeInfo);
//		preBlockEnd();
//	}
//	//
//	ret = (this->*Runp)();
//	//
//	if (postBlockBegin(blockresult)) {
//		log.type = 0;
//		log.length = 0;
//		log.data[0] = '\0';
//		log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d", LC_THREAD,
//				probeInfo.eventIndex, "Thread::Run",
//				probeInfo.currentTime, probeInfo.pID, probeInfo.tID);
//		//Input,ret
//		log.length += sprintf(log.data + log.length, "`,`,0x%x",(unsigned int)ret;
//		//PCAddr,errno,InternalCall,size,FD,FDType,FDApiType,FileSize,FilePath
//
//
//		log.length += sprintf(log.data + log.length,
//				"`,0`,0`,%d`,0`,0x%x`,%d`,%d`,`,", blockresult,
//				(unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_OTHER);
//		//callstack
//		log.length += sprintf(log.data + log.length, "`,\ncallstack_start`,");
//		getBacktraceString(&log, 4096 - log.length - 17);
//		log.length += sprintf(log.data + log.length, "`,callstack_end");
//
//		printLog(&log, MSG_LOG);
//		postBlockEnd();
//	}
//	return ret;
//}

result Thread::Start(void) {
	typedef result (Thread::*methodType)(void);
	static methodType Startp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Startp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread5StartEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Start");
			exit(0);
		}

		memcpy(&Startp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Startp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_START);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}
result Thread::Stop(void) {
	typedef result (Thread::*methodType)(void);
	static methodType Stopp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Stopp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime6Thread4StopEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Thread::Stop");
			exit(0);
		}

		memcpy(&Stopp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Stopp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_WORKER, THREAD_API_STOP);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result EventDrivenThread::Construct(long stackSize, ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(long stackSize, ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW],
				"_ZN5Tizen4Base7Runtime17EventDrivenThread9ConstructElNS1_14ThreadPriorityE");
		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::EventDrivenThread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "xd", stackSize, priority);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_EVENTDRIVEN, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result EventDrivenThread::Construct(const Tizen::Base::String &name, long stackSize, 
		ThreadPriority priority) {
	typedef result
	(Thread::*methodType)(const Tizen::Base::String &name, long stackSize,
			ThreadPriority priority);
	static methodType Constructp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Constructp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr =
				dlsym(
						lib_handle[LIBOSP_APPFW],
						"_ZN5Tizen4Base7Runtime17EventDrivenThread9ConstructERKNS0_6StringElNS1_14ThreadPriorityE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::EventDrivenThread::Construct");
			exit(0);
		}

		memcpy(&Constructp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Constructp)(name, stackSize, priority);
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, name.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "sx", temp, stackSize);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_EVENTDRIVEN, THREAD_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;

}

result EventDrivenThread::Quit() {
	typedef result (Thread::*methodType)(void);
	static methodType Quitp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Quitp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime17EventDrivenThread4QuitEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::EventThread::Quit");
			exit(0);
		}

		memcpy(&Quitp, &tmpPtr, sizeof(tmpPtr));
		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Quitp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_THREAD, LC_THREAD, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, THREAD_OSPTHREAD_EVENTDRIVEN, THREAD_API_STOP);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

}
}
}
