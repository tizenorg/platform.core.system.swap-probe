/*
 *  DA manager
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Cherepanov Vitaliy <v.cherepanov@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contributors:
 * - S-Core Co., Ltd
 * - Samsung RnD Institute Russia
 *
 */
//#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include "dahelper.h"

static inline int __redirect(int old, int new)
{
	int ret = 0;

	if (dup2(old, new) == -1) {
		/* fail to print up there. there is no sdterr */
		/* TODO inform about problem */
		fprintf(stderr, "dup2 fail\n");
		ret = -1;
	}

	return ret;
}

int __redirect_std(void)
{
#ifdef STDTOFILE
	char STDOUT[MAX_PATH_LENGTH];
	snprintf(STDOUT,sizeof(STDOUT), "/tmp/da_preloaded_%d_%d.log", getppid(), getpid());
#else
	#define STDOUT		"/dev/null"
#endif

	int ret = 0;
	int fd = open(STDOUT, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd != -1) {
		if (__redirect(fd, 1) != 0 ||
		    __redirect(fd, 2) != 0) {
			/* fail to print up there. there is no sdterr */
			/* TODO inform about problem */
			/* fprintf(sdterr, "duplicate fd fail\n"); */
			ret = -1;
		}

		close(fd);
	} else {
		/* TODO inform about problem */
		close(1);
		close(2);
	}

	close(0);
	return ret;
}
