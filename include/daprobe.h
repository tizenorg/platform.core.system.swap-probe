/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: 
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
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

#ifndef __DAPROBE_H__
#define __DAPROBE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dlfcn.h>
#include "probeinfo.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
#endif

#define NUM_OF_MONITOR		3
#define DA_LOG_MAX			4096
#define PERROR_MSG_MAX		128

#define DEFAULT_TOKEN	"`,"

#define CALLER_ADDRESS	\
	((void*) __builtin_extract_return_addr(__builtin_return_address(0)))

#define BACKTRACE_SYMBOLS cached_backtrace_symbols

typedef enum
{
	MT_MEMORY,
	MT_FILE,
	MT_SOCKET,
} MonitorType;

enum BlockLocation
{
	BL_PRE,
	BL_POST,
};

typedef struct
{
	int type;
	int length;
	char data[DA_LOG_MAX];
} log_t;

typedef struct
{
	int				eventIndex;
	int				pID;
	int				tID;
	unsigned long	currentTime;
	unsigned int	callDepth;
} probeInfo_t;

/***************************************************************************
 * helper apis
 ***************************************************************************/

int preBlockBegin(void* caller, bool bFiltering, enum DaOptions);
void preBlockEnd();

int postBlockBegin(int preresult);
void postBlockEnd();

void probeBlockStart();
void probeBlockEnd();

unsigned int getCurrentEventIndex();
unsigned int getCallDepth();
unsigned long getTraceState();
unsigned long getCurrentTime();
bool setProbePoint(probeInfo_t * iProbe);
int update_heap_memory_size(bool isAdd, size_t size);

bool printLogStr(const char* str, int msgType);
//bool printLog(log_t* log, int msgType);
int __appendTypeLog(log_t* log, int nInput, char* token, ...);
int getBacktraceString(log_t* log, int bufsize);

//char* captureScreenShotX(int* width, int* height);
//void releaseScreenShotX();

//int printSamplingLog(void);

//bool captureScreen(unsigned int logNum);
//void captureProbe();
//int registeScreenChange(int renderID);
//void detectTouchEvent(int touchID);
//int getTouchState();
//int isPossibleCapture();


/***************************************************************************
 * helper macros
 ***************************************************************************/

// ========================= print log =====================================
#define PRINTMSG(msg)	printLogStr(msg, MSG_MSG)

// =========================== declare variables ===========================
// local variable is faster than heap allocated variable
// array variable initialization with declare is expensive than memset

// declare variable for standard api (not tizen related api)
#define DECLARE_VARIABLE_STANDARD		\
		probeInfo_t	probeInfo; 			\
		int blockresult;				\
		bool bfiltering = true;			\
		int olderrno, newerrno;			\
		int __attribute__((unused)) ret

// ========================== get function pointer =========================

#define GET_REAL_FUNCP(FUNCNAME, SONAME, FUNCTIONPOINTER)			\
		do {														\
			if(!FUNCTIONPOINTER) {									\
				probeBlockStart();									\
				if(lib_handle[SONAME] == NULL) {					\
					lib_handle[SONAME] = dlopen(lib_string[SONAME], RTLD_LAZY);			\
					if(lib_handle[SONAME] == NULL) {				\
						char perror_msg[PERROR_MSG_MAX];			\
						sprintf(perror_msg, "dlopen failed : %s", lib_string[SONAME]);	\
						perror(perror_msg);							\
						exit(0);									\
					}												\
				}													\
				FUNCTIONPOINTER = dlsym(lib_handle[SONAME], #FUNCNAME);		\
				if(FUNCTIONPOINTER == NULL || dlerror() != NULL) {	\
					perror("dlsym failed : " #FUNCNAME);			\
					exit(0);										\
				}													\
				probeBlockEnd();									\
			}														\
		} while(0)

#define GET_REAL_FUNC(FUNCNAME, SONAME)		\
		GET_REAL_FUNCP(FUNCNAME, SONAME, FUNCNAME##p)

#define GET_REAL_FUNCP_RTLD_NEXT(FUNCNAME, FUNCTIONPOINTER)			\
		do {														\
			if(!FUNCTIONPOINTER) {									\
				probeBlockStart(); 									\
				FUNCTIONPOINTER = dlsym(RTLD_NEXT, #FUNCNAME);		\
				if(FUNCTIONPOINTER == NULL || dlerror() != NULL) {	\
					perror("dlsym failed");							\
					exit(0);										\
				}													\
				probeBlockEnd(); 									\
			}														\
		} while(0)

#define GET_REAL_FUNC_RTLD_NEXT(FUNCNAME)	\
		GET_REAL_FUNCP_RTLD_NEXT(FUNCNAME, FUNCNAME##p)

#define GET_REAL_FUNCP_RTLD_NEXT_CPP(FUNCNAME, FUNCTIONPOINTER)		\
		do {														\
			if(!FUNCTIONPOINTER) {									\
				probeBlockStart(); 									\
				void* funcp = dlsym(RTLD_NEXT, #FUNCNAME);			\
				if(funcp == NULL || dlerror() != NULL) {			\
					perror("dlsym failed : " #FUNCNAME);			\
					exit(0);										\
				}													\
				memcpy(&FUNCTIONPOINTER, &funcp, sizeof(void*));	\
				probeBlockEnd(); 									\
			}														\
		} while(0)

#define GET_REAL_FUNC_RTLD_NEXT_CPP(FUNCNAME)	\
		GET_REAL_FUNCP_RTLD_NEXT_CPP(FUNCNAME, FUNCNAME##p)

// ======================= pre block macro ================================

#define PRE_PROBEBLOCK_BEGIN()		 											\
	if((blockresult = preBlockBegin(CALLER_ADDRESS, bfiltering, _sopt)) != 0) {	\
		setProbePoint(&probeInfo)

#define PRE_PROBEBLOCK_END()	\
		preBlockEnd();			\
	}							\
	olderrno = errno;			\
	errno = 0

#define PRE_PROBEBLOCK() \
		PRE_PROBEBLOCK_BEGIN(); \
		PRE_PROBEBLOCK_END()

// ===================== unconditional probe block ========================

#define PRE_UNCONDITIONAL_BLOCK_BEGIN()	\
	do {								\
		probeBlockStart(); 				\
		setProbePoint(&probeInfo)

#define PRE_UNCONDITIONAL_BLOCK_END()	\
		probeBlockEnd();				\
	} while(0)

#define POST_UNCONDITIONAL_BLOCK_BEGIN(LCTYPE)	\
	do {										\
		probeBlockStart();						\
		INIT_LOG;								\
		APPEND_LOG_BASIC(LCTYPE)
		
#define POST_UNCONDITIONAL_BLOCK_END()			\
		printLog(&log, MSG_LOG);				\
		probeBlockEnd();						\
	} while(0)

// ========================= simplify macro ================================

#define BEFORE_ORIGINAL(FUNCNAME, LIBNAME)	\
	DECLARE_VARIABLE_STANDARD;				\
	GET_REAL_FUNC(FUNCNAME, LIBNAME);		\
	PRE_PROBEBLOCK()

#define BEFORE_ORIGINAL_NOFILTER(FUNCNAME, LIBNAME)	\
	DECLARE_VARIABLE_STANDARD;						\
	GET_REAL_FUNC(FUNCNAME, LIBNAME);				\
	bfiltering = false;								\
	PRE_PROBEBLOCK()

#ifdef __cplusplus
}
#endif

#endif
