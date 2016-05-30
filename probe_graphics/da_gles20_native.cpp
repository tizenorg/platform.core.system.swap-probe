/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Sanghyun Lee <sanghyunnim.lee@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
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
 * - Samsung RnD Institute Russia
 *
 */
#include "common_probe_init.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "da_gles20.h"
#include "binproto.h"

#define DECLARE FUNC_DECLAR
#define DECLARE_NOARGS FUNC_DECLAR_NOARGS

#define REAL_NAME(func) PROBE_NAME(func)
#define BEFORE BEFORE_GL2_ORIG
#define CALL_ORIG(func, ...) func##p(__VA_ARGS__)
#define TYPEDEF(type) typedef type


#include "da_gles20.inc"

#ifdef __cplusplus
}
#endif




#undef CALL_ORIG
#undef BEFORE
