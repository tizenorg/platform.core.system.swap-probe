/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
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
#include <assert.h>
#include <errno.h>
#include "dahelper.h"

const char *lib_string[NUM_ORIGINAL_LIBRARY] = {
	"libc.so.6",				//0
	"libpthread.so.0",			//1
	"libelementary.so.1",			//2
	"libecore_input_evas.so.1",		//3
	"libdaemon.so.0",			//4
	"libcapi-appfw-application.so.0",	//5
	"libGLESv2.so",				//6
	"libEGL.so",				//7
	SELF_LIB_NAME,				//8
	"libevas.so.1"				//9
};
void *lib_handle[NUM_ORIGINAL_LIBRARY];

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
	},		// __appInfo
	{
		0,							// int state
		PTHREAD_MUTEX_INITIALIZER	// ptrhread_mutex_t ssMutex
	},		// __screenshotInfo
	-1,								// int stateTouch
	0,								// int init_complete
	0,								// int custom_chart_callback_count
	0,								// unsigned long optionflag
	{
		PTHREAD_MUTEX_INITIALIZER,	// pthread_mutex_t bins_mutex
		SLIST_HEAD_INITIALIZER(bins_list) // struct head = NULL
	}		// bins_info_t
};

// return 0 if succeed
// return -1 if error occured
int remove_indir(const char *dirname)
{
	DIR *dir;
	struct dirent *entry;
	char path[MAX_PATH_LENGTH];
	static char dirent_buffer[ sizeof(struct dirent) + PATH_MAX + 1 ] = {0,};
	static struct dirent *dirent_r = (struct dirent *)dirent_buffer;


	dir = opendir(dirname);
	if(dir == NULL)
	{
		return -1;
	}

	while ((readdir_r(dir, dirent_r, &entry) == 0) && entry) {
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

static int absolute_filepath_p(const char *fname)
{
	return fname[0] == '/';
}

char *absolutize_filepath(const char *fname, char *buffer, size_t bufsiz)
{
	char cwd[PATH_MAX];

	assert(fname && "Filename, passed to stdc function is NULL.");
	if (absolute_filepath_p(fname) || getcwd(cwd, sizeof(cwd)) == NULL)
		snprintf(buffer, bufsiz, "%s", fname);
	else
		snprintf(buffer, bufsiz, "%s/%s", cwd, fname);
	return buffer;
}

char *real_abs_path(int fd, char *buffer, size_t bufsiz)
{
	static const char *PROC_FD = "/proc/self/fd/%d";
	char proc_path[sizeof(PROC_FD) + 16]; /* PATH_MAX not needed here */
	ssize_t ret = 0;

	if (fd < 0)
		return NULL;

	snprintf(proc_path, sizeof(proc_path), PROC_FD, fd);

	ret = readlink(proc_path, buffer, bufsiz);
	if (ret < 0) /* some error occured */
		return NULL;
	buffer[ret] = '\0';

	return buffer;
}

void swap_usleep(useconds_t usec)
{
	struct timespec req;
	struct timespec rem;
	req.tv_sec = usec / 1000000;
	req.tv_nsec = (usec % 1000000) * 1000;
	if (nanosleep(&req, &rem) == -1) {
		PRINTWRN("sleep was terminated by signal\n");
	}
}


/*****************************************************************************
 *                                  BINARIES LIST                            *
 *****************************************************************************/

static struct bin_info_t *_find_binary_no_lock(const char *path)
{
	struct bin_info_t *bin_info;

	SLIST_FOREACH(bin_info, &gTraceInfo.bins_info.bins_list, list) {
		if (strncmp(bin_info->path, path, PATH_MAX) == 0)
			return bin_info;
	}

	return NULL;
}


/* Returns 0 on success, -errcode on fail */
int add_binary(char *path)
{
	struct bin_info_t *bin_info;
	int ret = 0;
	char *real_path;

	real_path = realpath(path, NULL);
	if (real_path == NULL) {
		PRINTERR("Cannot resolve real path for <%s> due to <%s>\n",
			 strerror(errno));
		return -EINVAL;
	}

	pthread_mutex_lock(&gTraceInfo.bins_info.bins_mutex);

	if (!SLIST_EMPTY(&gTraceInfo.bins_info.bins_list) &&
		(_find_binary_no_lock(real_path) != NULL)) {
		ret = -EALREADY;
		goto add_bin_unlock;
	}

	bin_info = malloc(sizeof(*bin_info));
	if (bin_info == NULL) {
		ret = -ENOMEM;
		goto add_bin_unlock;
	}

	// TODO Slow copy / cleanup of memory allocated somewhere else?
	bin_info->path = real_path;
	SLIST_INSERT_HEAD(&gTraceInfo.bins_info.bins_list, bin_info, list);

add_bin_unlock:
	pthread_mutex_unlock(&gTraceInfo.bins_info.bins_mutex);

	return ret;
}

/* Returns 0 on success, -errcode on fail */
int remove_binary(char *path)
{
	struct bin_info_t *bin_info;
	int ret = 0;
	char *real_path;

	real_path = realpath(path, NULL);
	if (real_path == NULL) {
		PRINTERR("Cannot resolve real path for <%s> due to <%s>\n",
			 strerror(errno));
		return -EINVAL;
	}

	pthread_mutex_lock(&gTraceInfo.bins_info.bins_mutex);

	if (SLIST_EMPTY(&gTraceInfo.bins_info.bins_list)) {
		ret = -EINVAL;
		goto remove_bin_unlock;
	}

	bin_info = _find_binary_no_lock(real_path);
	if (bin_info == NULL) {
		ret = -EINVAL;
		goto remove_bin_unlock;
	}

	SLIST_REMOVE(&gTraceInfo.bins_info.bins_list, bin_info, bin_info_t, list);

remove_bin_unlock:
	pthread_mutex_unlock(&gTraceInfo.bins_info.bins_mutex);

	return ret;
}

/* Checks if binary is a target one */
bool check_binary(const char *path)
{
	struct bin_info_t *bin_info;
	bool ret = false;
	char orig_path[PATH_MAX + 1] = {0};
	char *real_path;

	real_path = realpath(path, orig_path);
	if (real_path == NULL) {
		PRINTERR("No real path for <%s> path!\n", path);
		return ret;
	}

	pthread_mutex_lock(&gTraceInfo.bins_info.bins_mutex);

	if (SLIST_EMPTY(&gTraceInfo.bins_info.bins_list))
		goto check_bin_unlock;

	bin_info = _find_binary_no_lock(real_path);
	if (bin_info != NULL)
		ret = true;

check_bin_unlock:
	pthread_mutex_unlock(&gTraceInfo.bins_info.bins_mutex);

	return ret;
}

void cleanup_binaries(void)
{
	struct bin_info_t *bin_info, *tmp;

	pthread_mutex_lock(&gTraceInfo.bins_info.bins_mutex);

	if (SLIST_EMPTY(&gTraceInfo.bins_info.bins_list))
		goto cleanup_unlock;

	/* There is no FOREACH_SAFE in queue.h */
	for (bin_info = SLIST_FIRST(&gTraceInfo.bins_info.bins_list);
	     bin_info;
	     bin_info = tmp) {

		tmp = SLIST_NEXT(bin_info, list);

		SLIST_REMOVE_HEAD(&gTraceInfo.bins_info.bins_list, list);
		if (bin_info->path != NULL)
			free(bin_info->path);

		free(bin_info);
	}

cleanup_unlock:
	pthread_mutex_unlock(&gTraceInfo.bins_info.bins_mutex);
}
