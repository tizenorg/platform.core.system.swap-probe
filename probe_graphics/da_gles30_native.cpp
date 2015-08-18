#include "da_gles20.h"
#include "daprobe.h"
#include "binproto.h"
#include "real_functions.h"
#include "common_probe_init.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef REAL_NAME
	#define REAL_NAME(func) func
#endif

#ifndef CALL_ORIG
	#define CALL_ORIG(func, ...) /* CALL_ORIG */ func##p(__VA_ARGS__)
#endif

#ifndef BEFORE
	#define BEFORE BEFORE_GL3_ORIG
#endif

#ifndef DECLAR
	#define DECLAR FUNC_DECLAR
#endif

#ifndef DECLAR_NOARGS
	#define DECLAR_NOARGS FUNC_DECLAR_NOARGS
#endif

#ifndef TYPEDEF
	#define TYPEDEF(type) typedef type
#endif

static __thread int is_gl_error_external = 1;
//static enum DaOptions _sopt = OPT_GLES;
extern GLenum glGetError();

DECLAR(void, glVertexAttribI1i, GLuint index, GLint x)
{
	TYPEDEF(void (*methodType)(GLuint, GLint));
	BEFORE(glVertexAttribI1i);
	CALL_ORIG(glVertexAttribI1i, index, x);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", index, x);
}

DECLAR(void, glVertexAttribI2i, GLuint index, GLint x, GLint y)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint));
	BEFORE(glVertexAttribI2i);
	CALL_ORIG(glVertexAttribI2i, index, x, y);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd", index, x, y);
}

DECLAR(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint));
	BEFORE(glVertexAttribI3i);
	CALL_ORIG(glVertexAttribI3i, index, x, y, z);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", index, x,
	      y, z);
}

DECLAR(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z,
       GLint w)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint, GLint));
	BEFORE(glVertexAttribI4i);
	CALL_ORIG(glVertexAttribI4i, index, x, y, z, w);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddddd", index, x,
	      y, z, w);
}

DECLAR(void, glVertexAttribI1ui, GLuint index, GLuint x)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glVertexAttribI1ui);
	CALL_ORIG(glVertexAttribI1ui, index, x);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", index, x);
}

DECLAR(void, glUniformBlockBinding, GLuint program, GLuint UniformBlockIndex,
       GLuint uniformBlockBinding)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint));
	BEFORE(glUniformBlockBinding);
	CALL_ORIG(glUniformBlockBinding, program, UniformBlockIndex,
		  uniformBlockBinding);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd", program,
	      UniformBlockIndex, uniformBlockBinding);
}

DECLAR(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI2ui);
	CALL_ORIG(glVertexAttribI2ui, index, x, y);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd", index, x, y);
}

DECLAR(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI3ui);
	CALL_ORIG(glVertexAttribI3ui, index, x, y, z);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", index, x, y,
	      z);
}


DECLAR(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z,
       GLuint w)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI4ui);
	CALL_ORIG(glVertexAttribI4ui, index, x, y, z, w);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddddd", index, x,
	      y, z, w);
}

DECLAR(void, glVertexAttribI1iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI1iv);
	CALL_ORIG(glVertexAttribI1iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI2iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI2iv);
	CALL_ORIG(glVertexAttribI2iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI3iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI3iv);
	CALL_ORIG(glVertexAttribI3iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI4iv);
	CALL_ORIG(glVertexAttribI4iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI1uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI1uiv);
	CALL_ORIG(glVertexAttribI1uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI2uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI2uiv);
	CALL_ORIG(glVertexAttribI2uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLuint *));
	BEFORE(glGetQueryObjectuiv);
	CALL_ORIG(glGetQueryObjectuiv, id, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", id, pname,
	      params);
}

DECLAR(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount,
       const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const GLuint *, GLenum,
		GLint *));
	BEFORE(glGetActiveUniformsiv);
	CALL_ORIG(glGetActiveUniformsiv, program, uniformCount,uniformIndices,
		  pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddxp", program,
	      uniformCount, uniformIndices, pname, params);
}

DECLAR(void, glVertexAttribI3uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI3uiv);
	CALL_ORIG(glVertexAttribI3uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI4uiv);
	CALL_ORIG(glVertexAttribI4uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4bv, GLuint index, const GLbyte *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLbyte*));
	BEFORE(glVertexAttribI4bv);
	CALL_ORIG(glVertexAttribI4bv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4sv, GLuint index, const GLshort *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLshort*));
	BEFORE(glVertexAttribI4sv);
	CALL_ORIG(glVertexAttribI4sv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLubyte*));
	BEFORE(glVertexAttribI4ubv);
	CALL_ORIG(glVertexAttribI4ubv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribI4usv, GLuint index, const GLushort *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLushort*));
	BEFORE(glVertexAttribI4usv);
	CALL_ORIG(glVertexAttribI4usv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", index, v);
}

DECLAR(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type,
       GLsizei stride, const GLvoid *pointer)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLenum, GLsizei, const
		GLvoid *));
	BEFORE(glVertexAttribIPointer);
	CALL_ORIG(glVertexAttribIPointer, index, size, type, stride, pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxdp", index,
	      size, type, stride, pointer);
}

DECLAR(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint*));
	BEFORE(glGetVertexAttribIiv);
	CALL_ORIG(glGetVertexAttribIiv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", index,
	      pname, params);
}

DECLAR(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLuint*));
	BEFORE(glGetVertexAttribIuiv);
	CALL_ORIG(glGetVertexAttribIuiv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", index, pname,
	      params);
}

DECLAR(void, glUniform1ui, GLint location, GLuint v0)
{
	TYPEDEF(void (*methodType)(GLint, GLuint));
	BEFORE(glUniform1ui);
	CALL_ORIG(glUniform1ui, location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", location, v0);
}

DECLAR(void, glUniform2ui, GLint location, GLuint v0, GLuint v1)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint));
	BEFORE(glUniform2ui);
	CALL_ORIG(glUniform2ui, location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddd", location,
	      v0, v1);
}

DECLAR(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint, GLuint));
	BEFORE(glUniform3ui);
	CALL_ORIG(glUniform3ui, location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", location,
	      v0, v1, v2);
}

DECLAR(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2,
       GLuint v3)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint, GLuint, GLuint));
	BEFORE(glUniform4ui);
	CALL_ORIG(glUniform4ui, location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddddd", location,
	      v0, v1, v2, v3);
}

DECLAR(void, glUniform2uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform2uiv);
	CALL_ORIG(glUniform2uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", location,
	      count, value);
}

DECLAR(void, glUniform3uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform3uiv);
	CALL_ORIG(glUniform3uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", location,
	      count, value);
}

DECLAR(void, glUniform1uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform1uiv);
	CALL_ORIG(glUniform1uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", location,
	      count, value);
}

DECLAR(void, glUniform4uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform4uiv);
	CALL_ORIG(glUniform4uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", location,
	      count, value);
}

DECLAR(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLuint*));
	BEFORE(glGetUniformuiv);
	CALL_ORIG(glGetUniformuiv, program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", program,
	      location, params);
}

DECLAR(void, glBindFragDataLocation, GLuint program, GLuint colorNumber,
       const GLchar *name)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, const GLchar*));
	BEFORE(glBindFragDataLocation);
	CALL_ORIG(glBindFragDataLocation, program, colorNumber, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddp", program,
	      colorNumber, name);
}

DECLAR(void, glGetFragDataLocation, GLuint program, const GLchar *name)
{
	TYPEDEF(void (*methodType)(GLuint, const GLchar*));
	BEFORE(glGetFragDataLocation);
	CALL_ORIG(glGetFragDataLocation, program, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ds", program,
	      name);
}

DECLAR(void, glBeginConditionalRenderNV, GLuint id, GLenum mode)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum));
	BEFORE(glBeginConditionalRenderNV);
	CALL_ORIG(glBeginConditionalRenderNV, id, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dx", id, mode);
}

DECLAR_NOARGS(void, glEndConditionalRenderNV)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glEndConditionalRenderNV);
	CALL_ORIG(glEndConditionalRenderNV);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

DECLAR(void, glClampColorARB, GLenum target, GLenum clamp)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum));
	BEFORE(glClampColorARB);
	CALL_ORIG(glClampColorARB, target, clamp);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xx", target,
	      clamp);
}

DECLAR(void, glRenderbufferStorageMultisample, GLenum target, GLsizei
       samplers, GLenum internalformat, GLsizei width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei,
		GLsizei));
	BEFORE(glRenderbufferStorageMultisample);
	CALL_ORIG(glRenderbufferStorageMultisample, target, samplers,
		  internalformat, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxdd", samplers,
	      internalformat, width, height);
}

DECLAR(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1,
       GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint, GLint, GLint,
		GLint, GLint));
	BEFORE(glBlitFramebuffer);
	CALL_ORIG(glBlitFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,
	          dstX1, dstY1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddddddd", srcX0,
	      srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1);
}

DECLAR(void, glClearColorIi, GLint r, GLint g, GLint b, GLint a)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint));
	BEFORE(glClearColorIi);
	CALL_ORIG(glClearColorIi, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", r, g, b, a);
}

DECLAR(void, glClearColorIui, GLint r, GLint g, GLint b, GLint a)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint));
	BEFORE(glClearColorIui);
	CALL_ORIG(glClearColorIui, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", r, g, b, a);
}

DECLAR(void, glTexParameterIiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glTexParameterIiv);
	CALL_ORIG(glTexParameterIiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp", target, pname,
	      params);
}

DECLAR(void, glTexParameterIuiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glTexParameterIuiv);
	CALL_ORIG(glTexParameterIuiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp", target, pname,
	      params);
}

DECLAR(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glGetTexParameterIiv);
	CALL_ORIG(glGetTexParameterIiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp", target, pname,
	      params);
}

DECLAR(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glGetTexParameterIuiv);
	CALL_ORIG(glGetTexParameterIuiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp", target, pname,
	      params);
}

DECLAR(void, glFramebufferTureLayer, GLenum target, GLenum attachment, GLuint
       ture, GLint level, GLint layer)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLuint, GLint, GLint));
	BEFORE(glFramebufferTureLayer);
	CALL_ORIG(glFramebufferTureLayer, target, attachment, ture, level,
		  layer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxddd", target,
	      attachment, ture, layer);
}

DECLAR(void, glColorMaskIndexed, GLuint buf, GLboolean r, GLboolean g,
       GLboolean b, GLboolean a)
{
	TYPEDEF(void (*methodType)(GLuint, GLboolean, GLboolean, GLboolean,
		GLboolean));
	BEFORE(glColorMaskIndexed);
	CALL_ORIG(glColorMaskIndexed, buf, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddd", buf, r, g,
	      b, a);
}

DECLAR(void, glGetBooleanIndexedv, GLenum value, GLint index, GLboolean *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLboolean*));
	BEFORE(glGetBooleanIndexedv);
	CALL_ORIG(glGetBooleanIndexedv, value, index, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdp", value, index,
	      data);
}

DECLAR(void, glGetIntegerIndexedv, GLenum value, GLint index, GLboolean *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLboolean*));
	BEFORE(glGetIntegerIndexedv);
	CALL_ORIG(glGetIntegerIndexedv, value, index, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdp", value, index,
	      data);
}

DECLAR(void, glEnableIndexed, GLenum value, GLint index)
{
	TYPEDEF(void (*methodType)(GLenum, GLint));
	BEFORE(glEnableIndexed);
	CALL_ORIG(glEnableIndexed, value, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd", value, index);
}

DECLAR(void, glDisableIndexed, GLenum value, GLint index)
{
	TYPEDEF(void (*methodType)(GLenum, GLint));
	BEFORE(glDisableIndexed);
	CALL_ORIG(glDisableIndexed, value, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd", value, index);
}

DECLAR(GLboolean, glIsEnabledIndexed, GLenum target, GLuint index)
{
	TYPEDEF(GLboolean (*methodType)(GLenum, GLuint));
	BEFORE(glIsEnabledIndexed);
	GLboolean ret = CALL_ORIG(glIsEnabledIndexed, target, index);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "xd", target, index);

	return ret;
}

DECLAR(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer,
       GLintptr offset, GLsizeiptr size)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLintptr,
		GLsizeiptr));
	BEFORE(glBindBufferRange);
	CALL_ORIG(glBindBufferRange, target, index, buffer, offset, size);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddpp", target,
	      index, buffer, offset, size);
}

DECLAR(void, glBindBufferOffset, GLenum target, GLuint index, GLuint buffer,
       GLintptr offset)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLintptr));
	BEFORE(glBindBufferOffset);
	CALL_ORIG(glBindBufferOffset, target, index, buffer, offset);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddp", target,
	      index, buffer, offset);
}

DECLAR(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint));
	BEFORE(glBindBufferBase);
	CALL_ORIG(glBindBufferBase, target, index, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddp", target,
	      index, buffer);
}

DECLAR(void, glBeginTransformFeedback, GLenum primitiveMode)
{
	TYPEDEF(void (*methodType)(GLenum));
	BEFORE(glBeginTransformFeedback);
	CALL_ORIG(glBeginTransformFeedback, primitiveMode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x", primitiveMode);
}

DECLAR_NOARGS(void, glEndTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glEndTransformFeedback);
	CALL_ORIG(glEndTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

DECLAR(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const
       char **varyings, GLenum bufferMode)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const char **, GLenum));
	BEFORE(glTransformFeedbackVaryings);
	CALL_ORIG(glTransformFeedbackVaryings, program, count, varyings,
		  bufferMode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddpx", program,
	      count, varyings, bufferMode);
}

DECLAR(void, glGetTransformFeedbackVarying, GLuint program, GLuint index,
       GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, char
       *name)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei
		*, GLenum *, char *));
	BEFORE(glGetTransformFeedbackVarying);
	CALL_ORIG(glGetTransformFeedbackVarying, program, index, bufSize,
		  length, size, type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddppps", program,
	      index, bufSize, length, size, type, name);
}

DECLAR(void, glFlushMappedBufferRange, GLenum target, GLintptr offset,
       GLsizeiptr length)
{
	TYPEDEF(void (*methodType)(GLenum, GLintptr, GLsizeiptr));
	BEFORE(glFlushMappedBufferRange);
	CALL_ORIG(glFlushMappedBufferRange, target, offset, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xpp", target,
	      offset, length );
}

DECLAR(GLboolean, glUnmapBuffer, GLenum target)
{
	TYPEDEF(GLboolean (*methodType)(GLenum));
	BEFORE(glUnmapBuffer);
	GLboolean ret = CALL_ORIG(glUnmapBuffer, target);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "x", target);

	return ret;
}

DECLAR(void, glCopyBufferSubData, GLenum readtarget,GLenum writetarget,
       GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLintptr, GLintptr,
		GLsizeiptr));
	BEFORE(glCopyBufferSubData);
	CALL_ORIG(glCopyBufferSubData, readtarget, writetarget, readoffset,
		  writeoffset, size);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxppp", readtarget,
	      writetarget, readoffset, writeoffset, size);
}

DECLAR(void, glGenVertexArrays, GLsizei n, GLuint *arrays)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenVertexArrays);
	CALL_ORIG(glGenVertexArrays, n, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xp", n, arrays);
}

DECLAR(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteVertexArrays);
	CALL_ORIG(glDeleteVertexArrays, n, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xp", n, arrays);
}

DECLAR(void, glBindVertexArray, GLuint arrays)
{
	TYPEDEF(void (*methodType)(GLuint));
	BEFORE(glBindVertexArray);
	CALL_ORIG(glBindVertexArray, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", arrays);
}

DECLAR(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei
       *length, GLenum *binaryFormat, void *binary)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, GLsizei *, GLenum *,
		void*));
	BEFORE(glGetProgramBinary);
	CALL_ORIG(glGetProgramBinary, program, bufSize, length, binaryFormat,
		  binary);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddppp", program,
	      bufSize, length, binaryFormat, binary);
}

DECLAR(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void
       *binary, GLsizei length)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const void *, GLsizei));
	BEFORE(glProgramBinary);
	CALL_ORIG(glProgramBinary, program, binaryFormat, binary, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", program,
	      binaryFormat, binary, length);
}

DECLAR(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar
       *uniformBlockName)
{
	TYPEDEF(GLuint (*methodType)(GLuint, const GLchar *));
	BEFORE(glGetUniformBlockIndex);
	GLint ret = CALL_ORIG(glGetUniformBlockIndex, program,
			      uniformBlockName);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "dp", program, uniformBlockName);

	return ret;
}

DECLAR(void, glGetActiveUniformBlockName, GLuint program, GLuint
       UniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar
       *uniformBlockName)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLchar
		*));
	BEFORE(glGetActiveUniformBlockName);
	CALL_ORIG(glGetActiveUniformBlockName, program, UniformBlockIndex,
	          bufSize, length, uniformBlockName);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxp", program,
	      UniformBlockIndex, bufSize, length, uniformBlockName);
}

DECLAR(void, glGetActiveUniformBlockiv, GLuint program, GLuint
       UniformBlockIndex, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLenum, GLint *));
	BEFORE(glGetActiveUniformBlockiv);
	CALL_ORIG(glGetActiveUniformBlockiv, program, UniformBlockIndex,
	          pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxp", program,
	      UniformBlockIndex, pname, params);
}

DECLAR(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const
       GLchar **uniformsNames, GLuint *uniformIndices)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const GLchar **, GLuint
		*));
	BEFORE(glGetUniformIndices);
	CALL_ORIG(glGetUniformIndices, program, uniformCount, uniformsNames,
	          uniformIndices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "ddxpp", program,
	      uniformCount, uniformsNames, uniformIndices);
}

DECLAR(void, glGenQueries, GLsizei n, GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenQueries);
	CALL_ORIG(glGenQueries, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, ids);
}

DECLAR(void, glBeginQuery, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glBeginQuery);
	CALL_ORIG(glBeginQuery, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd", target, id);
}

DECLAR(void, glEndQuery, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glEndQuery);
	CALL_ORIG(glEndQuery, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd", target, id);
}

DECLAR(void, glDeleteQueries, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteQueries);
	CALL_ORIG(glDeleteQueries, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, ids);
}

DECLAR(void, glGenTransformFeedbacks, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glGenTransformFeedbacks);
	CALL_ORIG(glGenTransformFeedbacks, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, ids);
}

DECLAR(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteTransformFeedbacks);
	CALL_ORIG(glDeleteTransformFeedbacks, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, ids);
}

DECLAR_NOARGS(void, glPauseTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glPauseTransformFeedback);
	CALL_ORIG(glPauseTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

DECLAR_NOARGS(void, glResumeTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glResumeTransformFeedback);
	CALL_ORIG(glResumeTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "");
}

DECLAR(void, glGenSamplers, GLsizei n, GLuint *samplers)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenSamplers);
	CALL_ORIG(glGenSamplers, n, samplers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, samplers);
}

DECLAR(void, glBindSampler, GLuint unit, GLuint sampler)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glBindSampler);
	CALL_ORIG(glBindSampler, unit, sampler);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", unit, sampler);
}

DECLAR(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLfloat));
	BEFORE(glSamplerParameterf);
	CALL_ORIG(glSamplerParameterf, sampler, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxd", sampler,
	      pname, param);
}

DECLAR(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint));
	BEFORE(glSamplerParameteri);
	CALL_ORIG(glSamplerParameteri, sampler, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxd", sampler,
	      pname, param);
}

DECLAR(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLfloat *));
	BEFORE(glSamplerParameterfv);
	CALL_ORIG(glSamplerParameterfv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", sampler,
	      pname, params);
}

DECLAR(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLint *));
	BEFORE(glSamplerParameteriv);
	CALL_ORIG(glSamplerParameteriv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", sampler,
	      pname, params);
}

DECLAR(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLint *));
	BEFORE(glSamplerParameterIiv);
	CALL_ORIG(glSamplerParameterIiv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", sampler,
	      pname, params);
}

DECLAR(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const
       GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLuint *));
	BEFORE(glSamplerParameterIuiv);
	CALL_ORIG(glSamplerParameterIuiv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxp", sampler,
	      pname, params);
}

DECLAR(void, glDeleteSamplers, GLsizei n, const GLuint *samplers)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteSamplers);
	CALL_ORIG(glDeleteSamplers, n, samplers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, samplers);
}

DECLAR(void, glTexImage3D, GLenum target, GLint level, GLint internalFormat,
       GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum
       format, GLenum type, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLsizei, GLsizei,
		GLsizei, GLint, GLenum, GLenum, const GLvoid *));
	BEFORE(glTexImage3D);
	CALL_ORIG(glTexImage3D, target, level, internalFormat, width,
		  height, depth, border, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddddddxxp", target,
	      level, internalFormat, width, height, depth, border, format,
	      type, data);
}

DECLAR(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum
       internalformat, GLsizei width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei, GLsizei));
	BEFORE(glTexStorage2D);
	CALL_ORIG(glTexStorage2D, target, levels, internalformat, width,
		  height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxdd", target,
	      levels, internalformat, width, height);
}

DECLAR(void, glTexSubImage3D, GLint level, GLint xoffset, GLint yoffset, GLint
       zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
       GLenum type, const GLvoid *pixels)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint, GLsizei,
		GLsizei, GLsizei, GLenum, GLenum, const GLvoid *));
	BEFORE(glTexSubImage3D);
	CALL_ORIG(glTexSubImage3D, level, xoffset, yoffset, zoffset, width,
		  height, depth, format, type, pixels);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddddddxxp", level,
	      xoffset, yoffset, zoffset, width, height, depth, format, type,
	      pixels);
}

DECLAR(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum
       internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint
       border, GLsizei imageSize, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLenum, GLsizei, GLsizei,
		GLsizei, GLint, GLsizei, const GLvoid *));
	BEFORE(glCompressedTexImage3D);
	CALL_ORIG(glCompressedTexImage3D, target, level, internalformat,
		  width, height, depth, border, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxdddddp", target,
	      level, internalformat, width, height, depth, border,
	      imageSize, data);
}

DECLAR(void, glCompressedTexSubImage3D, GLenum target, GLint xoffset, GLint
       yoffset, GLint zoffset, GLsizei width, GLint height, GLint depth,
       GLenum format, GLsizei imageSize, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei,
		GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *));
	BEFORE(glCompressedTexSubImage3D);
	CALL_ORIG(glCompressedTexSubImage3D, target, xoffset, yoffset,
		  zoffset, width, height, depth, format, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddddxdp",
	      target, xoffset, yoffset, zoffset, width, height, depth,
	      format, imageSize, data);
}

DECLAR(void, glFramebufferTextureLayer, GLenum target, GLenum attachment,
       GLuint texture, GLint level, GLint layer)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLuint, GLint, GLint));
	BEFORE(glFramebufferTextureLayer);
	CALL_ORIG(glFramebufferTextureLayer, target, attachment, texture,
		  level, layer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxddd", target,
	      attachment, texture, level, layer);
}

DECLAR(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments,
       const GLenum *attachments)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, const GLenum *));
	BEFORE(glInvalidateFramebuffer);
	CALL_ORIG(glInvalidateFramebuffer, target, numAttachments,
		  attachments);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdp", target,
	      numAttachments, attachments);

}

DECLAR(void, glInvalidateSubFramebuffer, GLenum target, GLsizei
       numAttachments, const GLenum *attachments, GLint x, GLint y, GLint
       width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, const GLenum *, GLint,
		GLint, GLsizei, GLsizei));
	BEFORE(glInvalidateSubFramebuffer);
	CALL_ORIG(glInvalidateSubFramebuffer, target, numAttachments,
		  attachments, x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdpdddd", target,
	      numAttachments, attachments, x, y, width, height);
}

DECLAR(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint
       xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
       GLsizei depth)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint, GLint,
		GLsizei, GLsizei, GLsizei));
	BEFORE(glInvalidateTexSubImage);
	CALL_ORIG(glInvalidateTexSubImage, texture, level, xoffset, yoffset,
		  zoffset, width, height, depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dddddddd", texture,
	      level, xoffset, yoffset, zoffset, width, height, depth);
}

DECLAR(void, glInvalidateTexImage, GLuint texture, GLint level)
{
	TYPEDEF(void (*methodType)(GLuint, GLint));
	BEFORE(glInvalidateTexImage);
	CALL_ORIG(glInvalidateTexImage, texture, level);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd", texture, level);
}

DECLAR(void, glInvalidateBufferData, GLuint buffer)
{
	TYPEDEF(void (*methodType)(GLuint));
	BEFORE(glInvalidateBufferData);
	CALL_ORIG(glInvalidateBufferData, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "d", buffer);
}

DECLAR(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset,
       GLsizeiptr length)
{
	TYPEDEF(void (*methodType)(GLuint, GLintptr, GLsizeiptr));
	BEFORE(glInvalidateBufferSubData);
	CALL_ORIG(glInvalidateBufferSubData, buffer, offset, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxx", buffer,
	      offset, length);
}

DECLAR(GLboolean, glIsVertexArray, GLuint array)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsVertexArray);
	GLboolean ret = CALL_ORIG(glIsVertexArray, array);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", array);

	return ret;
}

DECLAR(GLboolean, glIsTransformFeedback, GLuint id)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsTransformFeedback);
	GLboolean ret = CALL_ORIG(glIsTransformFeedback, id);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d", id);

	return ret;
}

/* TODO search real definition */
#ifndef ARB_sync
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
#endif

//==========================================================================
DECLAR(void, glBindTransformFeedback, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glBindTransformFeedback);
	CALL_ORIG(glBindTransformFeedback, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xd", target, id);
}

DECLAR(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags,
       GLuint64 timeout)
{
	TYPEDEF(GLenum (*methodType)(GLsync, GLbitfield, GLuint64));
	BEFORE(glClientWaitSync);
	GLenum ret = CALL_ORIG(glClientWaitSync, sync, flags, timeout);
	GL_GET_ERROR();
	AFTER('x', ret, APITYPE_CONTEXT, "", "pxx",
	      sync, flags, timeout);

	return ret;
}

DECLAR(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset,
       GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width,
       GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLint, GLint , GLint,
				   GLint, GLsizei, GLsizei));
	BEFORE(glCopyTexSubImage3D);
	CALL_ORIG(glCopyTexSubImage3D, target, level, xoffset, yoffset, zoffset,
		  x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddddddd",
	      target, level, xoffset, yoffset, zoffset, x, y, width, height);
}

DECLAR(void, glDeleteSync, GLsync sync)
{
	TYPEDEF(void (*methodType)(GLsync));
	BEFORE(glDeleteSync);
	CALL_ORIG(glDeleteSync, sync);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "p", sync);
}

DECLAR(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count,
       GLsizei primcount)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLsizei, GLsizei));
	BEFORE(glDrawArraysInstanced);
	CALL_ORIG(glDrawArraysInstanced, mode, first, count, primcount);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xddd",  mode, first,
	      count, primcount);
}

DECLAR(void, glDrawBuffers, GLsizei n, const GLenum *bufs)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLenum *));
	BEFORE(glDrawBuffers);
	CALL_ORIG(glDrawBuffers, n, bufs);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dp", n, bufs);
}

DECLAR(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type,
       const void * indices, GLsizei primcount)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, const void *,
				   GLsizei));
	BEFORE(glDrawElementsInstanced);
	CALL_ORIG(glDrawElementsInstanced, mode, count, type, indices,
		  primcount);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxpd",
	      mode, count, type, indices, primcount);
}

DECLAR(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end,
       GLsizei count, GLenum type, const GLvoid * indices)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLsizei, GLenum,
		const GLvoid *));
	BEFORE(glDrawRangeElements);
	CALL_ORIG(glDrawRangeElements, mode, start, end, count, type, indices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdddxp",
	      mode, start, end, count, type, indices);
}

DECLAR(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	TYPEDEF(void (*methodType)(GLsync, GLbitfield, GLuint64));
	BEFORE(glWaitSync);
	CALL_ORIG(glWaitSync, sync, flags, timeout);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "pxx",
	      sync, flags, timeout);
}

DECLAR(void, glVertexAttribDivisor, GLuint index, GLuint divisor)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glVertexAttribDivisor);
	CALL_ORIG(glVertexAttribDivisor, index, divisor);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dd",
	      index, divisor);
}

DECLAR(void, glTexStorage3D, GLenum target, GLsizei levels,
       GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei, GLsizei,
		GLsizei));
	BEFORE(glTexStorage3D);
	CALL_ORIG(glTexStorage3D, target, levels, internalformat, width, height,
		  depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xdxddd",
	      target, levels, internalformat, width, height, depth);
}

DECLAR(void, glReadBuffer, GLenum src)
{
	TYPEDEF(void (*methodType)(GLenum));
	BEFORE(glReadBuffer);
	CALL_ORIG(glReadBuffer, src);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "x",
	      src);
}

DECLAR(void, glProgramParameteri, GLuint program, GLenum pname, GLint value)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint));
	BEFORE(glProgramParameteri);
	CALL_ORIG(glProgramParameteri, program, pname, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "dxd",
	      program, pname, value);
}

DECLAR(void *, glMapBufferRange, GLenum target, GLintptr offset,
       GLsizeiptr length, GLbitfield access)
{
	TYPEDEF(void *(*methodType)(GLenum, GLintptr, GLsizeiptr, GLbitfield));
	BEFORE(glMapBufferRange);
	void *ret = CALL_ORIG(glMapBufferRange, target, offset, length, access);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xpdx",
	      target, offset, length, access);

	return ret;
}

DECLAR(GLboolean, glIsSync, GLsync sync)
{
	TYPEDEF(GLboolean (*methodType)(GLsync));
	BEFORE(glIsSync);
	GLboolean ret = CALL_ORIG(glIsSync, sync);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "p",
	      sync);

	return ret;
}

DECLAR(GLboolean, glIsQuery, GLuint id)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsQuery);
	GLboolean ret = CALL_ORIG(glIsQuery, id);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, "", "d",
	      id);

	return ret;
}

DECLAR(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize,
       GLsizei *length, GLint *values)
{
	TYPEDEF(void (*methodType)(GLsync, GLenum, GLsizei, GLsizei *, GLint *));
	BEFORE(glGetSynciv);
	CALL_ORIG(glGetSynciv, sync, pname, bufSize, length, values);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "pxdpp",
	      sync, pname, bufSize, length, values);
}

DECLAR(void, glGetQueryiv, GLenum target, GLenum pname, GLint * params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint *));
	BEFORE(glGetQueryiv);
	CALL_ORIG(glGetQueryiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
	      target, pname, params);
}

DECLAR(void, glGetInternalformativ, GLenum target, GLenum internalformat,
       GLenum pname, GLsizei bufSize, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLenum, GLsizei, GLint *));
	BEFORE(glGetInternalformativ);
	CALL_ORIG(glGetInternalformativ, target, internalformat, pname, bufSize,
		  params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxxdp",
	      target, internalformat, pname, bufSize, params);
}

DECLAR(void, glGetBufferPointerv, GLenum target, GLenum pname,
       GLvoid ** params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLvoid **));
	BEFORE(glGetBufferPointerv);
	CALL_ORIG(glGetBufferPointerv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, "", "xxp",
	      target, pname, params);
}

DECLAR(GLsync, glFenceSync, GLenum condition, GLbitfield flags)
{
	TYPEDEF(GLsync (*methodType)(GLenum, GLbitfield));
	BEFORE(glFenceSync);
	GLsync ret = CALL_ORIG(glFenceSync, condition, flags);
	GL_GET_ERROR();
	AFTER('x', ret, APITYPE_CONTEXT, "", "xx",
	      condition, flags);

	return ret;
}

#undef CALL_ORIG
#undef BEFORE

#ifdef __cplusplus
 } /* extern "C" */
#endif
