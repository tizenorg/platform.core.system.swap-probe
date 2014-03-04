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
 * - S-Core Co., Ltd
 *
 */

#include <unistd.h>
#include "common_probe_init.h"



//#define EGL_TEST

void dummy()
{
	return;
}

void probe_terminate_with_err(const char *msg, const char *func_name,
			      ORIGINAL_LIBRARY id)
{
	char error_msg[1024];

	sprintf(error_msg, "%s : [%s], %s", msg, func_name, lib_string[id]);
	perror(error_msg);
	PRINTERR(error_msg);
	//wait for flush
	sleep(1);
	exit(0);

}

////////////////////////////////////////////////////////////////////////////
//egl init probe function
//  params:
//    func_name    - original function name for dlsym search
//    func_pointer - original function pointer (return)
//
//  info
//    search original function by name
//    function have no return becouse on error it terminates main application
#ifdef EGL_TEST
void init_probe_egl(__attribute__ ((unused))const char *func_name, void **func_pointer,
		    __attribute__ ((unused))ORIGINAL_LIBRARY id)
{
	PRINTMSG(func_name);
	*func_pointer = (void *)dummy;
}
#else
void init_probe_egl(const char *func_name, void **func_pointer,
		    ORIGINAL_LIBRARY id)
{
	void *faddr = 0;

	(gProbeBlockCount++);
	if (lib_handle[id] == ((void *)0)) {
		lib_handle[id] = dlopen(lib_string[id],
					RTLD_LAZY | RTLD_GLOBAL);

		if (lib_handle[id] == ((void *)0))
			probe_terminate_with_err("dlopen failed", func_name, id);
	};
	faddr = dlsym(lib_handle[id], func_name);
	if (faddr == __null || dlerror() != __null)
		probe_terminate_with_err("dlsym failed", func_name, id);
	memcpy(func_pointer, &faddr, sizeof(faddr));
	(gProbeBlockCount--);
}
#endif
