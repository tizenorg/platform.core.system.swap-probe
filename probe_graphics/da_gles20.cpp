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
 *
 */
#include "da_gles20.h"
#include "binproto.h"

static enum DaOptions _sopt = OPT_GLES;

// ==================================================================
// A 2
// ==================================================================

void glActiveTexture(GLenum texture) {
	typedef void (*methodType)(GLenum);
	BEFORE(glActiveTexture);
	glActiveTexturep(texture);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
			texture);
}

void glAttachShader(GLuint program, GLuint shader) {
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glAttachShader);
	glAttachShaderp(program, shader);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, shader);
}

// ==================================================================
// B 12
// ==================================================================

void glBindAttribLocation(GLuint program, GLuint index, const char *name) {
	typedef void (*methodType)(GLuint, GLuint, const char *);
	BEFORE(glBindAttribLocation);
	glBindAttribLocationp(program, index, name);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dds",
			program, index, name);
}

void glBindBuffer(GLenum target, GLuint buffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindBuffer);
	glBindBufferp(target, buffer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			target, buffer);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindFramebuffer);
	glBindFramebufferp(target, framebuffer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			target, framebuffer);
}

void glBindRenderbuffer(GLenum target, GLuint renderbuffer) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindRenderbuffer);
	glBindRenderbufferp(target, renderbuffer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			target, renderbuffer);
}

void glBindTexture(GLenum target, GLuint texture) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glBindTexture);
	glBindTexturep(target, texture);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			target, texture);
}

void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glBlendColor);
	glBlendColorp(red, green, blue, alpha);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
			red, green, blue, alpha);
}

void glBlendEquation(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glBlendEquation);
	glBlendEquationp(mode);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", mode);
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendEquationSeparate);
	glBlendEquationSeparatep(modeRGB, modeAlpha);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			modeRGB, modeAlpha);
}

void glBlendFunc(GLenum sfactor, GLenum dfactor) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glBlendFunc);
	glBlendFuncp(sfactor, dfactor);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			sfactor, dfactor);
}

void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,
		GLenum dstAlpha) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glBlendFuncSeparate);
	glBlendFuncSeparatep(srcRGB, dstRGB, srcAlpha, dstAlpha);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
			srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data,
		GLenum usage) {
	typedef void (*methodType)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
	BEFORE(glBufferData);
	glBufferDatap(target, size, data, usage);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xxpx", target, size, data, usage);
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLintptr, GLsizeiptr, const GLvoid *);
	BEFORE(glBufferSubData);
	glBufferSubDatap(target, offset, size, data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xxxp", target, offset, size, data);
}

// ==================================================================
// C 14
// ==================================================================

GLenum glCheckFramebufferStatus(GLenum target) {
	typedef GLenum (*methodType)(GLenum);
	BEFORE(glCheckFramebufferStatus);
	GLenum ret = glCheckFramebufferStatusp(target);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "x", target);

	return ret;
}

void glClear(GLbitfield mask) {
	typedef void (*methodType)(GLbitfield);
	BEFORE(glClear);
	glClearp(mask);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", mask);
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
	typedef void (*methodType)(GLclampf, GLclampf, GLclampf, GLclampf);
	BEFORE(glClearColor);
	glClearColorp(red, green, blue, alpha);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ffff",
			red, green, blue, alpha);
}

void glClearDepthf(GLclampf depth) {
	typedef void (*methodType)(GLclampf);
	BEFORE(glClearDepthf);
	glClearDepthfp(depth);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", depth);
}

void glClearStencil(GLint s) {
	typedef void (*methodType)(GLint);
	BEFORE(glClearStencil);
	glClearStencilp(s);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", s);
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue,
		GLboolean alpha) {
	typedef void (*methodType)(GLboolean, GLboolean, GLboolean, GLboolean);
	BEFORE(glColorMask);
	glColorMaskp(red, green, blue, alpha);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			red, green, blue, alpha);
}

void glCompileShader(GLuint shader) {
	typedef void (*methodType)(GLuint);
	BEFORE(glCompileShader);
	glCompileShaderp(shader);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", shader);
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat,
		GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint,
			GLsizei, const GLvoid *);
	BEFORE(glCompressedTexImage2D);
	glCompressedTexImage2Dp(target, level, internalformat, width, height,
			border, imageSize, data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xdxddddp",
			target, level, internalformat, width, height, border, imageSize, data);
}

void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset,
		GLint yoffset, GLsizei width, GLsizei height, GLenum format,
		GLsizei imageSize, const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
			GLenum, GLsizei, const GLvoid *);
	BEFORE(glCompressedTexSubImage2D);
	glCompressedTexSubImage2Dp(target, level, xoffset, yoffset, width, height,
			format, imageSize, data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xdddddxdp",
			target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat,
		GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
	typedef void (*methodType)(GLenum, GLint, GLenum, GLint, GLint, GLsizei,
			GLsizei, GLint);
	BEFORE(glCopyTexImage2D);
	glCopyTexImage2Dp(target, level, internalformat, x, y, width, height,
			border);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xdxddddd",
			target, level, internalformat, x, y, width, height, border);
}

void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset,
		GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLint, GLint,
			GLsizei, GLsizei);
	BEFORE(glCopyTexSubImage2D);
	glCopyTexSubImage2Dp(target, level, xoffset, yoffset, x, y, width, height);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xddddddd",
			target, level, xoffset, yoffset, x, y, width, height);
}

GLuint glCreateProgram(void) {
	typedef GLuint (*methodType)(void);
	BEFORE(glCreateProgram);
	GLuint ret = glCreateProgramp();
	error = glGetError();
	AFTER_NO_PARAM(ret, APITYPE_CONTEXT, "");

	return ret;
}

GLuint glCreateShader(GLenum shaderType) {
	typedef GLuint (*methodType)(GLenum);
	BEFORE(glCreateShader);
	GLuint ret = glCreateShaderp(shaderType);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "x", shaderType);

	return ret;
}

void glCullFace(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glCullFace);
	glCullFacep(mode);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", mode);
}

// ==================================================================
// D 14
// ==================================================================

void glDeleteBuffers(GLsizei n, const GLuint * buffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteBuffers);
	glDeleteBuffersp(n, buffers);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, buffers);
}

void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteFramebuffers);
	glDeleteFramebuffersp(n, framebuffers);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, framebuffers);
}

void glDeleteProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteProgram);
	glDeleteProgramp(program);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteRenderbuffers);
	glDeleteRenderbuffersp(n, renderbuffers);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, renderbuffers);
}

void glDeleteShader(GLuint shader) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDeleteShader);
	glDeleteShaderp(shader);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", shader);
}

void glDeleteTextures(GLsizei n, const GLuint * textures) {
	typedef void (*methodType)(GLsizei, const GLuint *);
	BEFORE(glDeleteTextures);
	glDeleteTexturesp(n, textures);
	error = glGetError();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", textures[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
				n, textures);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
				n, textures);
	}
}

void glDepthFunc(GLenum func) {
	typedef void (*methodType)(GLenum);
	BEFORE(glDepthFunc);
	glDepthFuncp(func);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", func);
}

void glDepthMask(GLboolean flag) {
	typedef void (*methodType)(GLboolean);
	BEFORE(glDepthMask);
	glDepthMaskp(flag);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", flag);
}

void glDepthRangef(GLclampf nearVal, GLclampf farVal) {
	typedef void (*methodType)(GLclampf, GLclampf);
	BEFORE(glDepthRangef);
	glDepthRangefp(nearVal, farVal);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
			nearVal, farVal);
}

void glDetachShader(GLuint program, GLuint shader) {
	typedef void (*methodType)(GLuint, GLuint);
	BEFORE(glDetachShader);
	glDetachShaderp(program, shader);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, shader);
}

void glDisable(GLenum cap) {
	typedef void (*methodType)(GLenum);
	BEFORE(glDisable);
	glDisablep(cap);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", cap);
}

void glDisableVertexAttribArray(GLuint index) {
	typedef void (*methodType)(GLuint);
	BEFORE(glDisableVertexAttribArray);
	glDisableVertexAttribArrayp(index);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
	typedef void (*methodType)(GLenum, GLint, GLsizei);
	BEFORE(glDrawArrays);
	glDrawArraysp(mode, first, count);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
			mode, first, count);
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type,
		const GLvoid * indices) {
	typedef void (*methodType)(GLenum, GLsizei, GLenum, const GLvoid *);
	BEFORE(glDrawElements);
	glDrawElementsp(mode, count, type, indices);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxp",
			mode, count, type, indices);
}

// ==================================================================
// E 2
// ==================================================================

void glEnable(GLenum cap) {
	typedef void (*methodType)(GLenum);
	BEFORE(glEnable);
	glEnablep(cap);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", cap);
}

void glEnableVertexAttribArray(GLuint index) {
	typedef void (*methodType)(GLuint);
	BEFORE(glEnableVertexAttribArray);
	glEnableVertexAttribArrayp(index);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", index);
}

#undef eglSwapBuffers
extern "C" EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
	typedef EGLBoolean (*methodType)(EGLDisplay, EGLSurface);
	BEFORE_EGL(eglSwapBuffers);
	EGLBoolean ret = eglSwapBuffersp(dpy, surface);
	error = eglGetError();
	AFTER_NO_PARAM(ret, APITYPE_CONTEXT, "");

	return ret;
}
#define eglSwapBuffers _SglSwapBuffers

EGLBoolean _SglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
	typedef EGLBoolean (*methodType)(EGLDisplay, EGLSurface);
	BEFORE_OSP_UIFW(_SglSwapBuffers);
	EGLBoolean ret = _SglSwapBuffersp(dpy, surface);
	error = eglGetError();
	AFTER_NO_PARAM(ret, APITYPE_CONTEXT, "");

	return ret;
}

// ==================================================================
// F 5
// ==================================================================

void glFinish(void) {
	typedef void (*methodType)(void);
	BEFORE(glFinish);
	glFinishp();
	error = glGetError();
	AFTER_NO_PARAM(NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFlush(void) {
	typedef void (*methodType)(void);
	BEFORE(glFlush);
	glFlushp();
	error = glGetError();
	AFTER_NO_PARAM(NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glFramebufferRenderbuffer(GLenum target, GLenum attachment,
		GLenum renderbuffertarget, GLuint renderbuffer) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint);
	BEFORE(glFramebufferRenderbuffer);
	glFramebufferRenderbufferp(target, attachment, renderbuffertarget,
			renderbuffer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxd",
			target, attachment, renderbuffertarget, renderbuffer);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget,
		GLuint texture, GLint level) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLuint, GLint);
	BEFORE(glFramebufferTexture2D);
	glFramebufferTexture2Dp(target, attachment, textarget, texture, level);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xxxdd", target, attachment, textarget, texture, level);
}

void glFrontFace(GLenum mode) {
	typedef void (*methodType)(GLenum);
	BEFORE(glFrontFace);
	glFrontFacep(mode);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", mode);
}

// ==================================================================
// G 31
// ==================================================================

void glGenBuffers(GLsizei n, GLuint * buffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenBuffers);
	glGenBuffersp(n, buffers);
	error = glGetError();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", buffers[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
			n, buffers);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, buffers);
	}
}

void glGenFramebuffers(GLsizei n, GLuint * framebuffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenFramebuffers);
	glGenFramebuffersp(n, framebuffers);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, framebuffers);
}

void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenRenderbuffers);
	glGenRenderbuffersp(n, renderbuffers);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
			n, renderbuffers);
}

void glGenTextures(GLsizei n, GLuint * textures) {
	typedef void (*methodType)(GLsizei, GLuint *);
	BEFORE(glGenTextures);
	glGenTexturesp(n, textures);
	error = glGetError();
	if (error == GL_NO_ERROR) {
		char buf[128] = "";
		int bufP = 0;
		for (int i = 0; i < n; i++) {
			bufP += sprintf(buf + bufP, "%u", textures[i]);
			if (i != n - 1) {
				bufP += sprintf(buf + bufP, ",");
			}
		}
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "dp",
				n, textures);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp",
				n, textures);
	}
}

void glGenerateMipmap(GLenum target) {
	typedef void (*methodType)(GLenum);
	BEFORE(glGenerateMipmap);
	glGenerateMipmapp(target);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", target);
}

//lsh_get
void glGetBooleanv(GLenum pname, GLboolean * params) {
	typedef void (*methodType)(GLenum, GLboolean *);
	BEFORE(glGetBooleanv);
	glGetBooleanvp(pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", pname);
}

//lsh_get
void glGetFloatv(GLenum pname, GLfloat * params) {
	typedef void (*methodType)(GLenum, GLfloat *);
	BEFORE(glGetFloatv);
	glGetFloatvp(pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", pname);
}

//lsh_get
void glGetIntegerv(GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLint *);
	BEFORE(glGetIntegerv);
	glGetIntegervp(pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", pname);
}

//lsh_get
void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize,
		GLsizei *length, GLint *size, GLenum *type, char *name) {
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
			GLenum *, char *);
	BEFORE(glGetActiveAttrib);
	glGetActiveAttribp(program, index, bufSize, length, size, type, name);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			program, index, bufSize);
}

//lsh_get
void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize,
		GLsizei *length, GLint *size, GLenum *type, char *name) {
	typedef void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLint *,
			GLenum *, char *);
	BEFORE(glGetActiveUniform);
	glGetActiveUniformp(program, index, bufSize, length, size, type, name);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			program, index, bufSize);
}

//lsh_get
void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count,
		GLuint *shaders) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, GLuint *);
	BEFORE(glGetAttachedShaders);
	glGetAttachedShadersp(program, maxCount, count, shaders);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, maxCount);
}

//lsh_get
void glGetAttribLocation(GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLint *);
	BEFORE(glGetAttribLocation);
	glGetAttribLocationp(pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", pname);
}

//lsh_get
void glGetBufferParameteriv(GLenum target, GLenum value, GLint * data) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetBufferParameteriv);
	glGetBufferParameterivp(target, value, data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			target, value);
}

GLenum glGetError(void) {
	typedef GLenum (*methodType)(void);
	BEFORE(glGetError);
	GLenum ret = glGetErrorp();
//	error = glGetError();
	AFTER_NO_PARAM(ret, APITYPE_CONTEXT, "");

	return ret;
}

//lsh_get
void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment,
		GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLint *);
	BEFORE(glGetFramebufferAttachmentParameteriv);
	glGetFramebufferAttachmentParameterivp(target, attachment, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
			target, attachment, pname);
}

//lsh_get
void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length,
		char *infoLog) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetProgramInfoLog);
	glGetProgramInfoLogp(program, maxLength, length, infoLog);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, maxLength);
}

//lsh_get
void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetProgramiv);
	glGetProgramivp(program, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
			program, pname);
}

//lsh_get
void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetRenderbufferParameteriv);
	glGetRenderbufferParameterivp(target, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			target, pname);
}

//lsh_get
void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length,
		char *infoLog) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderInfoLog);
	glGetShaderInfoLogp(shader, maxLength, length, infoLog);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			shader, maxLength);
}

//lsh_get
void glGetShaderPrecisionFormat(GLenum shaderType, GLenum precisionType,
		GLint *range, GLint *precision) {
	typedef void (*methodType)(GLenum, GLenum, GLint *, GLint *);
	BEFORE(glGetShaderPrecisionFormat);
	glGetShaderPrecisionFormatp(shaderType, precisionType, range, precision);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			shaderType, precisionType);
}

//lsh_get
void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length,
		char *source) {
	typedef void (*methodType)(GLuint, GLsizei, GLsizei *, char *);
	BEFORE(glGetShaderSource);
	glGetShaderSourcep(shader, bufSize, length, source);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			shader, bufSize);
}

//lsh_get
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetShaderiv);
	glGetShaderivp(shader, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
			shader, pname);
}

const GLubyte* glGetString(GLenum name) {
	typedef const GLubyte* (*methodType)(GLenum);
	BEFORE(glGetString);
	const GLubyte* ret = glGetStringp(name);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "x", name);

	return ret;
}

//lsh_get
void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat * params) {
	typedef void (*methodType)(GLenum, GLenum, GLfloat *);
	BEFORE(glGetTexParameterfv);
	glGetTexParameterfvp(target, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			target, pname);
}

//lsh_get
void glGetTexParameteriv(GLenum target, GLenum pname, GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, GLint *);
	BEFORE(glGetTexParameteriv);
	glGetTexParameterivp(target, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			target, pname);
}

//lsh_get
void glGetUniformfv(GLuint program, GLint location, GLfloat *params) {
	typedef void (*methodType)(GLuint, GLuint, GLfloat *);
	BEFORE(glGetUniformfv);
	glGetUniformfvp(program, location, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, location);
}

//lsh_get
void glGetUniformiv(GLuint program, GLint location, GLint *params) {
	typedef void (*methodType)(GLuint, GLuint, GLint *);
	BEFORE(glGetUniformiv);
	glGetUniformivp(program, location, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			program, location);
}

//lsh_get
GLint glGetUniformLocation(GLuint program, const char *name) {
	typedef GLint (*methodType)(GLuint, const char *);
	BEFORE(glGetUniformLocation);
	GLint ret = glGetUniformLocationp(program, name);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

//lsh_get
void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params) {
	typedef void (*methodType)(GLuint, GLenum, GLfloat *);
	BEFORE(glGetVertexAttribfv);
	glGetVertexAttribfvp(index, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
			index, pname);
}

//lsh_get
void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params) {
	typedef void (*methodType)(GLuint, GLenum, GLint *);
	BEFORE(glGetVertexAttribiv);
	glGetVertexAttribivp(index, pname, params);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
			index, pname);
}

//lsh_get
void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid **pointer) {
	typedef void (*methodType)(GLuint, GLenum, GLvoid **);
	BEFORE(glGetVertexAttribPointerv);
	glGetVertexAttribPointervp(index, pname, pointer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx",
			index, pname);
}

// ==================================================================
// H 1
// ==================================================================

void glHint(GLenum target, GLenum mode) {
	typedef void (*methodType)(GLenum, GLenum);
	BEFORE(glHint);
	glHintp(target, mode);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx",
			target, mode);
}

// ==================================================================
// I 7
// ==================================================================

GLboolean glIsBuffer(GLuint buffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsBuffer);
	GLboolean ret = glIsBufferp(buffer);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", buffer);

	return ret;
}

GLboolean glIsEnabled(GLenum cap) {
	typedef GLboolean (*methodType)(GLenum);
	BEFORE(glIsEnabled);
	GLboolean ret = glIsEnabledp(cap);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "x", cap);

	return ret;
}

GLboolean glIsFramebuffer(GLuint framebuffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsFramebuffer);
	GLboolean ret = glIsFramebufferp(framebuffer);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", framebuffer);

	return ret;
}

GLboolean glIsProgram(GLuint program) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsProgram);
	GLboolean ret = glIsProgramp(program);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", program);

	return ret;
}

GLboolean glIsRenderbuffer(GLuint renderbuffer) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsRenderbuffer);
	GLboolean ret = glIsRenderbufferp(renderbuffer);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", renderbuffer);

	return ret;
}

GLboolean glIsShader(GLuint shader) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsShader);
	GLboolean ret = glIsShaderp(shader);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", shader);

	return ret;
}

GLboolean glIsTexture(GLuint texture) {
	typedef GLboolean (*methodType)(GLuint);
	BEFORE(glIsTexture);
	GLboolean ret = glIsTexturep(texture);
	error = glGetError();
	AFTER(ret, APITYPE_CONTEXT, "", "d", texture);

	return ret;
}

// ==================================================================
// L 2
// ==================================================================

void glLineWidth(GLfloat width) {
	typedef void (*methodType)(GLfloat);
	BEFORE(glLineWidth);
	glLineWidthp(width);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "f", width);
}

void glLinkProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glLinkProgram);
	glLinkProgramp(program);
	error = glGetError();
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
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, buf, "d",
			program);
}

// ==================================================================
// P 2
// ==================================================================

void glPixelStorei(GLenum pname, GLint param) {
	typedef void (*methodType)(GLenum, GLint);
	BEFORE(glPixelStorei);
	glPixelStoreip(pname, param);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			pname, param);
}

void glPolygonOffset(GLfloat factor, GLfloat units) {
	typedef void (*methodType)(GLfloat, GLfloat);
	BEFORE(glPolygonOffset);
	glPolygonOffsetp(factor, units);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ff",
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
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"ddddxx", x, y, width, height, format, type);
}

void glReleaseShaderCompiler(void) {
	typedef void (*methodType)(void);
	BEFORE(glReleaseShaderCompiler);
	glReleaseShaderCompilerp();
	error = glGetError();
	AFTER_NO_PARAM(NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width,
		GLsizei height) {
	typedef void (*methodType)(GLenum, GLenum, GLsizei, GLsizei);
	BEFORE(glRenderbufferStorage);
	glRenderbufferStoragep(target, internalformat, width, height);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
			target, internalformat, width, height);
}

// ==================================================================
// S 10
// ==================================================================

void glSampleCoverage(GLclampf value, GLboolean invert) {
	typedef void (*methodType)(GLclampf, GLboolean);
	BEFORE(glSampleCoverage);
	glSampleCoveragep(value, invert);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "fx",
			value, invert);
}

void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);
	BEFORE(glScissor);
	glScissorp(x, y, width, height);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			x, y, width, height);
}

//lsh_param
void glShaderBinary(GLsizei n, const GLuint *shaders, GLenum binaryformat,
		const void *binary, GLsizei length) {
	typedef void (*methodType)(GLsizei, const GLuint *, GLenum, const void *,
			GLsizei);
	BEFORE(glShaderBinary);
	glShaderBinaryp(n, shaders, binaryformat, binary, length);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"dpxpd", n, shaders, binaryformat, binary, length);
}

//lsh_param
void glShaderSource(GLuint shader, GLsizei count, const char** string,
		const GLint* length) {
	typedef void (*methodType)(GLuint, GLsizei, const char**, const GLint*);
	BEFORE(glShaderSource);
	glShaderSourcep(shader, count, string, length);
	error = glGetError();
	if (error == GL_NO_ERROR) {
		GLint length[1];
		glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, length);
		char buf[length[0]];
		glGetShaderSource(shader, length[0], NULL, buf);
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, buf,
				"ddpp", shader, count, string, length);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
				"ddpp", shader, count, string, length);
	}
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
	typedef void (*methodType)(GLenum, GLint, GLint);
	BEFORE(glStencilFunc);
	glStencilFuncp(func, ref, mask);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdd",
			func, ref, mask);
}

void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
	typedef void (*methodType)(GLenum, GLenum, GLint, GLuint);
	BEFORE(glStencilFuncSeparate);
	glStencilFuncSeparatep(face, func, ref, mask);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxdd",
			face, func, ref, mask);
}

void glStencilMask(GLuint mask) {
	typedef void (*methodType)(GLuint);
	BEFORE(glStencilMask);
	glStencilMaskp(mask);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", mask);
}

void glStencilMaskSeparate(GLenum face, GLuint mask) {
	typedef void (*methodType)(GLenum, GLuint);
	BEFORE(glStencilMaskSeparate);
	glStencilMaskSeparatep(face, mask);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd",
			face, mask);
}

void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
	typedef void (*methodType)(GLenum, GLenum, GLenum);
	BEFORE(glStencilOp);
	glStencilOpp(sfail, dpfail, dppass);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
			sfail, dpfail, dppass);
}

void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail,
		GLenum dppass) {
	typedef void (*methodType)(GLenum, GLenum, GLenum, GLenum);
	BEFORE(glStencilOpSeparate);
	glStencilOpSeparatep(face, sfail, dpfail, dppass);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxx",
			face, sfail, dpfail, dppass);
}

// ==================================================================
// T 6
// ==================================================================

void glTexImage2D(GLenum target, GLint level, GLint internalformat,
		GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint,
			GLenum, GLenum, const GLvoid *);
	BEFORE(glTexImage2D);
	glTexImage2Dp(target, level, internalformat, width, height, border, format,
			type, data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xdddddxxp",
			target, level, internalformat, width, height, border, format, type, data);
}

void glTexParameterf(GLenum target, GLenum pname, GLfloat param) {
	typedef void (*methodType)(GLenum, GLenum, GLfloat);
	BEFORE(glTexParameterf);
	glTexParameterfp(target, pname, param);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
			target, pname, (GLenum)param);
}

void glTexParameterfv(GLenum target, GLenum pname, const GLfloat * params) {
	typedef void (*methodType)(GLenum, GLenum, const GLfloat *);
	BEFORE(glTexParameterfv);
	glTexParameterfvp(target, pname, params);
	error = glGetError();
	if(error == GL_NO_ERROR) {
		char param0[8];
		sprintf(param0, "%x", (GLenum)params[0]);
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, param0, "xxp",
					target, pname, params);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
			target, pname, params);
	}
}

void glTexParameteri(GLenum target, GLenum pname, GLint param) {
	typedef void (*methodType)(GLenum, GLenum, GLint);
	BEFORE(glTexParameteri);
	glTexParameterip(target, pname, param);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxx",
			target, pname, param);
}

void glTexParameteriv(GLenum target, GLenum pname, const GLint * params) {
	typedef void (*methodType)(GLenum, GLenum, const GLint *);
	BEFORE(glTexParameteriv);
	glTexParameterivp(target, pname, params);
	error = glGetError();
	if(error == GL_NO_ERROR) {
		char param0[8];
		sprintf(param0, "%x", (GLenum)params[0]);
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, param0, "xxp",
					target, pname, params);
	} else {
		AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
				target, pname, params);
	}
}

void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
		GLsizei width, GLsizei height, GLenum format, GLenum type,
		const GLvoid * data) {
	typedef void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei,
			GLenum, GLenum, const GLvoid *);
	BEFORE(glTexImage2D);
	glTexImage2Dp(target, level, xoffset, yoffset, width, height, format, type,
			data);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"xdddddxxp",
			target, level, xoffset, yoffset, width, height, format, type, data);
}

// ==================================================================
// U 20
// ==================================================================

void glUniform1f(GLint location, GLfloat v0) {
	typedef void (*methodType)(GLint, GLfloat);
	BEFORE(glUniform1f);
	glUniform1fp(location, v0);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "df",
			location, v0);
}

void glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat);
	BEFORE(glUniform2f);
	glUniform2fp(location, v0, v1);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dff",
			location, v0, v1);
}

void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform3f);
	glUniform3fp(location, v0, v1, v2);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dfff",
			location, v0, v1, v2);
}

void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2,
		GLfloat v3) {
	typedef void (*methodType)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	BEFORE(glUniform4f);
	glUniform4fp(location, v0, v1, v2, v3);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"dffff", location, v0, v1, v2, v3);
}

void glUniform1fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform1fv);
	glUniform1fvp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform2fv);
	glUniform2fvp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform3fv);
	glUniform3fvp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLfloat *);
	BEFORE(glUniform4fv);
	glUniform4fvp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform1i(GLint location, GLint v0) {
	typedef void (*methodType)(GLint, GLint);
	BEFORE(glUniform1i);
	glUniform1ip(location, v0);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
			location, v0);
}

void glUniform2i(GLint location, GLint v0, GLint v1) {
	typedef void (*methodType)(GLint, GLint, GLint);
	BEFORE(glUniform2i);
	glUniform2ip(location, v0, v1);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd",
			location, v0, v1);
}

void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2) {
	typedef void (*methodType)(GLint, GLint, GLint, GLint);
	BEFORE(glUniform3i);
	glUniform3ip(location, v0, v1, v2);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			location, v0, v1, v2);
}

void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) {
	typedef void (*methodType)(GLint, GLint, GLint, GLint, GLint);
	BEFORE(glUniform4i);
	glUniform4ip(location, v0, v1, v2, v3);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"ddddd", location, v0, v1, v2, v3);
}

void glUniform1iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform1iv);
	glUniform1ivp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform2iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform2iv);
	glUniform2ivp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniform3iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform3iv);
	glUniform3ivp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix2fv);
	glUniformMatrix2fvp(location, count, transpose, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxp",
			location, count, transpose, value);
}

void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix3fv);
	glUniformMatrix3fvp(location, count, transpose, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxp",
			location, count, transpose, value);
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose,
		const GLfloat *value) {
	typedef void (*methodType)(GLint, GLsizei, GLboolean, const GLfloat *);
	BEFORE(glUniformMatrix4fv);
	glUniformMatrix4fvp(location, count, transpose, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxp",
			location, count, transpose, value);
}

void glUniform4iv(GLint location, GLsizei count, const GLint *value) {
	typedef void (*methodType)(GLint, GLsizei, const GLint *);
	BEFORE(glUniform4iv);
	glUniform4ivp(location, count, value);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp",
			location, count, value);
}

void glUseProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glUseProgram);
	glUseProgramp(program);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

// ==================================================================
// V 7
// ==================================================================

void glValidateProgram(GLuint program) {
	typedef void (*methodType)(GLuint);
	BEFORE(glValidateProgram);
	glValidateProgramp(program);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d",
			program);
}

void glVertexAttrib1f(GLuint index, GLfloat v0) {
	typedef void (*methodType)(GLuint, GLfloat);
	BEFORE(glVertexAttrib1f);
	glVertexAttrib1fp(index, v0);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"df", index, v0);
}

void glVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1) {
	typedef void (*methodType)(GLuint, GLfloat, GLfloat);
	BEFORE(glVertexAttrib2f);
	glVertexAttrib2fp(index, v0, v1);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dff", index, v0, v1);
}

void glVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2) {
	typedef void (*methodType)(GLuint, GLfloat, GLfloat, GLfloat);
	BEFORE(glVertexAttrib3f);
	glVertexAttrib3fp(index, v0, v1, v2);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
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

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dffff", index, v0, v1, v2, v3);
}

void glVertexAttrib1fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib1fv);
	glVertexAttrib1fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dp", index, v);
}

void glVertexAttrib2fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib2fv);
	glVertexAttrib2fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dp", index, v);
}

void glVertexAttrib3fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib3fv);
	glVertexAttrib3fvp(index, v);

	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dp", index, v);
}

void glVertexAttrib4fv(GLuint index, const GLfloat *v) {
	typedef void (*methodType)(GLuint, const GLfloat *);
	BEFORE(glVertexAttrib4fv);
	glVertexAttrib4fvp(index, v);
	GLfloat cv[4];
	glGetVertexAttribfv(index, GL_CURRENT_VERTEX_ATTRIB, cv);
	sprintf(contextValue, "%f,%f,%f,%f", cv[0], cv[1], cv[2], cv[3]);

	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, contextValue,
			"dp", index, v);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
		GLboolean normalized, GLsizei stride, const GLvoid * pointer) {
	typedef void (*methodType)(GLuint, GLint, GLenum, GLboolean, GLsizei,
			const GLvoid *);
	BEFORE(glVertexAttribPointer);
	glVertexAttribPointerp(index, size, type, normalized, stride, pointer);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "",
			"ddxxdp",
			index, size, type, normalized, stride, pointer);
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
	typedef void (*methodType)(GLint, GLint, GLsizei, GLsizei);
	BEFORE(glViewport);
	glViewportp(x, y, width, height);
	error = glGetError();
	AFTER(NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd",
			x, y, width, height);
}
