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
