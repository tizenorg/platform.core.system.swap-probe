/*
 *  DA probe
 *
 * Copyright 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Woojin Jung <woojin2.jung@samsung.com>
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 * Anastasia Lyupa <a.lyupa@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */

#include <FBaseResult.h>
#include <FBaseString.h>
#include <FBaseRtTypes.h>

#include <string.h>
#include "daprobe.h"
#include "probeinfo.h"
#include "dahelper.h"

#include "binproto.h"

static enum DaOptions _sopt = OPT_THREAD;

namespace Tizen {
namespace Base {
namespace Runtime {

class Mutex {
public:
	result Create(void);
	result Create(const Tizen::Base::String& name);
	result Release(void);
	result Acquire(void);
	result TryToAcquire(void);

//	Mutex(void);
//	virtual ~Mutex(void);

//private:
//	Mutex(const Mutex& value);
//	Mutex& opereator =(const Mutex& value);

//private:
//	friend class _MutexImpl;
//	class _MutexImpl * __pMutexImpl;
};

class Semaphore {
public:
	result Create(int count = 1);
	result Create(const Tizen::Base::String& name, int count = 1);
	result Acquire(long timeout = INFINITE);
	result TryToAcquire(void);
	result Release(void);

//	Semaphore(void);
//	virtual ~Semaphore(void);

//private:
//	Semaphore(const Semaphore& rhs);
//	Semaphore& operator =(const Semaphore& rhs);

//private:
//	friend class _SemaphoreImpl;
//	class _SemaphoreImpl * __pSemaphoreImpl;
};

class Monitor {
public:
	result Construct(void);
	result Enter(void);
	result Exit(void);
	result Wait(void);
	result Notify(void);
	result NotifyAll(void);

//	Monitor(void);
//	virtual ~Monitor(void);

//private:
//	Monitor(const Monitor& rhs);
//	Monitor& operator =(const Monitor& rhs);

//private:
//	friend class _MonitorImpl;
//	class _MonitorImpl * __pMonitorImpl;
};


result Mutex::Create(void) {
	typedef result
		(Mutex::*methodType)(void);
	static methodType Createp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Createp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime5Mutex6CreateEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Mutex::Create");
			exit(0);
		}

		memcpy(&Createp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Createp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Mutex::Create(const Tizen::Base::String& name) {
	typedef result 
		(Mutex::*methodType)(const Tizen::Base::String& name);
	static methodType Createp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Createp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime5Mutex6CreateERKNS0_6StringE");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Mutex::Create");
			exit(0);
		}

		memcpy(&Createp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Createp)(name);
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, name.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "s", temp);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Mutex::Release(void) {
	typedef result 
		(Mutex::*methodType)(void);
	static methodType Releasep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Releasep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime5Mutex7ReleaseEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Mutex::Release");
			exit(0);
		}

		memcpy(&Releasep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Releasep)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_RELEASE);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Mutex::Acquire(void) {
	typedef result
		(Mutex::*methodType)(void);
	static methodType Acquirep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Acquirep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime5Mutex7AcquireEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Mutex::Acquire");
			exit(0);
		}

		memcpy(&Acquirep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_ACQUIRE_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	ret = (this->*Acquirep)();
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_ACQUIRE_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Mutex::TryToAcquire(void) {
	typedef result 
		(Mutex::*methodType)(void);
	static methodType TryToAcquirep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!TryToAcquirep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime5Mutex12TryToAcquireEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Mutex::TryToAcquire");
			exit(0);
		}

		memcpy(&TryToAcquirep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*TryToAcquirep)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MUTEX, SYNC_API_TRY_ACQUIRE);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}


result Semaphore::Create(int count) {
	typedef result 
		(Semaphore::*methodType)(int count);
	static methodType Createp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Createp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime9Semaphore6CreateEi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Semaphore::Create");
			exit(0);
		}

		memcpy(&Createp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Createp)(count);
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "d", count);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Semaphore::Create(const Tizen::Base::String& name, int count) {
	typedef result 
		(Semaphore::*methodType)(const Tizen::Base::String& name, int count);
	static methodType Createp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;
	char temp[50];

	if (!Createp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime9Semaphore6CreateERKNS0_6StringEi");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Semaphore::Create");
			exit(0);
		}

		memcpy(&Createp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Createp)(name, count);
	//
	if (postBlockBegin(blockresult)) {

		WcharToChar(temp, name.GetPointer());

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "sd", temp, count);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;

}

result Semaphore::Acquire(long timeout) {
	typedef result
		(Semaphore::*methodType)(long timeout);
	static methodType Acquirep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Acquirep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime9Semaphore7AcquireEl");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Semaphore::Acquire");
			exit(0);
		}

		memcpy(&Acquirep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "x", timeout);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_ACQUIRE_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	ret = (this->*Acquirep)(timeout);
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "x", timeout);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_ACQUIRE_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Semaphore::TryToAcquire(void) {
	typedef result 
		(Semaphore::*methodType)(void);
	static methodType TryToAcquirep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!TryToAcquirep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime9Semaphore12TryToAcquireEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Semaphore::TryToAcquire");
			exit(0);
		}

		memcpy(&TryToAcquirep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*TryToAcquirep)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_TRY_ACQUIRE);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Semaphore::Release(void) {
	typedef result 
		(Semaphore::*methodType)(void);
	static methodType Releasep = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Releasep) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime9Semaphore7ReleaseEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Semaphore::Release");
			exit(0);
		}

		memcpy(&Releasep, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Releasep)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_SEMAPHORE, SYNC_API_RELEASE);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}


result Monitor::Construct(void) {
	typedef result 
		(Monitor::*methodType)(void);
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
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor9ConstructEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::Construct");
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
	ret = (this->*Constructp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_NEW);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Monitor::Enter(void) {
	typedef result
		(Monitor::*methodType)(void);
	static methodType Enterp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Enterp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor5EnterEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::Enter");
			exit(0);
		}

		memcpy(&Enterp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_ACQUIRE_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	ret = (this->*Enterp)();
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_ACQUIRE_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Monitor::Exit(void) {
	typedef result 
		(Monitor::*methodType)(void);
	static methodType Exitp = 0;
	result ret;
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
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor4ExitEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::Exit");
			exit(0);
		}

		memcpy(&Exitp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Exitp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_RELEASE);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Monitor::Wait(void) {
	typedef result
		(Monitor::*methodType)(void);
	static methodType Waitp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Waitp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor4WaitEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::Wait");
			exit(0);
		}

		memcpy(&Waitp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(0, 0, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_COND_WAIT_START);
		FLUSH_LOCAL_BUF();

		preBlockEnd();
	}
	//
	ret = (this->*Waitp)();
	//
	if (postBlockBegin(blockresult)) {
		setProbePoint(&probeInfo);

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_COND_WAIT_END);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Monitor::Notify(void) {
	typedef result 
		(Monitor::*methodType)(void);
	static methodType Notifyp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!Notifyp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor6NotifyEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::Notify");
			exit(0);
		}

		memcpy(&Notifyp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*Notifyp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_NOTIFY);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

result Monitor::NotifyAll(void) {
	typedef result 
		(Monitor::*methodType)(void);
	static methodType NotifyAllp = 0;
	result ret;
	probeInfo_t probeInfo;
	int blockresult;
	bool bfiltering = true;
	void *tmpPtr;

	if (!NotifyAllp) {
		probeBlockStart();
		if(lib_handle[LIBOSP_APPFW] == NULL) {
			lib_handle[LIBOSP_APPFW] = dlopen(lib_string[LIBOSP_APPFW], RTLD_LAZY);
			if (lib_handle[LIBOSP_APPFW] == NULL) {
				perror("dlopen failed : libosp-appfw.so");
				exit(0);
			}
		}
		tmpPtr = dlsym(lib_handle[LIBOSP_APPFW], "_ZN5Tizen4Base7Runtime7Monitor9NotifyAllEv");

		if (tmpPtr == NULL || dlerror() != NULL) {
			perror("dlsym failed : Tizen::Base::Runtime::Monitor::NotifyAll");
			exit(0);
		}

		memcpy(&NotifyAllp, &tmpPtr, sizeof(tmpPtr));

		probeBlockEnd();
	}

	if ((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {
		setProbePoint(&probeInfo);
		preBlockEnd();
	}
	//
	ret = (this->*NotifyAllp)();
	//
	if (postBlockBegin(blockresult)) {

		PREPARE_LOCAL_BUF();
		PACK_COMMON_BEGIN(MSG_PROBE_SYNC, LC_SYNC, "", 0);
		PACK_COMMON_END(ret, ret, blockresult);
		PACK_THREAD((unsigned int) this, SYNC_OSP_MONITOR, SYNC_API_NOTIFY_ALL);
		FLUSH_LOCAL_BUF();

		postBlockEnd();
	}
	return ret;
}

}
}
}
