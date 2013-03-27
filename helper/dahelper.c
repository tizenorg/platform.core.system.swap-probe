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

#include <wchar.h>			// for wcslen
#include <unistd.h>			// for unlink
#include <sys/types.h>
#include <dirent.h>			// for opendir, readdir
#include <sys/shm.h>		// for shared memory operation

#include "dahelper.h"

#define SHAREDMEMKEY			((key_t)463825)

typedef struct
{
	long long allocsize;
	long launch_flag;
} __daSharedInfo;

typedef struct
{
	int memid;				// shared mem id
	__daSharedInfo* pvalue;		// total alloced heap size
} __sharedMemInfo;

static __sharedMemInfo sharedmem =
{
	-1,			// memid
	(void*)-1	// pvalue
};

long long total_alloc_size = 0;

/* trace info global variable */
__traceInfo gTraceInfo =
{
	{
		1,							// int eventIndex
		PTHREAD_MUTEX_INITIALIZER	// pthread_mutex_t eventMutex
	},		// __indexInfo
	{
		-1,							// int daemonSock
		PTHREAD_MUTEX_INITIALIZER	// ptrhread_mutex_t sockMutex
	},		// __socketInfo
	{
		{0, },						// char appName[128]
		0							// unsigned int startTime
	},		// __appInfo
	{
		0,							// int state
		PTHREAD_MUTEX_INITIALIZER	// ptrhread_mutex_t ssMutex
	},		// __screenshotInfo
	{
		NULL,						// map_start
		NULL						// map_end
	},		// __mapInfo
	-1,								// int stateTouch
	0,								// int init_complete
	0,								// int custom_chart_callback_count
	NULL							// long* pprobeflag
};

__thread unsigned long	gSTrace = 0;

// **************************************************************************************
// shared memory management function
// **************************************************************************************

// return 0 for normal case
// return -1 for error case
int __atSharedMemory()
{
	sharedmem.memid = shmget(SHAREDMEMKEY, sizeof(__daSharedInfo), 0);
	if(sharedmem.memid == -1)
	{
		return -1;
	}
	else
	{
		sharedmem.pvalue = (__daSharedInfo*)shmat(sharedmem.memid, NULL, 0);
		if(sharedmem.pvalue == (void*)-1)
		{
			return -1;
		}
		else
		{
			sharedmem.pvalue->allocsize = total_alloc_size;
			gTraceInfo.pprobeflag = &(sharedmem.pvalue->launch_flag);
			return 0;
		}
	}
}

int __dtSharedMemory()
{
	int ret = 0;
	if(sharedmem.pvalue != (void*)-1)
	{
		ret = shmdt(sharedmem.pvalue);
		if(ret == 0)
			sharedmem.pvalue = (void*)-1;
	}
	gTraceInfo.pprobeflag = NULL;
	return ret;
}

// update heap memory size into shared memory
// return 0 if size is updated into shared memory
// return 1 if size is updated into global variable
int update_heap_memory_size(bool isAdd, size_t size)
{
	if(likely(sharedmem.pvalue != (void*)-1))
	{
		if(isAdd) sharedmem.pvalue->allocsize += (long)size;
		else sharedmem.pvalue->allocsize -= (long)size;
		return 0;
	}
	else
	{
		if(isAdd) total_alloc_size += (long)size;
		else total_alloc_size -= (long)size;
		return 1;
	}
}

void WcharToChar(char* pstrDest, const wchar_t* pwstrSrc)
{
	int nLen=(int)wcslen(pwstrSrc);
	wcstombs(pstrDest, pwstrSrc, nLen+1);
}

// return 0 if succeed
// return -1 if error occured
int remove_indir(const char *dirname)
{
	DIR *dir;
	struct dirent *entry;
	char path[MAX_PATH_LENGTH];

	dir = opendir(dirname);
	if(dir == NULL)
	{
		return -1;
	}

	while((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
		{
			snprintf(path, (size_t) MAX_PATH_LENGTH, "%s/%s", dirname, entry->d_name);
			if (entry->d_type != DT_DIR)	// file
			{
				unlink(path);
			}
			else { }	// directory
		}
	}
	closedir(dir);

	return 0;
}


