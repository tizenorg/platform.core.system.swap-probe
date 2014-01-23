/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Vitaliy Cherepanov <v.cherepanov@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */
#include <sys/types.h>
#include <signal.h>

#include "daprobe.h"
#include "dahelper.h"
#include "binproto.h"
#include "daforkexec.h"

DECLARE(int, execl , const char *path, const char *arg, ...);
DECLARE(int, execlp, const char *file, const char *arg, ...);
DECLARE(int, execle, const char *path, const char *arg, ...);//, char * const envp[]);
DECLARE(int, execv, const char *path, char *const argv[]);
DECLARE(int, execve, const char *filename, char *const argv[],char *const envp[]);
DECLARE(int, execvp, const char *file, char *const argv[]);
DECLARE(int, execvpe, const char *file, char *const argv[],char *const envp[]);
DECLARE(pid_t, fork, void);

void init_exec_fork()
{
	INIT_FUNC_EXEC(fork);
	INIT_FUNC_EXEC(execl);
	INIT_FUNC_EXEC(execlp);
	INIT_FUNC_EXEC(execle);
	INIT_FUNC_EXEC(execv);
	INIT_FUNC_EXEC(execve);
	INIT_FUNC_EXEC(execvp);
	INIT_FUNC_EXEC(execvpe);
}

#define prepare_params( FUNCTION , p1, p2)		\
	va_list ap;					\
	int args_count = 0;				\
	int i;						\
							\
	INIT_FUNC_EXEC( FUNCTION );			\
							\
	va_start(ap, p2);				\
	do						\
		args_count++;				\
	while (va_arg(ap, char *));			\
	va_end(ap);					\
	args_count += 2;				\
							\
	char *arg_arr[args_count];			\
							\
	va_start(ap, p2);				\
							\
	arg_arr[0] = (char *)p1;			\
	arg_arr[1] = (char *)p2;			\
	for (i = 2; i < args_count; i++)		\
		arg_arr[i] = va_arg(ap, char *);	\
							\
	va_end(ap)

int _da_call_original(void *funcp, char *args[], int args_count);

#define print_params(buf, p1, p2) \
	char *p = buf;\
	char *pp;\
	va_list par;\
	p += sprintf(p, "--> %s: <", __FUNCTION__);\
	p += sprintf(p, " \"%s\",", p1);\
	p += sprintf(p, " \"%s\",", p2);\
	va_start(par, arg);\
	while ( (pp = va_arg(par, char *)) != NULL)\
		p += sprintf(p, " \"%s\",", pp);\
	va_end(par);\
	p += sprintf(p, ">");

int execl(const char *path, const char *arg, ...)
{
	int res;
	char buf[DA_LOG_MAX];

	print_params(buf, path, arg);
	PRINTMSG(buf);

	_uninit_();
	prepare_params(execl, path, arg);
	res = _da_call_original(&execl_p, arg_arr, args_count);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execlp(const char *file, const char *arg, ...)
{
	int res;
	char buf[DA_LOG_MAX];

	print_params(buf, file, arg);
	PRINTMSG(buf);

	_uninit_();
	prepare_params(execlp, file, arg);
	res = _da_call_original(&execlp_p, arg_arr, args_count);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execle(const char *path, const char *arg, ...
	/* original func have one more argument but
	 * i can't leave it in code by compilation reasons
	 * so it is commented:
	 *
	 *,__attribute__((unused)) char * const envp[] */
	)
{
	int res;
	char buf[DA_LOG_MAX];

	print_params(buf, path, arg);
	PRINTMSG(buf);

	_uninit_();
	prepare_params(execle, path, arg);
	res = _da_call_original(&execle_p, arg_arr, args_count);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execv(const char *path, char *const argv[])
{
	int res;
	char buf[DA_LOG_MAX];

	PRINTMSG("execv");

	_uninit_();
	res = execv_p(path, argv);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execvp(const char *file, char *const argv[])
{
	int res;
	char buf[DA_LOG_MAX];

	PRINTMSG("execvp");

	_uninit_();
	res = execvp_p(file, argv);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execve(const char *filename, char *const argv[],char *const envp[])
{
	int res;
	char buf[DA_LOG_MAX];

	PRINTMSG("execve");

	_uninit_();
	res =  execve_p(filename, argv, envp);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

int execvpe(const char *file, char *const argv[],char *const envp[])
{
	int res;
	char buf[DA_LOG_MAX];

	PRINTMSG("execvpe");

	_uninit_();
	res =  execvpe_p(file, argv, envp);
	_init_();

	sprintf(buf,  "%s return %d", __FUNCTION__, res);
	PRINTMSG(buf);
	return res;
}

pid_t fork(void)
{
	char msg[DA_LOG_MAX];
	pid_t res = fork_p();

	sprintf(msg, "<fork = %d>", res);
	if (res == 0) {
		if (gTraceInfo.socket.daemonSock >= 0) {
			close(gTraceInfo.socket.daemonSock);
			_init_();
		}
	}
	PRINTMSG(msg);

	return res;
}
