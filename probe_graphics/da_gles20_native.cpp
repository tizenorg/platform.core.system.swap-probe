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

#include "da_gles20.h"
#include "daprobe.h"
#include "binproto.h"
#include "common_probe_init.h"

#ifndef CALL_ORIG
	#define CALL_ORIG(func, ...) func##p(__VA_ARGS__)
#endif

#ifndef BEFORE
	#define BEFORE BEFORE_GL_ORIG
#endif

static char contextValue[MAX_GL_CONTEXT_VALUE_SIZE];
static enum DaOptions _sopt = OPT_GLES;
static __thread GLenum gl_error_external = GL_NO_ERROR;

GLenum glGetError(void)
{
#undef glGetError
	typedef GLenum (*methodType)(void);
	BEFORE(glGetError);
	GLenum ret = CALL_ORIG(glGetError,);

	if (gl_error_external == GL_NO_ERROR)
		gl_error_external = ret;

	if (blockresult) {
		//external call
		ret = gl_error_external;
		gl_error_external = GL_NO_ERROR;
	}

	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, "");

	return ret;
}

// ==================================================================
// A 2
// ==================================================================

void glActiveTexture(GLenum texture)
{
#undef glActiveTexture
	typedef void (*methodType)(GLenum);
	BEFORE(glActiveTexture);
	CALL_ORIG(glActiveTexture, texture);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(texture));
}

void glAttachShader(GLuint program, GLuint shader)
{
#undef glAttachShader
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glAttachShader);
	CALL_ORIG(glAttachShader, program, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", program, shader);
}

// ==================================================================
// B 12
// ==================================================================

void glBindAttribLocation(GLuint program, GLuint index, const char *name)
{
#undef glBindAttribLocation
	typedef void (*methodType)(GLuint, GLuint, const char *);
	BEFORE(glBindAttribLocation);
	CALL_ORIG(glBindAttribLocation, program, index, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dds",
	      program, index, name);
}

void glBindBuffer(GLenum target, GLuint buffer)
{
#undef glBindBuffer
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindBuffer);
	CALL_ORIG(glBindBuffer, target, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), buffer);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
#undef glBindFramebuffer
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindFramebuffer);
	CALL_ORIG(glBindFramebuffer, target, framebuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), framebuffer);
}

void glBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
#undef glBindRenderbuffer
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindRenderbuffer);
	CALL_ORIG(glBindRenderbuffer, target, renderbuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), renderbuffer);
}

void glBindTexture(GLenum target, GLuint texture)
{
#undef glBindTexture
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindTexture);
	CALL_ORIG(glBindTexture, target, texture);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), texture);
}

void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
#undef glBlendColor
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glBlendColor);
	CALL_ORIG(glBlendColor, red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
	      red, green, blue, alpha);
}

void glBlendEquation(GLenum mode)
{
#undef glBlendEquation
	typedef void (*methodType)(GLenum);
	BEFORE(glBlendEquation);
	CALL_ORIG(glBlendEquation, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mode));
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
#undef glBlendEquationSeparate
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendEquationSeparate);
	CALL_ORIG(glBlendEquationSeparate, modeRGB, modeAlpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(modeRGB), (uint64_t)(modeAlpha));
}

void glBlendFunc(GLenum sfactor, GLenum dfactor)
{
#undef glBlendFunc
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendFunc);
	CALL_ORIG(glBlendFunc, sfactor, dfactor);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(sfactor), (uint64_t)(dfactor));
}

void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
			 GLenum dstAlpha)
{
#undef glBlendFuncSeparate
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glBlendFuncSeparate);
	CALL_ORIG(glBlendFuncSeparate, srcRGB, dstRGB, srcAlpha, dstAlpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
	      (uint64_t)(srcRGB), (uint64_t)(dstRGB),
	      (uint64_t)(srcAlpha), (uint64_t)(dstAlpha));
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data,
		  GLenum usage)
{
#undef glBufferData
	typedef void (*methodType)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
	BEFORE(glBufferData);
	CALL_ORIG(glBufferData, target, size, data, usage);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxpx",
	      (uint64_t)(target), (uint64_t)(size),
	      voidp_to_uint64(data), (uint64_t)(usage));
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
		     const GLvoid * data)
{
#undef glBufferSubData
	typedef void (*methodType)(GLenum, GLintptr, GLsizeiptr,
				   const GLvoid *);
	BEFORE(glBufferSubData);
	CALL_ORIG(glBufferSubData, target, offset, size, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxp",
	      (uint64_t)(target), (uint64_t)(offset),
	      (uint64_t)(size), voidp_to_uint64(data));
}

// ==================================================================
// C 14
// ==================================================================

GLenum glCheckFramebufferStatus(GLenum target)
{
#undef glCheckFramebufferStatus
	typedef GLenum (*methodType)(GLenum);
	BEFORE(glCheckFramebufferStatus);
	GLenum ret = CALL_ORIG(glCheckFramebufferStatus, target);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(target));

	return ret;
}

void glClear(GLbitfield mask)
{
#undef glClear
	typedef void (*methodType)(GLbitfield);
	BEFORE(glClear);
	CALL_ORIG(glClear, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mask));
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
#undef glClearColor
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glClearColor);
	CALL_ORIG(glClearColor, red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
	      red, green, blue, alpha);
}

void glClearDepthf(GLclampf depth)
{
#undef glClearDepthf
	typedef void (*methodType)(GLclampf);
	BEFORE(glClearDepthf);
	CALL_ORIG(glClearDepthf, depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", depth);
}

void glClearStencil(GLint s)
{
#undef glClearStencil
	typedef void (*methodType)(GLint);
	BEFORE(glClearStencil);
	CALL_ORIG(glClearStencil, s);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", s);
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue,
		 GLboolean alpha)
{
#undef glColorMask
	typedef void (*methodType)(GLboolean, GLboolean, GLboolean, GLboolean);
	BEFORE(glColorMask);
	CALL_ORIG(glColorMask, red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
	      red, green, blue, alpha);
}

void glCompileShader(GLuint shader)
{
#undef glCompileShader
	typedef void (*methodType)(GLuint);
	BEFORE(glCompileShader);
	CALL_ORIG(glCompileShader, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(shader));
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
			    GLsizei width, GLsizei height, GLint border,
			    GLsizei imageSize, const GLvoid * data)
{
#undef glCompressedTexImage2D
	typedef void (*methodType)(GLenum, GLint, GLenum, GLsizei, GLsizei,
				   GLint, GLsizei, const GLvoid *);
	BEFORE(glCompressedTexImage2D);
	CALL_ORIG(glCompressedTexImage2D, target, level, internalformat, width,
		  height, border, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxddddp",
	      (uint64_t)(target), level,
	      (uint64_t)(internalformat), width, height, border, imageSize,
	      voidp_to_uint64(data));
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
			       GLint yoffset, GLsizei width, GLsizei height,
			       GLenum format, GLsizei imageSize,
			       const GLvoid * data)
{
#undef glCompressedTexSubImage2D
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei,
				   GLsizei, GLenum, GLsizei, const GLvoid *);
	BEFORE(glCompressedTexSubImage2D);
	CALL_ORIG(glCompressedTexSubImage2D, target, level, xoffset, yoffset,
		  width, height, format, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddxdp",
	      (uint64_t)(target), level, xoffset, yoffset, width, height,
	      (uint64_t)(format), imageSize, voidp_to_uint64(data));
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
		      GLint x, GLint y, GLsizei width, GLsizei height,
		      GLint border)
{
#undef glCopyTexImage2D
	typedef void (*methodType)(GLenum, GLint, GLenum, GLint, GLint,
				   GLsizei, GLsizei, GLint);
	BEFORE(glCopyTexImage2D);
	CALL_ORIG(glCopyTexImage2D, target, level, internalformat, x, y, width,
		  height, border);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxddddd",
	      (uint64_t)(target), level,
	      (uint64_t)(internalformat), x, y, width, height, border);
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
			 GLint yoffset, GLint x, GLint y, GLsizei width,
			 GLsizei height)
{
#undef glCopyTexSubImage2D
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLint, GLint,
				   GLsizei, GLsizei);
	BEFORE(glCopyTexSubImage2D);
	CALL_ORIG(glCopyTexSubImage2D, target, level, xoffset, yoffset, x, y,
		  width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddddddd",
	      (uint64_t)(target), level, xoffset, yoffset, x, y, width,
	      height);
}

GLuint glCreateProgram(void)
{
#undef glCreateProgram
	typedef GLuint (*methodType)(void);
	BEFORE(glCreateProgram);
	GLuint ret = CALL_ORIG(glCreateProgram,);
	GL_GET_ERROR();
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, "");

	return ret;
}

GLuint glCreateShader(GLenum shaderType)
{
#undef glCreateShader
	typedef GLuint (*methodType)(GLenum);
	BEFORE(glCreateShader);
	GLuint ret = CALL_ORIG(glCreateShader, shaderType);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(shaderType));

	return ret;
}

void glCullFace(GLenum mode)
{
#undef glCullFace
	typedef void (*methodType)(GLenum);
	BEFORE(glCullFace);
	CALL_ORIG(glCullFace, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mode));
}

// ==================================================================
// D 14
// ==================================================================

void glDeleteBuffers(GLsizei n, const GLuint * buffers)
{
#undef glDeleteBuffers
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteBuffers);
	CALL_ORIG(glDeleteBuffers, n, buffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(buffers));
}

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
{
#undef glDeleteFramebuffers
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteFramebuffers);
	CALL_ORIG(glDeleteFramebuffers, n, framebuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(framebuffers));
}

void glDeleteProgram(GLuint program)
{
#undef glDeleteProgram
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteProgram);
	CALL_ORIG(glDeleteProgram, program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", program);
}

void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers)
{
#undef glDeleteRenderbuffers
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteRenderbuffers);
	CALL_ORIG(glDeleteRenderbuffers, n, renderbuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(renderbuffers));
}

void glDeleteShader(GLuint shader)
{
#undef glDeleteShader
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteShader);
	CALL_ORIG(glDeleteShader, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", shader);
}

void glDeleteTextures(GLsizei n, const GLuint * textures)
{
#undef glDeleteTextures
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteTextures);
	CALL_ORIG(glDeleteTextures, n, textures);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		int i = 0;
		for (i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", textures[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
		      n, voidp_to_uint64(textures));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
		      n, voidp_to_uint64(textures));
	}
}

void glDepthFunc(GLenum func)
{
#undef glDepthFunc
	typedef void (*methodType)(GLenum);
	BEFORE(glDepthFunc);
	CALL_ORIG(glDepthFunc, func);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(func));
}

void glDepthMask(GLboolean flag)
{
#undef glDepthMask
	typedef void (*methodType)(GLboolean);
	BEFORE(glDepthMask);
	CALL_ORIG(glDepthMask, flag);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(flag));
}

void glDepthRangef(GLclampf nearVal, GLclampf farVal)
{
#undef glDepthRangef
	typedef void (*methodType)(GLclampf, GLclampf);
	BEFORE(glDepthRangef);
	CALL_ORIG(glDepthRangef, nearVal, farVal);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
	      nearVal, farVal);
}

void glDetachShader(GLuint program, GLuint shader)
{
#undef glDetachShader
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glDetachShader);
	CALL_ORIG(glDetachShader, program, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      program, shader);
}

void glDisable(GLenum cap)
{
#undef glDisable
	typedef void (*methodType)(GLenum);
	BEFORE(glDisable);
	CALL_ORIG(glDisable, cap);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));
}

void glDisableVertexAttribArray(GLuint index)
{
#undef glDisableVertexAttribArray
	typedef void (*methodType)(GLuint);
	BEFORE(glDisableVertexAttribArray);
	CALL_ORIG(glDisableVertexAttribArray, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
#undef glDrawArrays
	typedef void (*methodType)(GLenum, GLint, GLsizei);
	BEFORE(glDrawArrays);
	CALL_ORIG(glDrawArrays, mode, first, count);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
	      (uint64_t)(mode), first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type,
		const GLvoid *indices)
{
#undef glDrawElements
	typedef void (*methodType)(GLenum, GLsizei, GLenum, const GLvoid *);
	BEFORE(glDrawElements);
	CALL_ORIG(glDrawElements, mode, count, type, indices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxp",
	      (uint64_t)(mode), count, (uint64_t)(type), indices);
}

// ==================================================================
// E 2
// ==================================================================

void glEnable(GLenum cap)
{
#undef glEnable
	typedef void (*methodType)(GLenum);
	BEFORE(glEnable);
	CALL_ORIG(glEnable, cap);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));
}

void glEnableVertexAttribArray(GLuint index)
{
#undef glEnableVertexAttribArray
	typedef void (*methodType)(GLuint);
	BEFORE(glEnableVertexAttribArray);
	CALL_ORIG(glEnableVertexAttribArray, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

// ==================================================================
// F 5
// ==================================================================

void glFinish(void)
{
#undef glFinish
	typedef void (*methodType)(void);
	BEFORE(glFinish);
	CALL_ORIG(glFinish,);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFlush(void)
{
#undef glFlush
	typedef void (*methodType)(void);
	BEFORE(glFlush);
	CALL_ORIG(glFlush,);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment,
			       GLenum renderbuffertarget, GLuint renderbuffer)
{
#undef glFramebufferRenderbuffer
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint);
	BEFORE(glFramebufferRenderbuffer);
	CALL_ORIG(glFramebufferRenderbuffer, target, attachment,
		  renderbuffertarget, renderbuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxd",
	      (uint64_t)(target), (uint64_t)(attachment),
	      (uint64_t)(renderbuffertarget), renderbuffer);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
			    GLuint texture, GLint level)
{
#undef glFramebufferTexture2D
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint, GLint);
	BEFORE(glFramebufferTexture2D);
	CALL_ORIG(glFramebufferTexture2D, target, attachment, textarget,
		  texture, level);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxdd",
	      (uint64_t)(target), (uint64_t)(attachment),
	      (uint64_t)(textarget), texture, level);
}

void glFrontFace(GLenum mode)
{
#undef glFrontFace
	typedef void (*methodType)(GLenum);
	BEFORE(glFrontFace);
	CALL_ORIG(glFrontFace, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mode));
}

// ==================================================================
// G 31
// ==================================================================

void glGenBuffers(GLsizei n, GLuint * buffers)
{
#undef glGenBuffers
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenBuffers);
	CALL_ORIG(glGenBuffers, n, buffers);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		int i;
		for (i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", buffers[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
		      n, voidp_to_uint64(buffers));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
		      n, voidp_to_uint64(buffers));
	}
}

void glGenFramebuffers(GLsizei n, GLuint * framebuffers)
{
#undef glGenFramebuffers
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenFramebuffers);
	CALL_ORIG(glGenFramebuffers, n, framebuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(framebuffers));
}

void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers)
{
#undef glGenRenderbuffers
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenRenderbuffers);
	CALL_ORIG(glGenRenderbuffers, n, renderbuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(renderbuffers));
}

void glGenTextures(GLsizei n, GLuint * textures)
{
#undef glGenTextures
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenTextures);
	CALL_ORIG(glGenTextures, n, textures);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		int i;
		for (i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", textures[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
		      n, voidp_to_uint64(textures));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
		      n, voidp_to_uint64(textures));
	}
}

void glGenerateMipmap(GLenum target)
{
#undef glGenerateMipmap
	typedef void (*methodType)(GLenum);
	BEFORE(glGenerateMipmap);
	CALL_ORIG(glGenerateMipmap, target);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(target));
}

//lsh_get
void glGetBooleanv(GLenum pname, GLboolean * params)
{
#undef glGetBooleanv
	typedef void (*methodType)(GLenum, GLboolean *);
	BEFORE(glGetBooleanv);
	CALL_ORIG(glGetBooleanv, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(pname));
}

//lsh_get
void glGetFloatv(GLenum pname, GLfloat * params)
{
#undef glGetFloatv
	typedef void (*methodType)(GLenum, GLfloat *);
	BEFORE(glGetFloatv);
	CALL_ORIG(glGetFloatv, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(pname));
}

//lsh_get
void glGetIntegerv(GLenum pname, GLint * params)
{
#undef glGetIntegerv
	typedef void (*methodType)(GLenum, GLint *);
	BEFORE(glGetIntegerv);
	CALL_ORIG(glGetIntegerv, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(pname));
}

//lsh_get
void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize,
		       GLsizei *length, GLint *size, GLenum *type, char *name)
{
#undef glGetActiveAttrib
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
				   GLenum *, char *);
	BEFORE(glGetActiveAttrib);
	CALL_ORIG(glGetActiveAttrib, program, index, bufSize, length, size,
		  type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
	      program, index, bufSize);
}

//lsh_get
void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize,
			GLsizei *length, GLint *size, GLenum *type, char *name)
{
#undef glGetActiveUniform
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
				   GLenum *, char *);
	BEFORE(glGetActiveUniform);
	CALL_ORIG(glGetActiveUniform, program, index, bufSize, length, size,
		  type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
	      program, index, bufSize);
}

//lsh_get
void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count,
			  GLuint *shaders)
{
#undef glGetAttachedShaders
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, GLuint *);
	BEFORE(glGetAttachedShaders);
	CALL_ORIG(glGetAttachedShaders, program, maxCount, count, shaders);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      program, maxCount);
}

//lsh_get
int glGetAttribLocation(GLuint program, const char *name)
{
#undef glGetAttribLocation
	typedef int (*methodType)(GLuint, const char*);
	BEFORE(glGetAttribLocation);
	int ret = CALL_ORIG(glGetAttribLocation, program, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xs",
	      (uint64_t)(program), name);
	return ret;
}

//lsh_get
void glGetBufferParameteriv(GLenum target, GLenum value, GLint * data)
{
#undef glGetBufferParameteriv
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetBufferParameteriv);
	CALL_ORIG(glGetBufferParameteriv, target, value, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(value));
}

//lsh_get
void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment,
					   GLenum pname, GLint * params)
{
#undef glGetFramebufferAttachmentParameteriv
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLint *);
	BEFORE(glGetFramebufferAttachmentParameteriv);
	CALL_ORIG(glGetFramebufferAttachmentParameteriv, target, attachment,
		  pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(target), (uint64_t)(attachment), (uint64_t)(pname));
}

//lsh_get
void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length,
			 char *infoLog)
{
#undef glGetProgramInfoLog
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetProgramInfoLog);
	CALL_ORIG(glGetProgramInfoLog, program, maxLength, length, infoLog);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      program, maxLength);
}

//lsh_get
void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
#undef glGetProgramiv
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetProgramiv);
	CALL_ORIG(glGetProgramiv, program, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      program, (uint64_t)(pname));
}

//lsh_get
void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
{
#undef glGetRenderbufferParameteriv
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetRenderbufferParameteriv);
	CALL_ORIG(glGetRenderbufferParameteriv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length,
			char *infoLog)
{
#undef glGetShaderInfoLog
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderInfoLog);
	CALL_ORIG(glGetShaderInfoLog, shader, maxLength, length, infoLog);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      shader, maxLength);
}

//lsh_get
void glGetShaderPrecisionFormat(GLenum shaderType, GLenum precisionType,
				GLint *range, GLint *precision)
{
#undef glGetShaderPrecisionFormat
	typedef void (*methodType)(GLenum, GLenum, GLint *, GLint *);
	BEFORE(glGetShaderPrecisionFormat);
	CALL_ORIG(glGetShaderPrecisionFormat, shaderType, precisionType, range,
		  precision);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(shaderType), (uint64_t)(precisionType));
}

//lsh_get
void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length,
		       char *source)
{
#undef glGetShaderSource
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderSource);
	CALL_ORIG(glGetShaderSource, shader, bufSize, length, source);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      shader, bufSize);
}

//lsh_get
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
#undef glGetShaderiv
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetShaderiv);
	CALL_ORIG(glGetShaderiv, shader, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      shader, (uint64_t)(pname));
}

const GLubyte *glGetString(GLenum name)
{
#undef glGetString
	typedef const GLubyte *(*methodType)(GLenum);
	BEFORE(glGetString);
	const GLubyte *ret = CALL_ORIG(glGetString, name);
	GL_GET_ERROR();
	AFTER('p', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(name));

	return ret;
}

//lsh_get
void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params)
{
#undef glGetTexParameterfv
	typedef void (*methodType)(GLenum, GLenum, GLfloat *);
	BEFORE(glGetTexParameterfv);
	CALL_ORIG(glGetTexParameterfv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params)
{
#undef glGetTexParameteriv
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetTexParameteriv);
	CALL_ORIG(glGetTexParameteriv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetUniformfv(GLuint program, GLint location, GLfloat *params)
{
#undef glGetUniformfv
	typedef void (*methodType)(GLuint, GLuint, GLfloat *);
	BEFORE(glGetUniformfv);
	CALL_ORIG(glGetUniformfv, program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      program, location);
}

//lsh_get
void glGetUniformiv(GLuint program, GLint location, GLint *params)
{
#undef glGetUniformiv
	typedef void (*methodType)(GLuint, GLuint, GLint *);
	BEFORE(glGetUniformiv);
	CALL_ORIG(glGetUniformiv, program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      program, location);
}

//lsh_get
GLint glGetUniformLocation(GLuint program, const char *name)
{
#undef glGetUniformLocation
	typedef GLint (*methodType)(GLuint, const char *);
	BEFORE(glGetUniformLocation);
	GLint ret = CALL_ORIG(glGetUniformLocation, program, name);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

//lsh_get
void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
{
#undef glGetVertexAttribfv
	typedef void (*methodType)(GLuint, GLenum, GLfloat *);
	BEFORE(glGetVertexAttribfv);
	CALL_ORIG(glGetVertexAttribfv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

//lsh_get
void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
{
#undef glGetVertexAttribiv
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetVertexAttribiv);
	CALL_ORIG(glGetVertexAttribiv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

//lsh_get
void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer)
{
#undef glGetVertexAttribPointerv
	typedef void (*methodType)(GLuint, GLenum, GLvoid **);
	BEFORE(glGetVertexAttribPointerv);
	CALL_ORIG(glGetVertexAttribPointerv, index, pname, pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

// ==================================================================
// H 1
// ==================================================================

void glHint(GLenum target, GLenum mode)
{
#undef glHint
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glHint);
	CALL_ORIG(glHint, target, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(mode));
}

// ==================================================================
// I 7
// ==================================================================

GLboolean glIsBuffer(GLuint buffer)
{
#undef glIsBuffer
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsBuffer);
	GLboolean ret = CALL_ORIG(glIsBuffer, buffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", buffer);

	return ret;
}

GLboolean glIsEnabled(GLenum cap)
{
#undef glIsEnabled
	typedef GLboolean (*methodType)(GLenum);
	BEFORE(glIsEnabled);
	GLboolean ret = CALL_ORIG(glIsEnabled, cap);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));

	return ret;
}

GLboolean glIsFramebuffer(GLuint framebuffer)
{
#undef glIsFramebuffer
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsFramebuffer);
	GLboolean ret = CALL_ORIG(glIsFramebuffer, framebuffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", framebuffer);

	return ret;
}

GLboolean glIsProgram(GLuint program)
{
#undef glIsProgram
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsProgram);
	GLboolean ret = CALL_ORIG(glIsProgram, program);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

GLboolean glIsRenderbuffer(GLuint renderbuffer)
{
#undef glIsRenderbuffer
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsRenderbuffer);
	GLboolean ret = CALL_ORIG(glIsRenderbuffer, renderbuffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", renderbuffer);

	return ret;
}

GLboolean glIsShader(GLuint shader)
{
#undef glIsShader
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsShader);
	GLboolean ret = CALL_ORIG(glIsShader, shader);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", shader);

	return ret;
}

GLboolean glIsTexture(GLuint texture)
{
#undef glIsTexture
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsTexture);
	GLboolean ret = CALL_ORIG(glIsTexture, texture);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", texture);

	return ret;
}

// ==================================================================
// L 2
// ==================================================================

void glLineWidth(GLfloat width)
{
#undef glLineWidth
	typedef void (*methodType)(GLfloat);
	BEFORE(glLineWidth);
	CALL_ORIG(glLineWidth, width);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", width);
}

void glLinkProgram(GLuint program)
{
#undef glLinkProgram
	typedef void (*methodType)(GLuint);
	BEFORE(glLinkProgram);
	CALL_ORIG(glLinkProgram, program);
	GL_GET_ERROR();
	char buf[512] = "";
	if (error == GL_NO_ERROR) {
		int bufP = 0;
		int i;
		GLint activeNum[1];
		GLint maxLength[1];
		real_glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, activeNum);
		real_glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
				    maxLength);
		bufP += sprintf(buf + bufP, "%d", activeNum[0]);
		for (i = 0; i < activeNum[0]; i++) {
			GLsizei length[1];
			GLint size[1];
			GLenum type[1];
			char name[maxLength[0]];
			real_glGetActiveAttrib(program, i, maxLength[0], length,
					       size, type, name);
			bufP += sprintf(buf + bufP, ",%d,%s,%d,%x", i, name,
					size[0], type[0]);
		}

		real_glGetProgramiv(program, GL_ACTIVE_UNIFORMS, activeNum);
		real_glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
				    maxLength);
		bufP += sprintf(buf + bufP, ",%d", activeNum[0]);
		for (i = 0; i < activeNum[0]; i++) {
			GLsizei length[1];
			GLint size[1];
			GLenum type[1];
			char name[maxLength[0]];
			real_glGetActiveUniform(program, i, maxLength[0],
						length, size, type, name);
			bufP += sprintf(buf + bufP, ",%d,%s,%d,%x", i, name, size[0],
					type[0]);
		}
	}
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "d",
	      program);
}

// ==================================================================
// P 2
// ==================================================================

void glPixelStorei(GLenum pname, GLint param)
{
#undef glPixelStorei
	typedef void (*methodType)(GLenum, GLint);
	BEFORE(glPixelStorei);
	CALL_ORIG(glPixelStorei, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(pname), param);
}

void glPolygonOffset(GLfloat factor, GLfloat units)
{
#undef glPolygonOffset
	typedef void (*methodType)(GLfloat, GLfloat);
	BEFORE(glPolygonOffset);
	CALL_ORIG(glPolygonOffset, factor, units);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
	      factor, units);
}

// ==================================================================
// R 3
// ==================================================================

//lsh_get
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
		  GLenum format, GLenum type, GLvoid * data)
{
#undef glReadPixels
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei, GLenum,
				   GLenum, GLvoid *);
	BEFORE(glReadPixels);
	CALL_ORIG(glReadPixels, x, y, width, height, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "ddddxx", x, y, width, height,
	      (uint64_t)(format), (uint64_t)(type));
}

void glReleaseShaderCompiler(void)
{
#undef glReleaseShaderCompiler
	typedef void (*methodType)(void);
	BEFORE(glReleaseShaderCompiler);
	CALL_ORIG(glReleaseShaderCompiler,);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width,
			   GLsizei height)
{
#undef glRenderbufferStorage
	typedef void (*methodType)(GLenum, GLenum, GLsizei, GLsizei);
	BEFORE(glRenderbufferStorage);
	CALL_ORIG(glRenderbufferStorage, target, internalformat, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
	      (uint64_t)(target), (uint64_t)(internalformat), width, height);
}

// ==================================================================
// S 10
// ==================================================================

void glSampleCoverage(GLclampf value, GLboolean invert)
{
#undef glSampleCoverage
	typedef void (*methodType)(GLclampf, GLboolean);
	BEFORE(glSampleCoverage);
	CALL_ORIG(glSampleCoverage, value, invert);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "fx",
	      value, (uint64_t)(invert));
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
#undef glScissor
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);
	BEFORE(glScissor);
	CALL_ORIG(glScissor, x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
	      x, y, width, height);
}

//lsh_param
void glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
		    const void *binary, GLsizei length)
{
#undef glShaderBinary
	typedef void (*methodType)(GLsizei, const GLuint *, GLenum,
				   const void *, GLsizei);
	BEFORE(glShaderBinary);
	CALL_ORIG(glShaderBinary, n, shaders, binaryformat, binary, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "dpxpd", n, voidp_to_uint64(shaders),
	      (uint64_t)(binaryformat), voidp_to_uint64(binary), length);
}

//lsh_param
void glShaderSource(GLuint shader, GLsizei count, const char **string,
		    const GLint *length)
{
#undef glShaderSource
	typedef void (*methodType)(GLuint, GLsizei, const char **,
				   const GLint*);
	BEFORE(glShaderSource);
	CALL_ORIG(glShaderSource, shader, count, string, length);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		GLint length[1];
		real_glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, length);
		char buf[length[0]];
		real_glGetShaderSource(shader, length[0], NULL, buf);
		AFTER_SHADER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf,
			     length[0], "ddpp", shader, count,
			     voidp_to_uint64(string), voidp_to_uint64(length));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
		      "ddpp", shader, count,
		      voidp_to_uint64(string), voidp_to_uint64(length));
	}
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask)
{
#undef glStencilFunc
	typedef void (*methodType)(GLenum, GLint, GLint);
	BEFORE(glStencilFunc);
	CALL_ORIG(glStencilFunc, func, ref, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
	      (uint64_t)(func), ref, mask);
}

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
#undef glStencilFuncSeparate
	typedef void (*methodType)(GLenum, GLenum, GLint, GLuint);
	BEFORE(glStencilFuncSeparate);
	CALL_ORIG(glStencilFuncSeparate, face, func, ref, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
	      (uint64_t)(face), (uint64_t)(func), ref, mask);
}

void glStencilMask(GLuint mask)
{
#undef glStencilMask
	typedef void (*methodType)(GLuint);
	BEFORE(glStencilMask);
	CALL_ORIG(glStencilMask, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", mask);
}

void glStencilMaskSeparate(GLenum face, GLuint mask)
{
#undef glStencilMaskSeparate
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glStencilMaskSeparate);
	CALL_ORIG(glStencilMaskSeparate, face, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(face), mask);
}

void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
#undef glStencilOp
	typedef void (*methodType)(GLenum, GLenum, GLenum);
	BEFORE(glStencilOp);
	CALL_ORIG(glStencilOp, sfail, dpfail, dppass);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(sfail), (uint64_t)(dpfail),
	      (uint64_t)(dppass));
}

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail,
			 GLenum dppass)
{
#undef glStencilOpSeparate
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glStencilOpSeparate);
	CALL_ORIG(glStencilOpSeparate, face, sfail, dpfail, dppass);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
	      (uint64_t)(face), (uint64_t)(sfail), (uint64_t)(dpfail),
	      (uint64_t)(dppass));
}

// ==================================================================
// T 6
// ==================================================================

void glTexImage2D(GLenum target, GLint level, GLenum internalformat,
		  GLsizei width, GLsizei height, GLint border, GLenum format,
		  GLenum type, const GLvoid *data)
{
#undef glTexImage2D
	typedef void (*methodType)(GLenum, GLint, GLint, GLsizei, GLsizei,
				   GLint, GLenum, GLenum, const GLvoid *);
	BEFORE(glTexImage2D);
	CALL_ORIG(glTexImage2D, target, level, internalformat, width,
		  height, border, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddxxp",
	      (uint64_t)(target), level, internalformat, width, height,
	      border, (uint64_t)(format), (uint64_t)(type),
	      voidp_to_uint64(data));
}

void glTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
#undef glTexParameterf
	typedef void (*methodType)(GLenum, GLenum, GLfloat);
	BEFORE(glTexParameterf);
	CALL_ORIG(glTexParameterf, target, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxf",
	      (uint64_t)(target), (uint64_t)(pname), param);
}

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params)
{
#undef glTexParameterfv
	typedef void (*methodType)(GLenum, GLenum, const GLfloat *);
	BEFORE(glTexParameterfv);
	CALL_ORIG(glTexParameterfv, target, pname, params);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char param0[8];
		sprintf(param0, "%x", (GLenum)params[0]);
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, param0, "xxp",
		      (uint64_t)(target), (uint64_t)(pname),
		      voidp_to_uint64(params));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
		      (uint64_t)(target), (uint64_t)(pname),
		      voidp_to_uint64(params));
	}
}

void glTexParameteri(GLenum target, GLenum pname, GLint param)
{
#undef glTexParameteri
	typedef void (*methodType)(GLenum, GLenum, GLint);
	BEFORE(glTexParameteri);
	CALL_ORIG(glTexParameteri, target, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(target), (uint64_t)(pname),
	      (uint64_t)(param));
}

void glTexParameteriv(GLenum target, GLenum pname, const GLint * params)
{
#undef glTexParameteriv
	typedef void (*methodType)(GLenum, GLenum, const GLint *);
	BEFORE(glTexParameteriv);
	CALL_ORIG(glTexParameteriv, target, pname, params);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char param0[8];
		sprintf(param0, "%x", (GLenum)params[0]);
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, param0, "xxp",
		      (uint64_t)(target), (uint64_t)(pname),
		      voidp_to_uint64(params));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
		      (uint64_t)(target),
		      (uint64_t)(pname),
		      voidp_to_uint64(params));
	}
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
		     GLsizei width, GLsizei height, GLenum format, GLenum type,
		     const GLvoid * data)
{
#undef glTexSubImage2D
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei,
				   GLsizei, GLenum, GLenum, const GLvoid *);
	BEFORE(glTexSubImage2D);
	CALL_ORIG(glTexSubImage2D, target, level, xoffset, yoffset, width,
		  height, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "xdddddxxp",
	      (uint64_t)(target), level, xoffset, yoffset, width, height,
	      (uint64_t)(format), (uint64_t)(type), voidp_to_uint64(data));
}

// ==================================================================
// U 20
// ==================================================================

void glUniform1f(GLint location, GLfloat v0)
{
#undef glUniform1f
	typedef void (*methodType)(GLint, GLfloat);
	BEFORE(glUniform1f);
	CALL_ORIG(glUniform1f, location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "df",
	      location, v0);
}

void glUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
#undef glUniform2f
	typedef void (*methodType)(GLint, GLfloat, GLfloat);
	BEFORE(glUniform2f);
	CALL_ORIG(glUniform2f, location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dff",
	      location, v0, v1);
}

void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
#undef glUniform3f
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform3f);
	CALL_ORIG(glUniform3f, location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dfff",
	      location, v0, v1, v2);
}

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
#undef glUniform4f
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform4f);
	CALL_ORIG(glUniform4f, location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dffff",
	      location, v0, v1, v2, v3);
}

void glUniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
#undef glUniform1fv
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform1fv);
	CALL_ORIG(glUniform1fv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 1, voidp_to_uint64(value));
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
#undef glUniform2fv
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform2fv);
	CALL_ORIG(glUniform2fv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 2, voidp_to_uint64(value));
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
#undef glUniform3fv
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform3fv);
	CALL_ORIG(glUniform3fv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 3, voidp_to_uint64(value));
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
#undef glUniform4fv
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform4fv);
	CALL_ORIG(glUniform4fv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 4, voidp_to_uint64(value));
}

void glUniform1i(GLint location, GLint v0)
{
#undef glUniform1i
	typedef void (*methodType)(GLint, GLint);
	BEFORE(glUniform1i);
	CALL_ORIG(glUniform1i, location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      location, v0);
}

void glUniform2i(GLint location, GLint v0, GLint v1)
{
#undef glUniform2i
	typedef void (*methodType)(GLint, GLint, GLint);
	BEFORE(glUniform2i);
	CALL_ORIG(glUniform2i, location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
	      location, v0, v1);
}

void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
#undef glUniform3i
	typedef void (*methodType)(GLint, GLint, GLint, GLint);
	BEFORE(glUniform3i);
	CALL_ORIG(glUniform3i, location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
	      location, v0, v1, v2);
}

void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
#undef glUniform4i
	typedef void (*methodType)(GLint, GLint, GLint, GLint, GLint);
	BEFORE(glUniform4i);
	CALL_ORIG(glUniform4i, location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddddd",
	      location, v0, v1, v2, v3);
}

void glUniform1iv(GLint location, GLsizei count, const GLint *value)
{
#undef glUniform1iv
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform1iv);
	CALL_ORIG(glUniform1iv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 1, voidp_to_uint64(value));
}

void glUniform2iv(GLint location, GLsizei count, const GLint *value)
{
#undef glUniform2iv
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform2iv);
	CALL_ORIG(glUniform2iv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 2, voidp_to_uint64(value));
}

void glUniform3iv(GLint location, GLsizei count, const GLint *value)
{
#undef glUniform3iv
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform3iv);
	CALL_ORIG(glUniform3iv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 3, voidp_to_uint64(value));
}

void glUniform4iv(GLint location, GLsizei count, const GLint *value)
{
#undef glUniform4iv
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform4iv);
	CALL_ORIG(glUniform4iv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 4, voidp_to_uint64(value));
}

void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
			const GLfloat *value)
{
#undef glUniformMatrix2fv
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix2fv);
	CALL_ORIG(glUniformMatrix2fv, location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 2 * 2,
	      voidp_to_uint64(value));
}

void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
			const GLfloat *value)
{
#undef glUniformMatrix3fv
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix3fv);
	CALL_ORIG(glUniformMatrix3fv, location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 3 * 3,
	      voidp_to_uint64(value));
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
			const GLfloat *value)
{
#undef glUniformMatrix4fv
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix4fv);
	CALL_ORIG(glUniformMatrix4fv, location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 4 * 4,
	      voidp_to_uint64(value));
}

void glUseProgram(GLuint program)
{
#undef glUseProgram
	typedef void (*methodType)(GLuint);
	BEFORE(glUseProgram);
	CALL_ORIG(glUseProgram, program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
	      program);
}

// ==================================================================
// V 7
// ==================================================================

void glValidateProgram(GLuint program)
{
#undef glValidateProgram
	typedef void (*methodType)(GLuint);
	BEFORE(glValidateProgram);
	CALL_ORIG(glValidateProgram, program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
	      program);
}

void glVertexAttrib1f(GLuint index, GLfloat v0)
{
#undef glVertexAttrib1f
	typedef void (*methodType)(GLuint, GLfloat);
	BEFORE(glVertexAttrib1f);
	CALL_ORIG(glVertexAttrib1f, index, v0);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue, "df",
	      index, v0);
}

void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
#undef glVertexAttrib2f
	typedef void (*methodType)(GLuint, GLfloat, GLfloat);
	BEFORE(glVertexAttrib2f);
	CALL_ORIG(glVertexAttrib2f, index, v0, v1);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue, "dff",
	      index, v0, v1);
}

void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
#undef glVertexAttrib3f
	typedef void (*methodType)(GLuint, GLfloat, GLfloat, GLfloat);
	BEFORE(glVertexAttrib3f);
	CALL_ORIG(glVertexAttrib3f, index, v0, v1, v2);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue, "dfff",
	      index, v0, v1, v2);
}

void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2,
		      GLfloat v3)
{
#undef glVertexAttrib4f
	typedef void (*methodType)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
	BEFORE(glVertexAttrib4f);
	CALL_ORIG(glVertexAttrib4f, index, v0, v1, v2, v3);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue, "dffff",
	      index, v0, v1, v2, v3);
}

void glVertexAttrib1fv(GLuint index, const GLfloat *v)
{
#undef glVertexAttrib1fv
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib1fv);
	CALL_ORIG(glVertexAttrib1fv, index, v);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib2fv(GLuint index, const GLfloat *v)
{
#undef glVertexAttrib2fv
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib2fv);
	CALL_ORIG(glVertexAttrib2fv, index, v);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib3fv(GLuint index, const GLfloat *v)
{
#undef glVertexAttrib3fv
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib3fv);
	CALL_ORIG(glVertexAttrib3fv, index, v);

	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib4fv(GLuint index, const GLfloat *v)
{
#undef glVertexAttrib4fv
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib4fv);
	CALL_ORIG(glVertexAttrib4fv, index, v);
	GLfloat cv[4];
	real_glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
			   GLboolean normalized, GLsizei stride,
			   const GLvoid *pointer)
{
#undef glVertexAttribPointer
	typedef void (*methodType)(GLuint, GLint, GLenum, GLboolean, GLsizei,
				   const GLvoid *);
	BEFORE(glVertexAttribPointer);
	CALL_ORIG(glVertexAttribPointer, index, size, type, normalized, stride,
		  pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxxdp",
	      index, size, (uint64_t)(type), (uint64_t)(normalized),
	      stride, voidp_to_uint64(pointer));
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
#undef glViewport
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);

	BEFORE(glViewport);
	if (blockresult)
		PRINTWRN("olololo");

	CALL_ORIG(glViewport, x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
	      x, y, width, height);
}

#undef CALL_ORIG
#undef BEFORE
