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

#include <app.h>
#include <FApp.h>
#include "dahelper.h"
using namespace Tizen::App;

#define UNKNOWN_USER_EVENT     5873

#ifdef __cplusplus
extern "C"{
#endif

void application_exit()
{
	App* self = App::GetInstance();
	if(self != NULL)
	{
		PRINTMSG("EXIT self");
		self->Terminate();
		// send unknown user event to main thread
		// because this cause that main thread execute terminating routine without block
		self->SendUserEvent(UNKNOWN_USER_EVENT, NULL);
	}
	else
	{
		PRINTMSG("EXIT self - efl_exit >");
		app_efl_exit();
		if (app_efl_main_flg == 0) {
			PRINTMSG("no app_efl_main detected, terminate by exit(0)");
			//sleep for flush message to manager
			sleep(1);
			exit(0);
		}
		PRINTMSG("EXIT self - efl_exit <");
	}
}

#ifdef __cplusplus
}
#endif

