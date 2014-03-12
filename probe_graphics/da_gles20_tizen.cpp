/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

static char contextValue[MAX_GL_CONTEXT_VALUE_SIZE];
static enum DaOptions _sopt = OPT_GLES;
static __thread GLenum gl_error_external = GL_NO_ERROR;

// ==================================================================
// A 2
// ==================================================================

void glActiveTexture(GLenum texture) {
	typedef void (*methodType)(GLenum);
	BEFORE(glActiveTexture);
	glActiveTexturep(texture);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(texture));
}

void glAttachShader(GLuint program, GLuint shader) {
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glAttachShader);
	glAttachShaderp(program, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, shader);
}

// ==================================================================
// B 12
// ==================================================================

void glBindAttribLocation(GLuint program, GLuint index, const char *name) {
	typedef void (*methodType)(GLuint, GLuint, const char *);
	BEFORE(glBindAttribLocation);
	glBindAttribLocationp(program, index, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dds",
			program, index, name);
}

void glBindBuffer(GLenum target, GLuint buffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindBuffer);
	glBindBufferp(target, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), buffer);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindFramebuffer);
	glBindFramebufferp(target, framebuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), framebuffer);
}

void glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindRenderbuffer);
	glBindRenderbufferp(target, renderbuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), renderbuffer);
}

void glBindTexture(GLenum target, GLuint texture) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindTexture);
	glBindTexturep(target, texture);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(target), texture);
}

void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glBlendColor);
	glBlendColorp(red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
			red, green, blue, alpha);
}

void glBlendEquation(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glBlendEquation);
	glBlendEquationp(mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mode));
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendEquationSeparate);
	glBlendEquationSeparatep(modeRGB, modeAlpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(modeRGB), (uint64_t)(modeAlpha));
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendFunc);
	glBlendFuncp(sfactor, dfactor);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(sfactor), (uint64_t)(dfactor));
}

void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
		GLenum dstAlpha) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glBlendFuncSeparate);
	glBlendFuncSeparatep(srcRGB, dstRGB, srcAlpha, dstAlpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
	      (uint64_t)(srcRGB), (uint64_t)(dstRGB),
	      (uint64_t)(srcAlpha), (uint64_t)(dstAlpha));
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data,
		GLenum usage) {
	typedef void (*methodType)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
	BEFORE(glBufferData);
	glBufferDatap(target, size, data, usage);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxpx",
	      (uint64_t)(target), (uint64_t)(size),
	      voidp_to_uint64(data), (uint64_t)(usage));
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLintptr, GLsizeiptr, const GLvoid *);
	BEFORE(glBufferSubData);
	glBufferSubDatap(target, offset, size, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxp",
	      (uint64_t)(target), (uint64_t)(offset),
	      (uint64_t)(size), voidp_to_uint64(data));
}

// ==================================================================
// C 14
// ==================================================================

GLenum glCheckFramebufferStatus(GLenum target) {
	typedef GLenum (*methodType)(GLenum);
	BEFORE(glCheckFramebufferStatus);
	GLenum ret = glCheckFramebufferStatusp(target);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(target));

	return ret;
}

void glClear(GLbitfield mask) {
	typedef void (*methodType)(GLbitfield);
	BEFORE(glClear);
	glClearp(mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mask));
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glClearColor);
	glClearColorp(red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
			red, green, blue, alpha);
}

void glClearDepthf(GLclampf depth) {
	typedef void (*methodType)(GLclampf);
	BEFORE(glClearDepthf);
	glClearDepthfp(depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", depth);
}

void glClearStencil(GLint s) {
	typedef void (*methodType)(GLint);
	BEFORE(glClearStencil);
	glClearStencilp(s);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", s);
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue,
		GLboolean alpha) {
	typedef void (*methodType)(GLboolean, GLboolean, GLboolean, GLboolean);
	BEFORE(glColorMask);
	glColorMaskp(red, green, blue, alpha);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			red, green, blue, alpha);
}

void glCompileShader(GLuint shader) {
	typedef void (*methodType)(GLuint);
	BEFORE(glCompileShader);
	glCompileShaderp(shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(shader));
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint,
			GLsizei, const GLvoid *);
	BEFORE(glCompressedTexImage2D);
	glCompressedTexImage2Dp(target, level, internalformat, width, height,
			border, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxddddp",
	      (uint64_t)(target), level,
	      (uint64_t)(internalformat), width, height, border, imageSize,
	      voidp_to_uint64(data));
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
		GLint yoffset, GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
			GLenum, GLsizei, const GLvoid *);
	BEFORE(glCompressedTexSubImage2D);
	glCompressedTexSubImage2Dp(target, level, xoffset, yoffset, width, height,
			format, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddxdp",
	      (uint64_t)(target), level, xoffset, yoffset, width, height,
	      (uint64_t)(format), imageSize, voidp_to_uint64(data));
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
	typedef void (*methodType)(GLenum, GLint, GLenum, GLint, GLint, GLsizei,
			GLsizei, GLint);
	BEFORE(glCopyTexImage2D);
	glCopyTexImage2Dp(target, level, internalformat, x, y, width, height,
			border);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxddddd",
	      (uint64_t)(target), level,
	      (uint64_t)(internalformat), x, y, width, height, border);
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
		GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLint, GLint,
			GLsizei, GLsizei);
	BEFORE(glCopyTexSubImage2D);
	glCopyTexSubImage2Dp(target, level, xoffset, yoffset, x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddddddd",
	      (uint64_t)(target), level, xoffset, yoffset, x, y, width,
	      height);
}

GLuint glCreateProgram(void) {
	typedef GLuint (*methodType)(void);
	BEFORE(glCreateProgram);
	GLuint ret = glCreateProgramp();
	GL_GET_ERROR();
	AFTER_NO_PARAM('d', ret, APITYPE_CONTEXT, "");

	return ret;
}

GLuint glCreateShader(GLenum shaderType) {
	typedef GLuint (*methodType)(GLenum);
	BEFORE(glCreateShader);
	GLuint ret = glCreateShaderp(shaderType);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(shaderType));

	return ret;
}

void glCullFace(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glCullFace);
	glCullFacep(mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(mode));
}

// ==================================================================
// D 14
// ==================================================================

void glDeleteBuffers(GLsizei n, const GLuint * buffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteBuffers);
	glDeleteBuffersp(n, buffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(buffers));
}

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteFramebuffers);
	glDeleteFramebuffersp(n, framebuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(framebuffers));
}

void glDeleteProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteProgram);
	glDeleteProgramp(program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteRenderbuffers);
	glDeleteRenderbuffersp(n, renderbuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(renderbuffers));
}

void glDeleteShader(GLuint shader) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteShader);
	glDeleteShaderp(shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", shader);
}

void glDeleteTextures(GLsizei n, const GLuint * textures) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteTextures);
	glDeleteTexturesp(n, textures);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
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

void glDepthFunc(GLenum func) {
	typedef void (*methodType)(GLenum);
	BEFORE(glDepthFunc);
	glDepthFuncp(func);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(func));
}

void glDepthMask(GLboolean flag) {
	typedef void (*methodType)(GLboolean);
	BEFORE(glDepthMask);
	glDepthMaskp(flag);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(flag));
}

void glDepthRangef(GLclampf nearVal, GLclampf farVal) {
	typedef void (*methodType)(GLclampf, GLclampf);
	BEFORE(glDepthRangef);
	glDepthRangefp(nearVal, farVal);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
			nearVal, farVal);
}

void glDetachShader(GLuint program, GLuint shader) {
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glDetachShader);
	glDetachShaderp(program, shader);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, shader);
}

void glDisable(GLenum cap) {
	typedef void (*methodType)(GLenum);
	BEFORE(glDisable);
	glDisablep(cap);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));
}

void glDisableVertexAttribArray(GLuint index) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDisableVertexAttribArray);
	glDisableVertexAttribArrayp(index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
	typedef void (*methodType)(GLenum, GLint, GLsizei);
	BEFORE(glDrawArrays);
	glDrawArraysp(mode, first, count);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
	      (uint64_t)(mode), first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type,
		const GLvoid * indices) {
	typedef void (*methodType)(GLenum, GLsizei, GLenum, const GLvoid *);
	BEFORE(glDrawElements);
	glDrawElementsp(mode, count, type, indices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxp",
	      (uint64_t)(mode), count, (uint64_t)(type), indices);
}

// ==================================================================
// E 2
// ==================================================================

void glEnable(GLenum cap) {
	typedef void (*methodType)(GLenum);
	BEFORE(glEnable);
	glEnablep(cap);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));
}

void glEnableVertexAttribArray(GLuint index) {
	typedef void (*methodType)(GLuint);
	BEFORE(glEnableVertexAttribArray);
	glEnableVertexAttribArrayp(index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

// ==================================================================
// F 5
// ==================================================================

void glFinish(void) {
	typedef void (*methodType)(void);
	BEFORE(glFinish);
	glFinishp();
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFlush(void) {
	typedef void (*methodType)(void);
	BEFORE(glFlush);
	glFlushp();
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment,
		GLenum renderbuffertarget, GLuint renderbuffer) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint);
	BEFORE(glFramebufferRenderbuffer);
	glFramebufferRenderbufferp(target, attachment, renderbuffertarget,
			renderbuffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxd",
	      (uint64_t)(target), (uint64_t)(attachment),
	      (uint64_t)(renderbuffertarget), renderbuffer);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
		GLuint texture, GLint level) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint, GLint);
	BEFORE(glFramebufferTexture2D);
	glFramebufferTexture2Dp(target, attachment, textarget, texture, level);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxdd",
	      (uint64_t)(target), (uint64_t)(attachment),
	      (uint64_t)(textarget), texture, level);
}

void glFrontFace(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glFrontFace);
	glFrontFacep(mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      (uint64_t)(mode));
}

// ==================================================================
// G 31
// ==================================================================

void glGenBuffers(GLsizei n, GLuint * buffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenBuffers);
	glGenBuffersp(n, buffers);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
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

void glGenFramebuffers(GLsizei n, GLuint * framebuffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenFramebuffers);
	glGenFramebuffersp(n, framebuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(framebuffers));
}

void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenRenderbuffers);
	glGenRenderbuffersp(n, renderbuffers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
	      n, voidp_to_uint64(renderbuffers));
}

void glGenTextures(GLsizei n, GLuint * textures) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenTextures);
	glGenTexturesp(n, textures);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
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

void glGenerateMipmap(GLenum target) {
	typedef void (*methodType)(GLenum);
	BEFORE(glGenerateMipmap);
	glGenerateMipmapp(target);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(target));
}

//lsh_get
void glGetBooleanv(GLenum pname, GLboolean * params) {
	typedef void (*methodType)(GLenum, GLboolean *);
	BEFORE(glGetBooleanv);
	glGetBooleanvp(pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(pname));
}

//lsh_get
void glGetFloatv(GLenum pname, GLfloat * params) {
	typedef void (*methodType)(GLenum, GLfloat *);
	BEFORE(glGetFloatv);
	glGetFloatvp(pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(pname));
}

//lsh_get
void glGetIntegerv(GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLint *);
	BEFORE(glGetIntegerv);
	glGetIntegervp(pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", (uint64_t)(pname));
}

//lsh_get
void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize,
		GLsizei *length, GLint *size, GLenum *type, char *name) {
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
			GLenum *, char *);
	BEFORE(glGetActiveAttrib);
	glGetActiveAttribp(program, index, bufSize, length, size, type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			program, index, bufSize);
}

//lsh_get
void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize,
		GLsizei *length, GLint *size, GLenum *type, char *name) {
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
			GLenum *, char *);
	BEFORE(glGetActiveUniform);
	glGetActiveUniformp(program, index, bufSize, length, size, type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			program, index, bufSize);
}

//lsh_get
void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count,
		GLuint *shaders) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, GLuint *);
	BEFORE(glGetAttachedShaders);
	glGetAttachedShadersp(program, maxCount, count, shaders);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, maxCount);
}

//lsh_get
int glGetAttribLocation(GLuint program, const char* name)
{
	typedef int (*methodType)(GLuint , const char*);
	BEFORE(glGetAttribLocation);
	int ret = glGetAttribLocationp(program, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xs",
	      (uint64_t)(program), name);
	return ret;
}

//lsh_get
void glGetBufferParameteriv(GLenum target, GLenum value, GLint * data) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetBufferParameteriv);
	glGetBufferParameterivp(target, value, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(value));
}

GLenum glGetError(void) {
	typedef GLenum (*methodType)(void);
	BEFORE(glGetError);
	GLenum ret = glGetErrorp();

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

//lsh_get
void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment,
		GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLint *);
	BEFORE(glGetFramebufferAttachmentParameteriv);
	glGetFramebufferAttachmentParameterivp(target, attachment, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(target), (uint64_t)(attachment),
	      (uint64_t)(pname));
}

//lsh_get
void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length,
		char *infoLog) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetProgramInfoLog);
	glGetProgramInfoLogp(program, maxLength, length, infoLog);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, maxLength);
}

//lsh_get
void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetProgramiv);
	glGetProgramivp(program, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      program, (uint64_t)(pname));
}

//lsh_get
void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetRenderbufferParameteriv);
	glGetRenderbufferParameterivp(target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length,
		char *infoLog) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderInfoLog);
	glGetShaderInfoLogp(shader, maxLength, length, infoLog);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			shader, maxLength);
}

//lsh_get
void glGetShaderPrecisionFormat(GLenum shaderType, GLenum precisionType,
		GLint *range, GLint *precision) {
	typedef void (*methodType)(GLenum, GLenum, GLint *, GLint *);
	BEFORE(glGetShaderPrecisionFormat);
	glGetShaderPrecisionFormatp(shaderType, precisionType, range, precision);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(shaderType), (uint64_t)(precisionType));
}

//lsh_get
void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length,
		char *source) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderSource);
	glGetShaderSourcep(shader, bufSize, length, source);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			shader, bufSize);
}

//lsh_get
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetShaderiv);
	glGetShaderivp(shader, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      shader, (uint64_t)(pname));
}

const GLubyte* glGetString(GLenum name) {
	typedef const GLubyte* (*methodType)(GLenum);
	BEFORE(glGetString);
	const GLubyte* ret = glGetStringp(name);
	GL_GET_ERROR();
	AFTER('p', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(name));

	return ret;
}

//lsh_get
void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params) {
	typedef void (*methodType)(GLenum, GLenum, GLfloat *);
	BEFORE(glGetTexParameterfv);
	glGetTexParameterfvp(target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetTexParameteriv);
	glGetTexParameterivp(target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(pname));
}

//lsh_get
void glGetUniformfv(GLuint program, GLint location, GLfloat *params) {
	typedef void (*methodType)(GLuint, GLuint, GLfloat *);
	BEFORE(glGetUniformfv);
	glGetUniformfvp(program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, location);
}

//lsh_get
void glGetUniformiv(GLuint program, GLint location, GLint *params) {
	typedef void (*methodType)(GLuint, GLuint, GLint *);
	BEFORE(glGetUniformiv);
	glGetUniformivp(program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, location);
}

//lsh_get
GLint glGetUniformLocation(GLuint program, const char *name) {
	typedef GLint (*methodType)(GLuint, const char *);
	BEFORE(glGetUniformLocation);
	GLint ret = glGetUniformLocationp(program, name);
	GL_GET_ERROR();
	AFTER('d', ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

//lsh_get
void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) {
	typedef void (*methodType)(GLuint, GLenum, GLfloat *);
	BEFORE(glGetVertexAttribfv);
	glGetVertexAttribfvp(index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

//lsh_get
void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetVertexAttribiv);
	glGetVertexAttribivp(index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

//lsh_get
void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer) {
	typedef void (*methodType)(GLuint, GLenum, GLvoid **);
	BEFORE(glGetVertexAttribPointerv);
	glGetVertexAttribPointervp(index, pname, pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
	      index, (uint64_t)(pname));
}

// ==================================================================
// H 1
// ==================================================================

void glHint(GLenum target, GLenum mode) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glHint);
	glHintp(target, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
	      (uint64_t)(target), (uint64_t)(mode));
}

// ==================================================================
// I 7
// ==================================================================

GLboolean glIsBuffer(GLuint buffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsBuffer);
	GLboolean ret = glIsBufferp(buffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", buffer);

	return ret;
}

GLboolean glIsEnabled(GLenum cap) {
	typedef GLboolean (*methodType)(GLenum);
	BEFORE(glIsEnabled);
	GLboolean ret = glIsEnabledp(cap);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "x", (uint64_t)(cap));

	return ret;
}

GLboolean glIsFramebuffer(GLuint framebuffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsFramebuffer);
	GLboolean ret = glIsFramebufferp(framebuffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", framebuffer);

	return ret;
}

GLboolean glIsProgram(GLuint program) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsProgram);
	GLboolean ret = glIsProgramp(program);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

GLboolean glIsRenderbuffer(GLuint renderbuffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsRenderbuffer);
	GLboolean ret = glIsRenderbufferp(renderbuffer);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", renderbuffer);

	return ret;
}

GLboolean glIsShader(GLuint shader) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsShader);
	GLboolean ret = glIsShaderp(shader);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", shader);

	return ret;
}

GLboolean glIsTexture(GLuint texture) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsTexture);
	GLboolean ret = glIsTexturep(texture);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", texture);

	return ret;
}

// ==================================================================
// L 2
// ==================================================================

void glLineWidth(GLfloat width) {
	typedef void (*methodType)(GLfloat);
	BEFORE(glLineWidth);
	glLineWidthp(width);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", width);
}

void glLinkProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glLinkProgram);
	glLinkProgramp(program);
	GL_GET_ERROR();
	char buf[512] = "";
	if (error == GL_NO_ERROR) {
		int bufP = 0;
		GLint activeNum[1];
		GLint maxLength[1];
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, activeNum);
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, maxLength);
		bufP += sprintf(buf + bufP, "%d", activeNum[0]);
		for (int i = 0; i < activeNum[0]; i++) {
			GLsizei length[1];
			GLint size[1];
			GLenum type[1];
			char name[maxLength[0]];
			glGetActiveAttrib(program, i, maxLength[0], length, size, type,
					name);
			bufP += sprintf(buf + bufP, ",%d,%s,%d,%x", i, name, size[0],
					type[0]);
		}

		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, activeNum);
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, maxLength);
		bufP += sprintf(buf + bufP, ",%d", activeNum[0]);
		for (int i = 0; i < activeNum[0]; i++) {
			GLsizei length[1];
			GLint size[1];
			GLenum type[1];
			char name[maxLength[0]];
			glGetActiveUniform(program, i, maxLength[0], length, size, type,
					name);
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

void glPixelStorei(GLenum pname, GLint param) {
	typedef void (*methodType)(GLenum, GLint);
	BEFORE(glPixelStorei);
	glPixelStoreip(pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(pname), param);
}

void glPolygonOffset(GLfloat factor, GLfloat units) {
	typedef void (*methodType)(GLfloat, GLfloat);
	BEFORE(glPolygonOffset);
	glPolygonOffsetp(factor, units);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
			factor, units);
}

// ==================================================================
// R 3
// ==================================================================

//lsh_get
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height,
		GLenum format, GLenum type, GLvoid * data) {
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum,
			GLvoid *);
	BEFORE(glReadPixels);
	glReadPixelsp(x, y, width, height, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "ddddxx", x, y, width, height,
	      (uint64_t)(format), (uint64_t)(type));
}

void glReleaseShaderCompiler(void) {
	typedef void (*methodType)(void);
	BEFORE(glReleaseShaderCompiler);
	glReleaseShaderCompilerp();
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height) {
	typedef void (*methodType)(GLenum, GLenum, GLsizei, GLsizei);
	BEFORE(glRenderbufferStorage);
	glRenderbufferStoragep(target, internalformat, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
	      (uint64_t)(target), (uint64_t)(internalformat),
	      width, height);
}

// ==================================================================
// S 10
// ==================================================================

void glSampleCoverage(GLclampf value, GLboolean invert) {
	typedef void (*methodType)(GLclampf, GLboolean);
	BEFORE(glSampleCoverage);
	glSampleCoveragep(value, invert);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "fx",
			value, (uint64_t)(invert));
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);
	BEFORE(glScissor);
	glScissorp(x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			x, y, width, height);
}

//lsh_param
void glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
		const void *binary, GLsizei length) {
	typedef void (*methodType)(GLsizei, const GLuint *, GLenum, const void *,
			GLsizei);
	BEFORE(glShaderBinary);
	glShaderBinaryp(n, shaders, binaryformat, binary, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "dpxpd", n, voidp_to_uint64(shaders),
	      (uint64_t)(binaryformat), voidp_to_uint64(binary), length);
}

//lsh_param
void glShaderSource(GLuint shader, GLsizei count, const char** string,
		const GLint* length) {
	typedef void (*methodType)(GLuint, GLsizei, const char**, const GLint*);
	BEFORE(glShaderSource);
	glShaderSourcep(shader, count, string, length);
	GL_GET_ERROR();
	if (error == GL_NO_ERROR) {
		GLint length[1];
		glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, length);
		char buf[length[0]];
		glGetShaderSource(shader, length[0], NULL, buf);
		AFTER_SHADER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, buf,
			     length[0], "ddpp", shader, count,
			     voidp_to_uint64(string), voidp_to_uint64(length));
	} else {
		AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
		      "ddpp", shader, count,
		      voidp_to_uint64(string), voidp_to_uint64(length));
	}
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
	typedef void (*methodType)(GLenum, GLint, GLint);
	BEFORE(glStencilFunc);
	glStencilFuncp(func, ref, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
	      (uint64_t)(func), ref, mask);
}

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
	typedef void (*methodType)(GLenum, GLenum, GLint, GLuint);
	BEFORE(glStencilFuncSeparate);
	glStencilFuncSeparatep(face, func, ref, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
	      (uint64_t)(face), (uint64_t)(func), ref, mask);
}

void glStencilMask(GLuint mask) {
	typedef void (*methodType)(GLuint);
	BEFORE(glStencilMask);
	glStencilMaskp(mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", mask);
}

void glStencilMaskSeparate(GLenum face, GLuint mask) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glStencilMaskSeparate);
	glStencilMaskSeparatep(face, mask);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
	      (uint64_t)(face), mask);
}

void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
	typedef void (*methodType)(GLenum, GLenum, GLenum);
	BEFORE(glStencilOp);
	glStencilOpp(sfail, dpfail, dppass);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(sfail), (uint64_t)(dpfail),
	      (uint64_t)(dppass));
}

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail,
		GLenum dppass) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glStencilOpSeparate);
	glStencilOpSeparatep(face, sfail, dpfail, dppass);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
	      (uint64_t)(face), (uint64_t)(sfail), (uint64_t)(dpfail),
	      (uint64_t)(dppass));
}

// ==================================================================
// T 6
// ==================================================================

void glTexImage2D(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
		const GLvoid *data)
{
	typedef void (*methodType)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint,
			GLenum, GLenum, const GLvoid *);
	BEFORE(glTexImage2D);
	glTexImage2Dp(target, level, internalformat, width, height, border, format,
			type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddxxp",
	      (uint64_t)(target), level, internalformat, width, height,
	      border, (uint64_t)(format), (uint64_t)(type),
	      voidp_to_uint64(data));
}

void glTexParameterf(GLenum target, GLenum pname, GLfloat param) {
	typedef void (*methodType)(GLenum, GLenum, GLfloat);
	BEFORE(glTexParameterf);
	glTexParameterfp(target, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxf",
	      (uint64_t)(target), (uint64_t)(pname), param);
}

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params) {
	typedef void (*methodType)(GLenum, GLenum, const GLfloat *);
	BEFORE(glTexParameterfv);
	glTexParameterfvp(target, pname, params);
	GL_GET_ERROR();
	if(error == GL_NO_ERROR) {
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

void glTexParameteri(GLenum target, GLenum pname, GLint param) {
	typedef void (*methodType)(GLenum, GLenum, GLint);
	BEFORE(glTexParameteri);
	glTexParameterip(target, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
	      (uint64_t)(target), (uint64_t)(pname),
	      (uint64_t)(param));
}

void glTexParameteriv(GLenum target, GLenum pname, const GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, const GLint *);
	BEFORE(glTexParameteriv);
	glTexParameterivp(target, pname, params);
	GL_GET_ERROR();
	if(error == GL_NO_ERROR) {
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
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
			GLenum, GLenum, const GLvoid *);
	BEFORE(glTexSubImage2D);
	glTexSubImage2Dp(target, level, xoffset, yoffset, width, height, format, type,
			data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
	      "xdddddxxp",
	      (uint64_t)(target), level, xoffset, yoffset, width, height,
	      (uint64_t)(format), (uint64_t)(type), voidp_to_uint64(data));
}

// ==================================================================
// U 20
// ==================================================================

void glUniform1f(GLint location, GLfloat v0) {
	typedef void (*methodType)(GLint, GLfloat);
	BEFORE(glUniform1f);
	glUniform1fp(location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "df",
			location, v0);
}

void glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat);
	BEFORE(glUniform2f);
	glUniform2fp(location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dff",
			location, v0, v1);
}

void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform3f);
	glUniform3fp(location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dfff",
			location, v0, v1, v2);
}

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
		GLfloat v3) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform4f);
	glUniform4fp(location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"dffff", location, v0, v1, v2, v3);
}

void glUniform1fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform1fv);
	glUniform1fvp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 1, voidp_to_uint64(value));
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform2fv);
	glUniform2fvp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 2, voidp_to_uint64(value));
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform3fv);
	glUniform3fvp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 3, voidp_to_uint64(value));
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform4fv);
	glUniform4fvp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddF",
	      location, count, count * 4, voidp_to_uint64(value));
}

void glUniform1i(GLint location, GLint v0) {
	typedef void (*methodType)(GLint, GLint);
	BEFORE(glUniform1i);
	glUniform1ip(location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			location, v0);
}

void glUniform2i(GLint location, GLint v0, GLint v1) {
	typedef void (*methodType)(GLint, GLint, GLint);
	BEFORE(glUniform2i);
	glUniform2ip(location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			location, v0, v1);
}

void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
	typedef void (*methodType)(GLint, GLint, GLint, GLint);
	BEFORE(glUniform3i);
	glUniform3ip(location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			location, v0, v1, v2);
}

void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
	typedef void (*methodType)(GLint, GLint, GLint, GLint, GLint);
	BEFORE(glUniform4i);
	glUniform4ip(location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"ddddd", location, v0, v1, v2, v3);
}

void glUniform1iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform1iv);
	glUniform1ivp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 1, voidp_to_uint64(value));
}

void glUniform2iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform2iv);
	glUniform2ivp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 2, voidp_to_uint64(value));
}

void glUniform3iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform3iv);
	glUniform3ivp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 3, voidp_to_uint64(value));
}

void glUniform4iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform4iv);
	glUniform4ivp(location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddD",
	      location, count, count * 4, voidp_to_uint64(value));
}

void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix2fv);
	glUniformMatrix2fvp(location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 2 * 2, voidp_to_uint64(value));
}

void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix3fv);
	glUniformMatrix3fvp(location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 3 * 3, voidp_to_uint64(value));
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix4fv);
	glUniformMatrix4fvp(location, count, transpose, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxF",
	      location, count, (uint64_t)(transpose), count * 4 * 4, voidp_to_uint64(value));
}


void glUseProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glUseProgram);
	glUseProgramp(program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

// ==================================================================
// V 7
// ==================================================================

void glValidateProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glValidateProgram);
	glValidateProgramp(program);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

void glVertexAttrib1f(GLuint index, GLfloat v0) {
	typedef void (*methodType)(GLuint, GLfloat);
	BEFORE(glVertexAttrib1f);
	glVertexAttrib1fp(index, v0);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"df", index, v0);
}

void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1) {
	typedef void (*methodType)(GLuint, GLfloat, GLfloat);
	BEFORE(glVertexAttrib2f);
	glVertexAttrib2fp(index, v0, v1);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dff", index, v0, v1);
}

void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
	typedef void (*methodType)(GLuint, GLfloat, GLfloat, GLfloat);
	BEFORE(glVertexAttrib3f);
	glVertexAttrib3fp(index, v0, v1, v2);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dfff", index, v0, v1, v2);
}

void glVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2,
		GLfloat v3) {
	typedef void (*methodType)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
	BEFORE(glVertexAttrib4f);
	glVertexAttrib4fp(index, v0, v1, v2, v3);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dffff", index, v0, v1, v2, v3);
}

void glVertexAttrib1fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib1fv);
	glVertexAttrib1fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib2fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib2fv);
	glVertexAttrib2fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib3fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib3fv);
	glVertexAttrib3fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttrib4fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib4fv);
	glVertexAttrib4fvp(index, v);
	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
	      "dp", index, voidp_to_uint64(v));
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const GLvoid * pointer) {
	typedef void (*methodType)(GLuint, GLint, GLenum, GLboolean, GLsizei,
			const GLvoid *);
	BEFORE(glVertexAttribPointer);
	glVertexAttribPointerp(index, size, type, normalized, stride, pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxxdp",
	      index, size, (uint64_t)(type), (uint64_t)(normalized),
	      stride, voidp_to_uint64(pointer));
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);
	BEFORE(glViewport);
	glViewportp(x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			x, y, width, height);
}
