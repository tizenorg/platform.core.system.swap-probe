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

#include <dlfcn.h>
#include "daprobe.h"
#include "dahelper.h"

int get_map_address(void* sym, void** start, void** end);

int getExecutableMappingAddress()
{
	int ret = 0;
	void* main_symbol;

	probeBlockStart();
	main_symbol = dlsym(RTLD_NEXT, "main");
	if(main_symbol != NULL)
	{
		ret = get_map_address(main_symbol, &(gTraceInfo.exec_map.map_start),
						&(gTraceInfo.exec_map.map_end));
	}
	else
	{
		// failed to get address of main
		ret = 0;
	}
	probeBlockEnd();

	return ret;
}

