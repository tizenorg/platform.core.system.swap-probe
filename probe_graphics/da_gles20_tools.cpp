/*
 *  DA probe
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:Yurchenko Darya<d.urchenko@partner.samsung.com>
 *
 *
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

#include "da_gles20.h"
#include "common_probe_init.h"


void __get_context_buf_data(GLenum target, char *buf, int buf_size)
{
	GLint n_buffer_size, n_buffer_usage_size;
	int print_size;

	if (buf == NULL)
		return;

	real_glGetBufferParameteriv(target, GL_BUFFER_SIZE,
				    &n_buffer_size);
	real_glGetBufferParameteriv(target, GL_BUFFER_USAGE,
				    &n_buffer_usage_size);

	print_size = snprintf(buf, buf_size, "%u,%u,%u",
			      target, n_buffer_size, n_buffer_usage_size);

	if (print_size >= buf_size) {
		/* data was truncated. so data is invalid */
		buf[0]='\0';
	}
}

