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
bool printLog(log_t* log, int msgType);
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

// ========================== initialize variable ==========================

#define INIT_LOG			\
		log.type = 0;		\
		log.length = 0;		\
		log.data[0] = '\0'

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

// ========================== make log macro ================================
#define APPEND_LOG_BASIC(LCTYPE)								\
	log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",	\
			LCTYPE, probeInfo.eventIndex, __func__,				\
			probeInfo.currentTime, probeInfo.pID, probeInfo.tID)

#define APPEND_LOG_BASIC_NAME(LCTYPE, APINAME)					\
	log.length = sprintf(log.data, "%d`,%d`,%s`,%lu`,%d`,%d",	\
		LCTYPE, probeInfo.eventIndex, APINAME,					\
		probeInfo.currentTime, probeInfo.pID, probeInfo.tID)

#define APPEND_LOG_COMMON_NONE(CALLER)								\
	log.length += sprintf(log.data + log.length, "`,`,`,`,`,`,%u",	\
		(unsigned int)CALLER)

#define APPEND_LOG_INPUT(INPUTFORMAT, ...)								\
	log.length += sprintf(log.data + log.length,						\
			DEFAULT_TOKEN INPUTFORMAT, __VA_ARGS__)

#define APPEND_LOG_RESULT(RETTYPE, RETVALUE)							\
	__appendTypeLog(&log, 4, NULL, RETTYPE, RETVALUE, VT_INT, 0,		\
			VT_INT, newerrno, VT_INT, blockresult)

#define APPEND_LOG_CALLER()												\
	log.length += sprintf(log.data + log.length,						\
			"`,%u",(unsigned int) CALLER_ADDRESS)

#if defined(USING_BACKTRACE) || defined(PRINT_BACKTRACE_CALLSTACK)
#define APPEND_LOG_CALLSTACK()											\
	log.length += sprintf(log.data + log.length,						\
			"`,\ncallstack_start`,");									\
	getBacktraceString(&log, DA_LOG_MAX - log.length - 17);				\
	log.length += sprintf(log.data + log.length, "`,callstack_end")
#else
#define APPEND_LOG_CALLSTACK()	APPEND_LOG_NULL_CALLSTACK()
#endif

#define APPEND_LOG_NULL_CALLSTACK()										\
	log.length += sprintf(log.data + log.length,						\
			"`,\ncallstack_start`,`,callstack_end")


// =========================== post block macro ===========================

#define POST_PROBEBLOCK_BEGIN(LCTYPE, RETTYPE, RETVALUE, INPUTFORMAT, ...)	\
	newerrno = errno;														\
	if(postBlockBegin(blockresult)) {										\
		INIT_LOG;															\
		APPEND_LOG_BASIC(LCTYPE);											\
		APPEND_LOG_INPUT(INPUTFORMAT, __VA_ARGS__);							\
		APPEND_LOG_RESULT(RETTYPE, RETVALUE);								\
		APPEND_LOG_CALLER()

#define POST_PROBEBLOCK_CALLSTACK()			\
	do {									\
		if(newerrno != 0) {					\
			APPEND_LOG_CALLSTACK();			\
		} else {							\
			APPEND_LOG_NULL_CALLSTACK();	\
		}									\
	} while(0)


#define POST_PROBEBLOCK_END() 						\
		printLog(&log, MSG_LOG);					\
		postBlockEnd();								\
	}												\
	errno = (newerrno != 0) ? newerrno : olderrno

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
