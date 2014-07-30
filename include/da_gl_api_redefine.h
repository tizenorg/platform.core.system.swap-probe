/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2014 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
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
 * - Samsung RnD Institute Russia
 *
 */

#define glActiveTexture __local_glActiveTexture
#define glAttachShader __local_glAttachShader
#define glBindAttribLocation __local_glBindAttribLocation
#define glBindBuffer __local_glBindBuffer
#define glBindFramebuffer __local_glBindFramebuffer
#define glBindRenderbuffer __local_glBindRenderbuffer
#define glBindTexture __local_glBindTexture
#define glBlendColor __local_glBlendColor
#define glBlendEquation __local_glBlendEquation
#define glBlendEquationSeparate __local_glBlendEquationSeparate
#define glBlendFunc __local_glBlendFunc
#define glBlendFuncSeparate __local_glBlendFuncSeparate
#define glBufferData __local_glBufferData
#define glBufferSubData __local_glBufferSubData
#define glCheckFramebufferStatus __local_glCheckFramebufferStatus
#define glClear __local_glClear
#define glClearColor __local_glClearColor
#define glClearDepthf __local_glClearDepthf
#define glClearStencil __local_glClearStencil
#define glColorMask __local_glColorMask
#define glCompileShader __local_glCompileShader
#define glCompressedTexImage2D __local_glCompressedTexImage2D
#define glCompressedTexSubImage2D __local_glCompressedTexSubImage2D
#define glCopyTexImage2D __local_glCopyTexImage2D
#define glCopyTexSubImage2D __local_glCopyTexSubImage2D
#define glCreateProgram __local_glCreateProgram
#define glCreateShader __local_glCreateShader
#define glCullFace __local_glCullFace
#define glDeleteBuffers __local_glDeleteBuffers
#define glDeleteFramebuffers __local_glDeleteFramebuffers
#define glDeleteProgram __local_glDeleteProgram
#define glDeleteRenderbuffers __local_glDeleteRenderbuffers
#define glDeleteShader __local_glDeleteShader
#define glDeleteTextures __local_glDeleteTextures
#define glDepthFunc __local_glDepthFunc
#define glDepthMask __local_glDepthMask
#define glDepthRangef __local_glDepthRangef
#define glDetachShader __local_glDetachShader
#define glDisable __local_glDisable
#define glDisableVertexAttribArray __local_glDisableVertexAttribArray
#define glDrawArrays __local_glDrawArrays
#define glDrawElements __local_glDrawElements
#define glEnable __local_glEnable
#define glEnableVertexAttribArray __local_glEnableVertexAttribArray
#define glFinish __local_glFinish
#define glFlush __local_glFlush
#define glFramebufferRenderbuffer __local_glFramebufferRenderbuffer
#define glFramebufferTexture2D __local_glFramebufferTexture2D
#define glFrontFace __local_glFrontFace
#define glGenBuffers __local_glGenBuffers
#define glGenFramebuffers __local_glGenFramebuffers
#define glGenRenderbuffers __local_glGenRenderbuffers
#define glGenTextures __local_glGenTextures
#define glGenerateMipmap __local_glGenerateMipmap
#define glGetBooleanv __local_glGetBooleanv
#define glGetFloatv __local_glGetFloatv
#define glGetIntegerv __local_glGetIntegerv
#define glGetActiveAttrib __local_glGetActiveAttrib
#define glGetActiveUniform __local_glGetActiveUniform
#define glGetAttachedShaders __local_glGetAttachedShaders
#define glGetAttribLocation __local_glGetAttribLocation
#define glGetBufferParameteriv __local_glGetBufferParameteriv
#define glGetError __local_glGetError
#define glGetFramebufferAttachmentParameteriv __local_glGetFramebufferAttachmentParameteriv
#define glGetProgramInfoLog __local_glGetProgramInfoLog
#define glGetProgramiv __local_glGetProgramiv
#define glGetRenderbufferParameteriv __local_glGetRenderbufferParameteriv
#define glGetShaderInfoLog __local_glGetShaderInfoLog
#define glGetShaderPrecisionFormat __local_glGetShaderPrecisionFormat
#define glGetShaderSource __local_glGetShaderSource
#define glGetShaderiv __local_glGetShaderiv
#define glGetString __local_glGetString
#define glGetTexParameterfv __local_glGetTexParameterfv
#define glGetTexParameteriv __local_glGetTexParameteriv
#define glGetUniformfv __local_glGetUniformfv
#define glGetUniformiv __local_glGetUniformiv
#define glGetUniformLocation __local_glGetUniformLocation
#define glGetVertexAttribfv __local_glGetVertexAttribfv
#define glGetVertexAttribiv __local_glGetVertexAttribiv
#define glGetVertexAttribPointerv __local_glGetVertexAttribPointerv
#define glHint __local_glHint
#define glIsBuffer __local_glIsBuffer
#define glIsEnabled __local_glIsEnabled
#define glIsFramebuffer __local_glIsFramebuffer
#define glIsProgram __local_glIsProgram
#define glIsRenderbuffer __local_glIsRenderbuffer
#define glIsShader __local_glIsShader
#define glIsTexture __local_glIsTexture
#define glLineWidth __local_glLineWidth
#define glLinkProgram __local_glLinkProgram
#define glPixelStorei __local_glPixelStorei
#define glPolygonOffset __local_glPolygonOffset
#define glReadPixels __local_glReadPixels
#define glReleaseShaderCompiler __local_glReleaseShaderCompiler
#define glRenderbufferStorage __local_glRenderbufferStorage
#define glSampleCoverage __local_glSampleCoverage
#define glScissor __local_glScissor
#define glShaderBinary __local_glShaderBinary
#define glShaderSource __local_glShaderSource
#define glStencilFunc __local_glStencilFunc
#define glStencilFuncSeparate __local_glStencilFuncSeparate
#define glStencilMask __local_glStencilMask
#define glStencilMaskSeparate __local_glStencilMaskSeparate
#define glStencilOp __local_glStencilOp
#define glStencilOpSeparate __local_glStencilOpSeparate
#define glTexImage2D __local_glTexImage2D
#define glTexParameterf __local_glTexParameterf
#define glTexParameterfv __local_glTexParameterfv
#define glTexParameteri __local_glTexParameteri
#define glTexParameteriv __local_glTexParameteriv
#define glTexSubImage2D __local_glTexSubImage2D
#define glUniform1f __local_glUniform1f
#define glUniform2f __local_glUniform2f
#define glUniform3f __local_glUniform3f
#define glUniform4f __local_glUniform4f
#define glUniform1fv __local_glUniform1fv
#define glUniform2fv __local_glUniform2fv
#define glUniform3fv __local_glUniform3fv
#define glUniform4fv __local_glUniform4fv
#define glUniform1i __local_glUniform1i
#define glUniform2i __local_glUniform2i
#define glUniform3i __local_glUniform3i
#define glUniform4i __local_glUniform4i
#define glUniform1iv __local_glUniform1iv
#define glUniform2iv __local_glUniform2iv
#define glUniform3iv __local_glUniform3iv
#define glUniform4iv __local_glUniform4iv
#define glUniformMatrix2fv __local_glUniformMatrix2fv
#define glUniformMatrix3fv __local_glUniformMatrix3fv
#define glUniformMatrix4fv __local_glUniformMatrix4fv
#define glUseProgram __local_glUseProgram
#define glValidateProgram __local_glValidateProgram
#define glVertexAttrib1f __local_glVertexAttrib1f
#define glVertexAttrib2f __local_glVertexAttrib2f
#define glVertexAttrib3f __local_glVertexAttrib3f
#define glVertexAttrib4f __local_glVertexAttrib4f
#define glVertexAttrib1fv __local_glVertexAttrib1fv
#define glVertexAttrib2fv __local_glVertexAttrib2fv
#define glVertexAttrib3fv __local_glVertexAttrib3fv
#define glVertexAttrib4fv __local_glVertexAttrib4fv
#define glVertexAttribPointer __local_glVertexAttribPointer
#define glViewport __local_glViewport
