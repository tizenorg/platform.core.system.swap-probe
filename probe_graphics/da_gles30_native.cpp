#ifdef __cplusplus
 extern "C" {
#endif

#include "da_gles20.h"

#define DECLARE FUNC_DECLAR
#define DECLARE_NOARGS FUNC_DECLAR_NOARGS

#define CALL_ORIG(func, ...) /* CALL_ORIG */ func##p(__VA_ARGS__)
#define BEFORE BEFORE_GL3_ORIG
#define TYPEDEF(type) typedef type

#include "daprobe.h"
#include "binproto.h"
#include "real_functions.h"
#include "common_probe_init.h"

/* TODO search real definition */
#ifndef ARB_sync
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
#endif


static __thread int is_gl_error_external = 1;
//static enum DaOptions _sopt = OPT_GLES;
extern GLenum PROBE_NAME(glGetError)();

DECLARE(void, glVertexAttribI1i, GLuint index, GLint x)
{
	TYPEDEF(void (*methodType)(GLuint, GLint));
	BEFORE(glVertexAttribI1i);
	CALL_ORIG(glVertexAttribI1i, index, x);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd", index, x);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI1i), GLuint, index, GLint, x)

DECLARE(void, glVertexAttribI2i, GLuint index, GLint x, GLint y)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint));
	BEFORE(glVertexAttribI2i);
	CALL_ORIG(glVertexAttribI2i, index, x, y);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddd", index, x, y);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI2i), GLuint, index, GLint, x, GLint, y)

DECLARE(void, glVertexAttribI3i, GLuint index, GLint x, GLint y, GLint z)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint));
	BEFORE(glVertexAttribI3i);
	CALL_ORIG(glVertexAttribI3i, index, x, y, z);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", index, x,
	      y, z);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI3i), GLuint, index, GLint, x, GLint, y, GLint, z)

DECLARE(void, glVertexAttribI4i, GLuint index, GLint x, GLint y, GLint z,
       GLint w)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint, GLint));
	BEFORE(glVertexAttribI4i);
	CALL_ORIG(glVertexAttribI4i, index, x, y, z, w);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddddd", index, x,
	      y, z, w);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4i), GLuint, index, GLint, x, GLint, y, GLint, z,
       GLint, w)

DECLARE(void, glVertexAttribI1ui, GLuint index, GLuint x)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glVertexAttribI1ui);
	CALL_ORIG(glVertexAttribI1ui, index, x);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd", index, x);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI1ui), GLuint, index, GLuint, x)

DECLARE(void, glUniformBlockBinding, GLuint program, GLuint UniformBlockIndex,
       GLuint uniformBlockBinding)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint));
	BEFORE(glUniformBlockBinding);
	CALL_ORIG(glUniformBlockBinding, program, UniformBlockIndex,
		  uniformBlockBinding);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddd", program,
	      UniformBlockIndex, uniformBlockBinding);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniformBlockBinding), GLuint, program, GLuint, UniformBlockIndex,
       GLuint, uniformBlockBinding)

DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix2x3fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix2x3fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix3x2fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix3x2fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix2x4fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix2x4fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix4x2fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix4x2fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix3x4fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix3x4fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glUniformMatrix4x3fv, "dxbp", GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glUniformMatrix4x3fv), GLint, location, GLsizei, count, GLboolean, transpose, const GLfloat *, value)

DECLARE(void, glVertexAttribI2ui, GLuint index, GLuint x, GLuint y)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI2ui);
	CALL_ORIG(glVertexAttribI2ui, index, x, y);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddd", index, x, y);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI2ui), GLuint, index, GLuint, x, GLuint, y)

DECLARE(void, glVertexAttribI3ui, GLuint index, GLuint x, GLuint y, GLuint z)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI3ui);
	CALL_ORIG(glVertexAttribI3ui, index, x, y, z);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", index, x, y,
	      z);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI3ui), GLuint, index, GLuint, x, GLuint, y, GLuint, z)


DECLARE(void, glVertexAttribI4ui, GLuint index, GLuint x, GLuint y, GLuint z,
       GLuint w)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLuint, GLuint, GLuint));
	BEFORE(glVertexAttribI4ui);
	CALL_ORIG(glVertexAttribI4ui, index, x, y, z, w);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddddd", index, x,
	      y, z, w);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4ui), GLuint, index, GLuint, x, GLuint, y, GLuint, z,
       GLuint, w)

DECLARE(void, glVertexAttribI1iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI1iv);
	CALL_ORIG(glVertexAttribI1iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI1iv), GLuint, index, const GLint *, v)

DECLARE(void, glVertexAttribI2iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI2iv);
	CALL_ORIG(glVertexAttribI2iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI2iv), GLuint, index, const GLint *, v)

DECLARE(void, glVertexAttribI3iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI3iv);
	CALL_ORIG(glVertexAttribI3iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI3iv), GLuint, index, const GLint *, v)

DECLARE(void, glVertexAttribI4iv, GLuint index, const GLint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLint*));
	BEFORE(glVertexAttribI4iv);
	CALL_ORIG(glVertexAttribI4iv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4iv), GLuint, index, const GLint *, v)

DECLARE(void, glVertexAttribI1uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI1uiv);
	CALL_ORIG(glVertexAttribI1uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI1uiv), GLuint, index, const GLuint *, v)

DECLARE(void, glVertexAttribI2uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI2uiv);
	CALL_ORIG(glVertexAttribI2uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI2uiv), GLuint, index, const GLuint *, v)

DECLARE(void, glGetQueryObjectuiv, GLuint id, GLenum pname, GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLuint *));
	BEFORE(glGetQueryObjectuiv);
	CALL_ORIG(glGetQueryObjectuiv, id, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", id, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetQueryObjectuiv), GLuint, id, GLenum, pname, GLuint *, params)

DECLARE(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount,
       const GLuint *uniformIndices, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const GLuint *, GLenum,
		GLint *));
	BEFORE(glGetActiveUniformsiv);
	CALL_ORIG(glGetActiveUniformsiv, program, uniformCount,uniformIndices,
		  pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddxp", program,
	      uniformCount, uniformIndices, pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetActiveUniformsiv), GLuint, program, GLsizei, uniformCount,
       const GLuint *, uniformIndices, GLenum, pname, GLint *, params)

DECLARE(void, glVertexAttribI3uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI3uiv);
	CALL_ORIG(glVertexAttribI3uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI3uiv), GLuint, index, const GLuint *, v)

DECLARE(void, glVertexAttribI4uiv, GLuint index, const GLuint *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLuint*));
	BEFORE(glVertexAttribI4uiv);
	CALL_ORIG(glVertexAttribI4uiv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4uiv), GLuint, index, const GLuint *, v)

DECLARE(void, glVertexAttribI4bv, GLuint index, const GLbyte *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLbyte*));
	BEFORE(glVertexAttribI4bv);
	CALL_ORIG(glVertexAttribI4bv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4bv), GLuint, index, const GLbyte *, v)

DECLARE(void, glVertexAttribI4sv, GLuint index, const GLshort *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLshort*));
	BEFORE(glVertexAttribI4sv);
	CALL_ORIG(glVertexAttribI4sv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4sv), GLuint, index, const GLshort *, v)

DECLARE(void, glVertexAttribI4ubv, GLuint index, const GLubyte *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLubyte*));
	BEFORE(glVertexAttribI4ubv);
	CALL_ORIG(glVertexAttribI4ubv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4ubv), GLuint, index, const GLubyte *, v)

DECLARE(void, glVertexAttribI4usv, GLuint index, const GLushort *v)
{
	TYPEDEF(void (*methodType)(GLuint, const GLushort*));
	BEFORE(glVertexAttribI4usv);
	CALL_ORIG(glVertexAttribI4usv, index, v);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", index, v);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribI4usv), GLuint, index, const GLushort *, v)

DECLARE(void, glVertexAttribIPointer, GLuint index, GLint size, GLenum type,
       GLsizei stride, const GLvoid *pointer)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLenum, GLsizei, const
		GLvoid *));
	BEFORE(glVertexAttribIPointer);
	CALL_ORIG(glVertexAttribIPointer, index, size, type, stride, pointer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddxdp", index,
	      size, type, stride, pointer);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribIPointer), GLuint, index, GLint, size, GLenum, type,
       GLsizei, stride, const GLvoid *, pointer)

DECLARE(void, glGetVertexAttribIiv, GLuint index, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint*));
	BEFORE(glGetVertexAttribIiv);
	CALL_ORIG(glGetVertexAttribIiv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", index,
	      pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetVertexAttribIiv), GLuint, index, GLenum, pname, GLint *, params)

DECLARE(void, glGetVertexAttribIuiv, GLuint index, GLenum pname, GLuint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLuint*));
	BEFORE(glGetVertexAttribIuiv);
	CALL_ORIG(glGetVertexAttribIuiv, index, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", index, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetVertexAttribIuiv), GLuint, index, GLenum, pname, GLuint *, params)

DECLARE(void, glUniform1ui, GLint location, GLuint v0)
{
	TYPEDEF(void (*methodType)(GLint, GLuint));
	BEFORE(glUniform1ui);
	CALL_ORIG(glUniform1ui, location, v0);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd", location, v0);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform1ui), GLint, location, GLuint, v0)

DECLARE(void, glUniform2ui, GLint location, GLuint v0, GLuint v1)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint));
	BEFORE(glUniform2ui);
	CALL_ORIG(glUniform2ui, location, v0, v1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddd", location,
	      v0, v1);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform2ui), GLint, location, GLuint, v0, GLuint, v1)

DECLARE(void, glUniform3ui, GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint, GLuint));
	BEFORE(glUniform3ui);
	CALL_ORIG(glUniform3ui, location, v0, v1, v2);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", location,
	      v0, v1, v2);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform3ui), GLint, location, GLuint, v0, GLuint, v1, GLuint, v2)

DECLARE(void, glUniform4ui, GLint location, GLuint v0, GLuint v1, GLuint v2,
       GLuint v3)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, GLuint, GLuint, GLuint));
	BEFORE(glUniform4ui);
	CALL_ORIG(glUniform4ui, location, v0, v1, v2, v3);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddddd", location,
	      v0, v1, v2, v3);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform4ui), GLint, location, GLuint, v0, GLuint, v1, GLuint, v2, GLuint, v3)

DECLARE(void, glUniform2uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform2uiv);
	CALL_ORIG(glUniform2uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", location,
	      count, value);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform2uiv), GLint, location, GLsizei, count, const GLuint *, value)

DECLARE(void, glUniform3uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform3uiv);
	CALL_ORIG(glUniform3uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", location,
	      count, value);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform3uiv), GLint, location, GLsizei, count, const GLuint *, value)

DECLARE(void, glUniform1uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform1uiv);
	CALL_ORIG(glUniform1uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", location,
	      count, value);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform1uiv), GLint, location, GLsizei, count, const GLuint *, value)

DECLARE(void, glUniform4uiv, GLint location, GLsizei count, const GLuint
       *value)
{
	TYPEDEF(void (*methodType)(GLint, GLuint, const GLuint*));
	BEFORE(glUniform4uiv);
	CALL_ORIG(glUniform4uiv, location, count, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", location,
	      count, value);
}
HANDLER_WRAPPERS(void, REAL_NAME(glUniform4uiv), GLint, location, GLsizei, count, const GLuint *, value)

DECLARE(void, glGetUniformuiv, GLuint program, GLint location, GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLuint*));
	BEFORE(glGetUniformuiv);
	CALL_ORIG(glGetUniformuiv, program, location, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", program,
	      location, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetUniformuiv), GLuint, program, GLint, location, GLuint *, params)

DECLARE(void, glBindFragDataLocation, GLuint program, GLuint colorNumber,
       const GLchar *name)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, const GLchar*));
	BEFORE(glBindFragDataLocation);
	CALL_ORIG(glBindFragDataLocation, program, colorNumber, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddp", program,
	      colorNumber, name);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindFragDataLocation), GLuint, program, GLuint, colorNumber,
       const GLchar *, name)

DECLARE(void, glGetFragDataLocation, GLuint program, const GLchar *name)
{
	TYPEDEF(void (*methodType)(GLuint, const GLchar*));
	BEFORE(glGetFragDataLocation);
	CALL_ORIG(glGetFragDataLocation, program, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ds", program,
	      name);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetFragDataLocation), GLuint, program, const GLchar *, name)

DECLARE(void, glBeginConditionalRenderNV, GLuint id, GLenum mode)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum));
	BEFORE(glBeginConditionalRenderNV);
	CALL_ORIG(glBeginConditionalRenderNV, id, mode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dx", id, mode);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBeginConditionalRenderNV), GLuint, id, GLenum, mode)

DECLARE_NOARGS(void, glEndConditionalRenderNV)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glEndConditionalRenderNV);
	CALL_ORIG(glEndConditionalRenderNV);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "");
}
HANDLER_WRAPPERS(void, REAL_NAME(glEndConditionalRenderNV), void)

DECLARE(void, glClampColorARB, GLenum target, GLenum clamp)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum));
	BEFORE(glClampColorARB);
	CALL_ORIG(glClampColorARB, target, clamp);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xx", target,
	      clamp);
}
HANDLER_WRAPPERS(void, REAL_NAME(glClampColorARB), GLenum, target, GLenum, clamp)

DECLARE(void, glRenderbufferStorageMultisample, GLenum target, GLsizei
       samplers, GLenum internalformat, GLsizei width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei,
		GLsizei));
	BEFORE(glRenderbufferStorageMultisample);
	CALL_ORIG(glRenderbufferStorageMultisample, target, samplers,
		  internalformat, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdxdd", samplers,
	      internalformat, width, height);
}
HANDLER_WRAPPERS(void, REAL_NAME(glRenderbufferStorageMultisample), GLenum, target, GLsizei,
       samplers, GLenum, internalformat, GLsizei, width, GLsizei, height)

DECLARE(void, glBlitFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1,
       GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint, GLint, GLint,
		GLint, GLint));
	BEFORE(glBlitFramebuffer);
	CALL_ORIG(glBlitFramebuffer, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,
	          dstX1, dstY1);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddddddd", srcX0,
	      srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBlitFramebuffer), GLint, srcX0, GLint, srcY0, GLint, srcX1,
       GLint, srcY1, GLint, dstX0, GLint, dstY0, GLint, dstX1, GLint, dstY1)

DECLARE(void, glClearColorIi, GLint r, GLint g, GLint b, GLint a)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint));
	BEFORE(glClearColorIi);
	CALL_ORIG(glClearColorIi, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", r, g, b, a);
}
HANDLER_WRAPPERS(void, REAL_NAME(glClearColorIi), GLint, r, GLint, g, GLint, b, GLint, a)

DECLARE(void, glClearColorIui, GLint r, GLint g, GLint b, GLint a)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint));
	BEFORE(glClearColorIui);
	CALL_ORIG(glClearColorIui, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", r, g, b, a);
}
HANDLER_WRAPPERS(void, REAL_NAME(glClearColorIui), GLint, r, GLint, g, GLint, b, GLint, a)

DECLARE_GL_DEFAULT_VOID(void, glClearBufferiv, "xdp", GLenum, buffer, GLint, drawBuffer, const GLuint *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glClearBufferiv), GLenum, buffer, GLint, drawBuffer, const GLuint *, value)
DECLARE_GL_DEFAULT_VOID(void, glClearBufferuiv, "xdp", GLenum, buffer, GLint, drawBuffer, const GLuint *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glClearBufferuiv), GLenum, buffer, GLint, drawBuffer, const GLuint *, value)
DECLARE_GL_DEFAULT_VOID(void, glClearBufferfv, "xdp", GLenum, buffer, GLint, drawBuffer, const GLfloat *, value)
HANDLER_WRAPPERS(void, REAL_NAME(glClearBufferfv), GLenum, buffer, GLint, drawBuffer, const GLfloat *, value)
DECLARE_GL_DEFAULT_VOID(void, glClearBufferfi, "xdpd", GLenum, buffer, GLint, drawBuffer, GLfloat, depth, GLint, stencil)
HANDLER_WRAPPERS(void, REAL_NAME(glClearBufferfi), GLenum, buffer, GLint, drawBuffer, GLfloat, depth, GLint, stencil)


DECLARE(void, glTexParameterIiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glTexParameterIiv);
	CALL_ORIG(glTexParameterIiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp", target, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexParameterIiv), GLenum, target, GLenum, pname, GLint *, params)

DECLARE(void, glTexParameterIuiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glTexParameterIuiv);
	CALL_ORIG(glTexParameterIuiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp", target, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexParameterIuiv), GLenum, target, GLenum, pname, GLint *, params)

DECLARE(void, glGetTexParameterIiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glGetTexParameterIiv);
	CALL_ORIG(glGetTexParameterIiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp", target, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetTexParameterIiv), GLenum, target, GLenum, pname, GLint *, params)

DECLARE(void, glGetTexParameterIuiv, GLenum target, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint*));
	BEFORE(glGetTexParameterIuiv);
	CALL_ORIG(glGetTexParameterIuiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp", target, pname,
	      params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetTexParameterIuiv), GLenum, target, GLenum, pname, GLint *, params)

DECLARE(void, glFramebufferTureLayer, GLenum target, GLenum attachment, GLuint
       ture, GLint level, GLint layer)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLuint, GLint, GLint));
	BEFORE(glFramebufferTureLayer);
	CALL_ORIG(glFramebufferTureLayer, target, attachment, ture, level,
		  layer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxddd", target,
	      attachment, ture, layer);
}
HANDLER_WRAPPERS(void, REAL_NAME(glFramebufferTureLayer), GLenum, target, GLenum, attachment, GLuint,
       ture, GLint, level, GLint, layer)

DECLARE(void, glColorMaskIndexed, GLuint buf, GLboolean r, GLboolean g,
       GLboolean b, GLboolean a)
{
	TYPEDEF(void (*methodType)(GLuint, GLboolean, GLboolean, GLboolean,
		GLboolean));
	BEFORE(glColorMaskIndexed);
	CALL_ORIG(glColorMaskIndexed, buf, r, g, b, a);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddd", buf, r, g,
	      b, a);
}
HANDLER_WRAPPERS(void, REAL_NAME(glColorMaskIndexed), GLuint, buf, GLboolean, r, GLboolean, g,
       GLboolean, b, GLboolean, a)

DECLARE(void, glGetBooleanIndexedv, GLenum value, GLint index, GLboolean *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLboolean*));
	BEFORE(glGetBooleanIndexedv);
	CALL_ORIG(glGetBooleanIndexedv, value, index, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdp", value, index,
	      data);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetBooleanIndexedv), GLenum, value, GLint, index, GLboolean *, data)

DECLARE_GL_DEFAULT_VOID(void, glGetInteger64i_v, "xdp", GLenum, target, GLuint, index, GLint64 *, data)
HANDLER_WRAPPERS(void, REAL_NAME(glGetInteger64i_v), GLenum, target, GLuint, index, GLint64 *, data)
DECLARE_GL_DEFAULT_VOID(void, glGetInteger64v, "xp", GLenum, pname, GLint64 *, data)
HANDLER_WRAPPERS(void, REAL_NAME(glGetInteger64v), GLenum, pname, GLint64 *, data)

DECLARE_GL_DEFAULT_VOID(void, glGetIntegeri_v, "xdp", GLenum, target, GLuint, index, GLint *, data)
HANDLER_WRAPPERS(void, REAL_NAME(glGetIntegeri_v), GLenum, target, GLuint, index, GLint *, data)

DECLARE(void, glGetIntegerIndexedv, GLenum value, GLint index, GLboolean *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLboolean*));
	BEFORE(glGetIntegerIndexedv);
	CALL_ORIG(glGetIntegerIndexedv, value, index, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdp", value, index,
	      data);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetIntegerIndexedv), GLenum, value, GLint, index, GLboolean *, data)

DECLARE(void, glEnableIndexed, GLenum value, GLint index)
{
	TYPEDEF(void (*methodType)(GLenum, GLint));
	BEFORE(glEnableIndexed);
	CALL_ORIG(glEnableIndexed, value, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xd", value, index);
}
HANDLER_WRAPPERS(void, REAL_NAME(glEnableIndexed), GLenum, value, GLint, index)

DECLARE(void, glDisableIndexed, GLenum value, GLint index)
{
	TYPEDEF(void (*methodType)(GLenum, GLint));
	BEFORE(glDisableIndexed);
	CALL_ORIG(glDisableIndexed, value, index);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xd", value, index);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDisableIndexed), GLenum, value, GLint, index)

DECLARE(GLboolean, glIsEnabledIndexed, GLenum target, GLuint index)
{
	TYPEDEF(GLboolean (*methodType)(GLenum, GLuint));
	BEFORE(glIsEnabledIndexed);
	GLboolean ret = CALL_ORIG(glIsEnabledIndexed, target, index);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "xd", target, index);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glIsEnabledIndexed), GLenum, target, GLuint, index)

DECLARE(void, glBindBufferRange, GLenum target, GLuint index, GLuint buffer,
       GLintptr offset, GLsizeiptr size)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLintptr,
		GLsizeiptr));
	BEFORE(glBindBufferRange);
	CALL_ORIG(glBindBufferRange, target, index, buffer, offset, size);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xddpp", target,
	      index, buffer, offset, size);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindBufferRange), GLenum, target, GLuint, index, GLuint, buffer,
       GLintptr, offset, GLsizeiptr, size)

DECLARE(void, glBindBufferOffset, GLenum target, GLuint index, GLuint buffer,
       GLintptr offset)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLintptr));
	BEFORE(glBindBufferOffset);
	CALL_ORIG(glBindBufferOffset, target, index, buffer, offset);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xddp", target,
	      index, buffer, offset);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindBufferOffset), GLenum, target, GLuint, index, GLuint, buffer,
       GLintptr, offset)

DECLARE(void, glBindBufferBase, GLenum target, GLuint index, GLuint buffer)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint));
	BEFORE(glBindBufferBase);
	CALL_ORIG(glBindBufferBase, target, index, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xddp", target,
	      index, buffer);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindBufferBase), GLenum, target, GLuint, index, GLuint, buffer)

DECLARE(void, glBeginTransformFeedback, GLenum primitiveMode)
{
	TYPEDEF(void (*methodType)(GLenum));
	BEFORE(glBeginTransformFeedback);
	CALL_ORIG(glBeginTransformFeedback, primitiveMode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "x", primitiveMode);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBeginTransformFeedback), GLenum, primitiveMode)

DECLARE_NOARGS(void, glEndTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glEndTransformFeedback);
	CALL_ORIG(glEndTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "");
}
HANDLER_WRAPPERS(void, REAL_NAME(glEndTransformFeedback), void)

DECLARE(void, glTransformFeedbackVaryings, GLuint program, GLsizei count, const
       char **varyings, GLenum bufferMode)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const char **, GLenum));
	BEFORE(glTransformFeedbackVaryings);
	CALL_ORIG(glTransformFeedbackVaryings, program, count, varyings,
		  bufferMode);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddpx", program,
	      count, varyings, bufferMode);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTransformFeedbackVaryings), GLuint, program, GLsizei, count, const
       char **, varyings, GLenum, bufferMode)

DECLARE(void, glGetTransformFeedbackVarying, GLuint program, GLuint index,
       GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, char
       *name)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei
		*, GLenum *, char *));
	BEFORE(glGetTransformFeedbackVarying);
	CALL_ORIG(glGetTransformFeedbackVarying, program, index, bufSize,
		  length, size, type, name);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddppps", program,
	      index, bufSize, length, size, type, name);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetTransformFeedbackVarying), GLuint, program, GLuint, index,
       GLsizei, bufSize, GLsizei *, length, GLsizei *, size, GLenum *, type, char *, name)

DECLARE(void, glFlushMappedBufferRange, GLenum target, GLintptr offset,
       GLsizeiptr length)
{
	TYPEDEF(void (*methodType)(GLenum, GLintptr, GLsizeiptr));
	BEFORE(glFlushMappedBufferRange);
	CALL_ORIG(glFlushMappedBufferRange, target, offset, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xpp", target,
	      offset, length );
}
HANDLER_WRAPPERS(void, REAL_NAME(glFlushMappedBufferRange), GLenum, target, GLintptr, offset,
       GLsizeiptr, length)

DECLARE(GLboolean, glUnmapBuffer, GLenum target)
{
	TYPEDEF(GLboolean (*methodType)(GLenum));
	BEFORE(glUnmapBuffer);
	GLboolean ret = CALL_ORIG(glUnmapBuffer, target);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "x", target);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glUnmapBuffer), GLenum, target)

DECLARE(void, glCopyBufferSubData, GLenum readtarget,GLenum writetarget,
       GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLintptr, GLintptr,
		GLsizeiptr));
	BEFORE(glCopyBufferSubData);
	CALL_ORIG(glCopyBufferSubData, readtarget, writetarget, readoffset,
		  writeoffset, size);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxppp", readtarget,
	      writetarget, readoffset, writeoffset, size);
}
HANDLER_WRAPPERS(void, REAL_NAME(glCopyBufferSubData), GLenum, readtarget, GLenum, writetarget,
       GLintptr, readoffset, GLintptr, writeoffset, GLsizeiptr, size)

DECLARE(void, glGenVertexArrays, GLsizei n, GLuint *arrays)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenVertexArrays);
	CALL_ORIG(glGenVertexArrays, n, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xp", n, arrays);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGenVertexArrays), GLsizei, n, GLuint *, arrays)

DECLARE(void, glDeleteVertexArrays, GLsizei n, const GLuint *arrays)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteVertexArrays);
	CALL_ORIG(glDeleteVertexArrays, n, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xp", n, arrays);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDeleteVertexArrays), GLsizei, n, const GLuint *, arrays)

DECLARE(void, glBindVertexArray, GLuint arrays)
{
	TYPEDEF(void (*methodType)(GLuint));
	BEFORE(glBindVertexArray);
	CALL_ORIG(glBindVertexArray, arrays);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "d", arrays);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindVertexArray), GLuint, arrays)

DECLARE(void, glGetProgramBinary, GLuint program, GLsizei bufSize, GLsizei
       *length, GLenum *binaryFormat, void *binary)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, GLsizei *, GLenum *,
		void*));
	BEFORE(glGetProgramBinary);
	CALL_ORIG(glGetProgramBinary, program, bufSize, length, binaryFormat,
		  binary);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddppp", program,
	      bufSize, length, binaryFormat, binary);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetProgramBinary), GLuint, program, GLsizei, bufSize, GLsizei *, length, GLenum *, binaryFormat, void *, binary)

DECLARE(void, glProgramBinary, GLuint program, GLenum binaryFormat, const void
       *binary, GLsizei length)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const void *, GLsizei));
	BEFORE(glProgramBinary);
	CALL_ORIG(glProgramBinary, program, binaryFormat, binary, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", program,
	      binaryFormat, binary, length);
}
HANDLER_WRAPPERS(void, REAL_NAME(glProgramBinary), GLuint, program, GLenum, binaryFormat, const void *, binary, GLsizei, length)

DECLARE(GLuint, glGetUniformBlockIndex, GLuint program, const GLchar
       *uniformBlockName)
{
	TYPEDEF(GLuint (*methodType)(GLuint, const GLchar *));
	BEFORE(glGetUniformBlockIndex);
	GLint ret = CALL_ORIG(glGetUniformBlockIndex, program,
			      uniformBlockName);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "dp", program, uniformBlockName);

	return ret;
}
HANDLER_WRAPPERS(GLuint, REAL_NAME(glGetUniformBlockIndex), GLuint, program, const GLchar *, uniformBlockName)

DECLARE(void, glGetActiveUniformBlockName, GLuint program, GLuint
       UniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar
       *uniformBlockName)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLsizei, GLsizei *, GLchar
		*));
	BEFORE(glGetActiveUniformBlockName);
	CALL_ORIG(glGetActiveUniformBlockName, program, UniformBlockIndex,
	          bufSize, length, uniformBlockName);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddxp", program,
	      UniformBlockIndex, bufSize, length, uniformBlockName);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetActiveUniformBlockName), GLuint, program, GLuint,
       UniformBlockIndex, GLsizei, bufSize, GLsizei *, length, GLchar *, uniformBlockName)

DECLARE(void, glGetActiveUniformBlockiv, GLuint program, GLuint
       UniformBlockIndex, GLenum pname, GLint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint, GLenum, GLint *));
	BEFORE(glGetActiveUniformBlockiv);
	CALL_ORIG(glGetActiveUniformBlockiv, program, UniformBlockIndex,
	          pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddxp", program,
	      UniformBlockIndex, pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetActiveUniformBlockiv), GLuint, program, GLuint,
       UniformBlockIndex, GLenum, pname, GLint *, params)

DECLARE(void, glGetUniformIndices, GLuint program, GLsizei uniformCount, const
       GLchar **uniformsNames, GLuint *uniformIndices)
{
	TYPEDEF(void (*methodType)(GLuint, GLsizei, const GLchar **, GLuint
		*));
	BEFORE(glGetUniformIndices);
	CALL_ORIG(glGetUniformIndices, program, uniformCount, uniformsNames,
	          uniformIndices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "ddxpp", program,
	      uniformCount, uniformsNames, uniformIndices);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetUniformIndices), GLuint, program, GLsizei, uniformCount, const
       GLchar **, uniformsNames, GLuint *, uniformIndices)

DECLARE(void, glGenQueries, GLsizei n, GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenQueries);
	CALL_ORIG(glGenQueries, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, ids);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGenQueries), GLsizei, n, GLuint *, ids)

DECLARE(void, glBeginQuery, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glBeginQuery);
	CALL_ORIG(glBeginQuery, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xd", target, id);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBeginQuery), GLenum, target, GLuint, id)

DECLARE(void, glEndQuery, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glEndQuery);
	CALL_ORIG(glEndQuery, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xd", target, id);
}
HANDLER_WRAPPERS(void, REAL_NAME(glEndQuery), GLenum, target, GLuint, id)

DECLARE(void, glDeleteQueries, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteQueries);
	CALL_ORIG(glDeleteQueries, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, ids);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDeleteQueries), GLsizei, n, const GLuint *, ids)

DECLARE(void, glGenTransformFeedbacks, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glGenTransformFeedbacks);
	CALL_ORIG(glGenTransformFeedbacks, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, ids);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGenTransformFeedbacks), GLsizei, n, const GLuint *, ids)

DECLARE(void, glDeleteTransformFeedbacks, GLsizei n, const GLuint *ids)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteTransformFeedbacks);
	CALL_ORIG(glDeleteTransformFeedbacks, n, ids);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, ids);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDeleteTransformFeedbacks), GLsizei, n, const GLuint *, ids)

DECLARE_NOARGS(void, glPauseTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glPauseTransformFeedback);
	CALL_ORIG(glPauseTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "");
}
HANDLER_WRAPPERS(void, REAL_NAME(glPauseTransformFeedback), void)

DECLARE_NOARGS(void, glResumeTransformFeedback)
{
	TYPEDEF(void (*methodType)(void));
	BEFORE(glResumeTransformFeedback);
	CALL_ORIG(glResumeTransformFeedback);
	GL_GET_ERROR();
	AFTER_NO_PARAM('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "");
}
HANDLER_WRAPPERS(void, REAL_NAME(glResumeTransformFeedback), void)

DECLARE(void, glGenSamplers, GLsizei n, GLuint *samplers)
{
	TYPEDEF(void (*methodType)(GLsizei, GLuint *));
	BEFORE(glGenSamplers);
	CALL_ORIG(glGenSamplers, n, samplers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, samplers);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGenSamplers), GLsizei, n, GLuint *, samplers)

DECLARE(void, glBindSampler, GLuint unit, GLuint sampler)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glBindSampler);
	CALL_ORIG(glBindSampler, unit, sampler);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd", unit, sampler);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindSampler), GLuint, unit, GLuint, sampler)

DECLARE(void, glSamplerParameterf, GLuint sampler, GLenum pname, GLfloat param)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLfloat));
	BEFORE(glSamplerParameterf);
	CALL_ORIG(glSamplerParameterf, sampler, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxd", sampler,
	      pname, param);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameterf), GLuint, sampler, GLenum, pname, GLfloat, param)

DECLARE(void, glSamplerParameteri, GLuint sampler, GLenum pname, GLint param)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint));
	BEFORE(glSamplerParameteri);
	CALL_ORIG(glSamplerParameteri, sampler, pname, param);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxd", sampler,
	      pname, param);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameteri), GLuint, sampler, GLenum, pname, GLint, param)

DECLARE(void, glSamplerParameterfv, GLuint sampler, GLenum pname, const GLfloat
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLfloat *));
	BEFORE(glSamplerParameterfv);
	CALL_ORIG(glSamplerParameterfv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", sampler,
	      pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameterfv), GLuint, sampler, GLenum, pname, const GLfloat *, params)

DECLARE(void, glSamplerParameteriv, GLuint sampler, GLenum pname, const GLint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLint *));
	BEFORE(glSamplerParameteriv);
	CALL_ORIG(glSamplerParameteriv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", sampler,
	      pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameteriv), GLuint, sampler, GLenum, pname, const GLint *, params)

DECLARE(void, glSamplerParameterIiv, GLuint sampler, GLenum pname, const GLint
       *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLint *));
	BEFORE(glSamplerParameterIiv);
	CALL_ORIG(glSamplerParameterIiv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", sampler,
	      pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameterIiv), GLuint, sampler, GLenum, pname, const GLint *, params)

DECLARE(void, glSamplerParameterIuiv, GLuint sampler, GLenum pname, const
       GLuint *params)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, const GLuint *));
	BEFORE(glSamplerParameterIuiv);
	CALL_ORIG(glSamplerParameterIuiv, sampler, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxp", sampler,
	      pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glSamplerParameterIuiv), GLuint, sampler, GLenum, pname, const GLuint *, params)

DECLARE(void, glDeleteSamplers, GLsizei n, const GLuint *samplers)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLuint *));
	BEFORE(glDeleteSamplers);
	CALL_ORIG(glDeleteSamplers, n, samplers);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, samplers);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDeleteSamplers), GLsizei, n, const GLuint *, samplers)

DECLARE(void, glTexImage3D, GLenum target, GLint level, GLint internalFormat,
       GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum
       format, GLenum type, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLsizei, GLsizei,
		GLsizei, GLint, GLenum, GLenum, const GLvoid *));
	BEFORE(glTexImage3D);
	CALL_ORIG(glTexImage3D, target, level, internalFormat, width,
		  height, depth, border, format, type, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xddddddxxp", target,
	      level, internalFormat, width, height, depth, border, format,
	      type, data);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexImage3D), GLenum, target, GLint, level, GLint, internalFormat,
       GLsizei, width, GLsizei, height, GLsizei, depth, GLint, border, GLenum,
       format, GLenum, type, const GLvoid *, data)

DECLARE(void, glTexStorage2D, GLenum target, GLsizei levels, GLenum
       internalformat, GLsizei width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei, GLsizei));
	BEFORE(glTexStorage2D);
	CALL_ORIG(glTexStorage2D, target, levels, internalformat, width,
		  height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdxdd", target,
	      levels, internalformat, width, height);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexStorage2D), GLenum, target, GLsizei, levels, GLenum,
       internalformat, GLsizei, width, GLsizei, height)

DECLARE(void, glTexSubImage3D, GLint level, GLint xoffset, GLint yoffset, GLint
       zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
       GLenum type, const GLvoid *pixels)
{
	TYPEDEF(void (*methodType)(GLint, GLint, GLint, GLint, GLsizei,
		GLsizei, GLsizei, GLenum, GLenum, const GLvoid *));
	BEFORE(glTexSubImage3D);
	CALL_ORIG(glTexSubImage3D, level, xoffset, yoffset, zoffset, width,
		  height, depth, format, type, pixels);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddddddxxp", level,
	      xoffset, yoffset, zoffset, width, height, depth, format, type,
	      pixels);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexSubImage3D), GLint, level, GLint, xoffset, GLint, yoffset, GLint,
       zoffset, GLsizei, width, GLsizei, height, GLsizei, depth, GLenum, format,
       GLenum, type, const GLvoid *, pixels)

DECLARE(void, glCompressedTexImage3D, GLenum target, GLint level, GLenum
       internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint
       border, GLsizei imageSize, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLenum, GLsizei, GLsizei,
		GLsizei, GLint, GLsizei, const GLvoid *));
	BEFORE(glCompressedTexImage3D);
	CALL_ORIG(glCompressedTexImage3D, target, level, internalformat,
		  width, height, depth, border, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdxdddddp", target,
	      level, internalformat, width, height, depth, border,
	      imageSize, data);
}
HANDLER_WRAPPERS(void, REAL_NAME(glCompressedTexImage3D), GLenum, target, GLint, level, GLenum,
       internalformat, GLsizei, width, GLsizei, height, GLsizei, depth, GLint,
       border, GLsizei, imageSize, const GLvoid *, data)

DECLARE(void, glCompressedTexSubImage3D, GLenum target, GLint xoffset, GLint
       yoffset, GLint zoffset, GLsizei width, GLint height, GLint depth,
       GLenum format, GLsizei imageSize, const GLvoid *data)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLint, GLsizei,
		GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *));
	BEFORE(glCompressedTexSubImage3D);
	CALL_ORIG(glCompressedTexSubImage3D, target, xoffset, yoffset,
		  zoffset, width, height, depth, format, imageSize, data);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdddddddxdp",
	      target, xoffset, yoffset, zoffset, width, height, depth,
	      format, imageSize, data);
}
HANDLER_WRAPPERS(void, REAL_NAME(glCompressedTexSubImage3D), GLenum, target, GLint, xoffset, GLint,
       yoffset, GLint, zoffset, GLsizei, width, GLint, height, GLint, depth,
       GLenum, format, GLsizei, imageSize, const GLvoid *, data)

DECLARE(void, glFramebufferTextureLayer, GLenum target, GLenum attachment,
       GLuint texture, GLint level, GLint layer)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLuint, GLint, GLint));
	BEFORE(glFramebufferTextureLayer);
	CALL_ORIG(glFramebufferTextureLayer, target, attachment, texture,
		  level, layer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxddd", target,
	      attachment, texture, level, layer);
}
HANDLER_WRAPPERS(void, REAL_NAME(glFramebufferTextureLayer), GLenum, target, GLenum, attachment,
       GLuint, texture, GLint, level, GLint, layer)

DECLARE(void, glInvalidateFramebuffer, GLenum target, GLsizei numAttachments,
       const GLenum *attachments)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, const GLenum *));
	BEFORE(glInvalidateFramebuffer);
	CALL_ORIG(glInvalidateFramebuffer, target, numAttachments,
		  attachments);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdp", target,
	      numAttachments, attachments);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateFramebuffer), GLenum, target, GLsizei, numAttachments,
       const GLenum *, attachments)

DECLARE(void, glInvalidateSubFramebuffer, GLenum target, GLsizei
       numAttachments, const GLenum *attachments, GLint x, GLint y, GLint
       width, GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, const GLenum *, GLint,
		GLint, GLsizei, GLsizei));
	BEFORE(glInvalidateSubFramebuffer);
	CALL_ORIG(glInvalidateSubFramebuffer, target, numAttachments,
		  attachments, x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdpdddd", target,
	      numAttachments, attachments, x, y, width, height);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateSubFramebuffer), GLenum, target, GLsizei,
       numAttachments, const GLenum *, attachments, GLint, x, GLint, y, GLint,
       width, GLsizei, height)

DECLARE(void, glInvalidateTexSubImage, GLuint texture, GLint level, GLint
       xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
       GLsizei depth)
{
	TYPEDEF(void (*methodType)(GLuint, GLint, GLint, GLint, GLint,
		GLsizei, GLsizei, GLsizei));
	BEFORE(glInvalidateTexSubImage);
	CALL_ORIG(glInvalidateTexSubImage, texture, level, xoffset, yoffset,
		  zoffset, width, height, depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dddddddd", texture,
	      level, xoffset, yoffset, zoffset, width, height, depth);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateTexSubImage), GLuint, texture, GLint, level, GLint,
       xoffset, GLint, yoffset, GLint, zoffset, GLsizei, width, GLsizei, height,
       GLsizei, depth)

DECLARE(void, glInvalidateTexImage, GLuint texture, GLint level)
{
	TYPEDEF(void (*methodType)(GLuint, GLint));
	BEFORE(glInvalidateTexImage);
	CALL_ORIG(glInvalidateTexImage, texture, level);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd", texture, level);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateTexImage), GLuint, texture, GLint, level)

DECLARE(void, glInvalidateBufferData, GLuint buffer)
{
	TYPEDEF(void (*methodType)(GLuint));
	BEFORE(glInvalidateBufferData);
	CALL_ORIG(glInvalidateBufferData, buffer);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "d", buffer);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateBufferData), GLuint, buffer)

DECLARE(void, glInvalidateBufferSubData, GLuint buffer, GLintptr offset,
       GLsizeiptr length)
{
	TYPEDEF(void (*methodType)(GLuint, GLintptr, GLsizeiptr));
	BEFORE(glInvalidateBufferSubData);
	CALL_ORIG(glInvalidateBufferSubData, buffer, offset, length);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxx", buffer,
	      offset, length);
}
HANDLER_WRAPPERS(void, REAL_NAME(glInvalidateBufferSubData), GLuint, buffer, GLintptr, offset,
       GLsizeiptr, length)

DECLARE(GLboolean, glIsVertexArray, GLuint array)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsVertexArray);
	GLboolean ret = CALL_ORIG(glIsVertexArray, array);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "d", array);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glIsVertexArray), GLuint, array)

DECLARE(GLboolean, glIsTransformFeedback, GLuint id)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsTransformFeedback);
	GLboolean ret = CALL_ORIG(glIsTransformFeedback, id);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "d", id);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glIsTransformFeedback), GLuint, id)

//==========================================================================
DECLARE(void, glBindTransformFeedback, GLenum target, GLuint id)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint));
	BEFORE(glBindTransformFeedback);
	CALL_ORIG(glBindTransformFeedback, target, id);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xd", target, id);
}
HANDLER_WRAPPERS(void, REAL_NAME(glBindTransformFeedback), GLenum, target, GLuint, id)

DECLARE(GLenum, glClientWaitSync, GLsync sync, GLbitfield flags,
       GLuint64 timeout)
{
	TYPEDEF(GLenum (*methodType)(GLsync, GLbitfield, GLuint64));
	BEFORE(glClientWaitSync);
	GLenum ret = CALL_ORIG(glClientWaitSync, sync, flags, timeout);
	GL_GET_ERROR();
	AFTER('x', ret, APITYPE_CONTEXT, call_type, caller, "", "pxx",
	      sync, flags, timeout);

	return ret;
}
HANDLER_WRAPPERS(GLenum, REAL_NAME(glClientWaitSync), GLsync, sync, GLbitfield, flags,
       GLuint64, timeout)

DECLARE(void, glCopyTexSubImage3D, GLenum target, GLint level, GLint xoffset,
       GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width,
       GLsizei height)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLint, GLint, GLint , GLint,
				   GLint, GLsizei, GLsizei));
	BEFORE(glCopyTexSubImage3D);
	CALL_ORIG(glCopyTexSubImage3D, target, level, xoffset, yoffset, zoffset,
		  x, y, width, height);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdddddddd",
	      target, level, xoffset, yoffset, zoffset, x, y, width, height);
}
HANDLER_WRAPPERS(void, REAL_NAME(glCopyTexSubImage3D), GLenum, target, GLint, level, GLint, xoffset,
       GLint, yoffset, GLint, zoffset, GLint, x, GLint, y, GLsizei, width,
       GLsizei, height)

DECLARE(void, glDeleteSync, GLsync sync)
{
	TYPEDEF(void (*methodType)(GLsync));
	BEFORE(glDeleteSync);
	CALL_ORIG(glDeleteSync, sync);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "p", sync);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDeleteSync), GLsync, sync)

DECLARE(void, glDrawArraysInstanced, GLenum mode, GLint first, GLsizei count,
       GLsizei primcount)
{
	TYPEDEF(void (*methodType)(GLenum, GLint, GLsizei, GLsizei));
	BEFORE(glDrawArraysInstanced);
	CALL_ORIG(glDrawArraysInstanced, mode, first, count, primcount);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xddd",  mode, first,
	      count, primcount);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDrawArraysInstanced), GLenum, mode, GLint, first, GLsizei, count,
       GLsizei, primcount)

DECLARE(void, glDrawBuffers, GLsizei n, const GLenum *bufs)
{
	TYPEDEF(void (*methodType)(GLsizei, const GLenum *));
	BEFORE(glDrawBuffers);
	CALL_ORIG(glDrawBuffers, n, bufs);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dp", n, bufs);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDrawBuffers), GLsizei, n, const GLenum *, bufs)

DECLARE(void, glDrawElementsInstanced, GLenum mode, GLsizei count, GLenum type,
       const void * indices, GLsizei primcount)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, const void *,
				   GLsizei));
	BEFORE(glDrawElementsInstanced);
	CALL_ORIG(glDrawElementsInstanced, mode, count, type, indices,
		  primcount);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdxpd",
	      mode, count, type, indices, primcount);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDrawElementsInstanced), GLenum, mode, GLsizei, count, GLenum, type,
       const void *, indices, GLsizei, primcount)

DECLARE(void, glDrawRangeElements, GLenum mode, GLuint start, GLuint end,
       GLsizei count, GLenum type, const GLvoid * indices)
{
	TYPEDEF(void (*methodType)(GLenum, GLuint, GLuint, GLsizei, GLenum,
		const GLvoid *));
	BEFORE(glDrawRangeElements);
	CALL_ORIG(glDrawRangeElements, mode, start, end, count, type, indices);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdddxp",
	      mode, start, end, count, type, indices);
}
HANDLER_WRAPPERS(void, REAL_NAME(glDrawRangeElements), GLenum, mode, GLuint, start, GLuint, end,
       GLsizei, count, GLenum, type, const GLvoid *, indices)

DECLARE(void, glWaitSync, GLsync sync, GLbitfield flags, GLuint64 timeout)
{
	TYPEDEF(void (*methodType)(GLsync, GLbitfield, GLuint64));
	BEFORE(glWaitSync);
	CALL_ORIG(glWaitSync, sync, flags, timeout);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "pxx",
	      sync, flags, timeout);
}
HANDLER_WRAPPERS(void, REAL_NAME(glWaitSync), GLsync, sync, GLbitfield, flags, GLuint64, timeout)

DECLARE(void, glVertexAttribDivisor, GLuint index, GLuint divisor)
{
	TYPEDEF(void (*methodType)(GLuint, GLuint));
	BEFORE(glVertexAttribDivisor);
	CALL_ORIG(glVertexAttribDivisor, index, divisor);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dd",
	      index, divisor);
}
HANDLER_WRAPPERS(void, REAL_NAME(glVertexAttribDivisor), GLuint, index, GLuint, divisor)

DECLARE(void, glTexStorage3D, GLenum target, GLsizei levels,
       GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)
{
	TYPEDEF(void (*methodType)(GLenum, GLsizei, GLenum, GLsizei, GLsizei,
		GLsizei));
	BEFORE(glTexStorage3D);
	CALL_ORIG(glTexStorage3D, target, levels, internalformat, width, height,
		  depth);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xdxddd",
	      target, levels, internalformat, width, height, depth);
}
HANDLER_WRAPPERS(void, REAL_NAME(glTexStorage3D), GLenum, target, GLsizei, levels,
       GLenum, internalformat, GLsizei, width, GLsizei, height, GLsizei, depth)

DECLARE(void, glReadBuffer, GLenum src)
{
	TYPEDEF(void (*methodType)(GLenum));
	BEFORE(glReadBuffer);
	CALL_ORIG(glReadBuffer, src);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "x",
	      src);
}
HANDLER_WRAPPERS(void, REAL_NAME(glReadBuffer), GLenum, src)

DECLARE(void, glProgramParameteri, GLuint program, GLenum pname, GLint value)
{
	TYPEDEF(void (*methodType)(GLuint, GLenum, GLint));
	BEFORE(glProgramParameteri);
	CALL_ORIG(glProgramParameteri, program, pname, value);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "dxd",
	      program, pname, value);
}
HANDLER_WRAPPERS(void, REAL_NAME(glProgramParameteri), GLuint, program, GLenum, pname, GLint, value)

DECLARE(void *, glMapBufferRange, GLenum target, GLintptr offset,
       GLsizeiptr length, GLbitfield access)
{
	TYPEDEF(void *(*methodType)(GLenum, GLintptr, GLsizeiptr, GLbitfield));
	BEFORE(glMapBufferRange);
	void *ret = CALL_ORIG(glMapBufferRange, target, offset, length, access);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xpdx",
	      target, offset, length, access);

	return ret;
}
HANDLER_WRAPPERS(void *, REAL_NAME(glMapBufferRange), GLenum, target, GLintptr, offset,
       GLsizeiptr, length, GLbitfield, access)

DECLARE(GLboolean, glIsSync, GLsync sync)
{
	TYPEDEF(GLboolean (*methodType)(GLsync));
	BEFORE(glIsSync);
	GLboolean ret = CALL_ORIG(glIsSync, sync);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "p",
	      sync);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glIsSync), GLsync, sync)

DECLARE(GLboolean, glIsQuery, GLuint id)
{
	TYPEDEF(GLboolean (*methodType)(GLuint));
	BEFORE(glIsQuery);
	GLboolean ret = CALL_ORIG(glIsQuery, id);
	GL_GET_ERROR();
	AFTER('c', ret, APITYPE_CONTEXT, call_type, caller, "", "d",
	      id);

	return ret;
}
HANDLER_WRAPPERS(GLboolean, REAL_NAME(glIsQuery), GLuint, id)

DECLARE_GL_DEFAULT_VOID(void, glGetSamplerParameterfv, "dxp", GLuint, sampler, GLenum, pname, GLfloat *, params)
HANDLER_WRAPPERS(void, REAL_NAME(glGetSamplerParameterfv), "dxp", GLuint, sampler, GLenum, pname, GLfloat *, params)
DECLARE_GL_DEFAULT_VOID(void, glGetSamplerParameteriv, "dxp", GLuint, sampler, GLenum, pname, GLint *, params)
HANDLER_WRAPPERS(void, REAL_NAME(glGetSamplerParameteriv), "dxp", GLuint, sampler, GLenum, pname, GLint *, params)

DECLARE(void, glGetSynciv, GLsync sync, GLenum pname, GLsizei bufSize,
       GLsizei *length, GLint *values)
{
	TYPEDEF(void (*methodType)(GLsync, GLenum, GLsizei, GLsizei *, GLint *));
	BEFORE(glGetSynciv);
	CALL_ORIG(glGetSynciv, sync, pname, bufSize, length, values);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "pxdpp",
	      sync, pname, bufSize, length, values);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetSynciv), GLsync, sync, GLenum, pname, GLsizei, bufSize,
       GLsizei *, length, GLint *, values)

DECLARE(void, glGetQueryiv, GLenum target, GLenum pname, GLint * params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLint *));
	BEFORE(glGetQueryiv);
	CALL_ORIG(glGetQueryiv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp",
	      target, pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetQueryiv), GLenum, target, GLenum, pname, GLint *, params)

DECLARE(void, glGetInternalformativ, GLenum target, GLenum internalformat,
       GLenum pname, GLsizei bufSize, GLint *params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLenum, GLsizei, GLint *));
	BEFORE(glGetInternalformativ);
	CALL_ORIG(glGetInternalformativ, target, internalformat, pname, bufSize,
		  params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxxdp",
	      target, internalformat, pname, bufSize, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetInternalformativ), GLenum, target, GLenum, internalformat,
       GLenum, pname, GLsizei, bufSize, GLint *, params)

DECLARE_GL_DEFAULT_VOID(void, glGetBufferParameteri64v, "xxp", GLenum, target, GLenum, value, GLint64 *, data)
HANDLER_WRAPPERS(void, REAL_NAME(glGetBufferParameteri64v), "xxp", GLenum, target, GLenum, value, GLint64 *, data)


DECLARE(void, glGetBufferPointerv, GLenum target, GLenum pname,
       GLvoid ** params)
{
	TYPEDEF(void (*methodType)(GLenum, GLenum, GLvoid **));
	BEFORE(glGetBufferPointerv);
	CALL_ORIG(glGetBufferPointerv, target, pname, params);
	GL_GET_ERROR();
	AFTER('v', NO_RETURN_VALUE, APITYPE_CONTEXT, call_type, caller, "", "xxp",
	      target, pname, params);
}
HANDLER_WRAPPERS(void, REAL_NAME(glGetBufferPointerv), GLenum, target, GLenum, pname,
       GLvoid **, params)

DECLARE(GLsync, glFenceSync, GLenum condition, GLbitfield flags)
{
	TYPEDEF(GLsync (*methodType)(GLenum, GLbitfield));
	BEFORE(glFenceSync);
	GLsync ret = CALL_ORIG(glFenceSync, condition, flags);
	GL_GET_ERROR();
	AFTER('x', ret, APITYPE_CONTEXT, call_type, caller, "", "xx",
	      condition, flags);

	return ret;
}
HANDLER_WRAPPERS(GLsync, REAL_NAME(glFenceSync), GLenum, condition, GLbitfield, flags)

#undef CALL_ORIG
#undef BEFORE

#ifdef __cplusplus
 } /* extern "C" */
#endif
