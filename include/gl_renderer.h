#pragma once

#include "GL/glcorearb.h"
#include "platform.h"
#include "utils.h"
#include "render_types.h"

static PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;
static PFNGLDELETETEXTURESPROC glDeleteTextures_ptr;
static PFNGLGENTEXTURESPROC glGenTextures_ptr;
static PFNGLBINDTEXTUREPROC glBindTexture_ptr;
static PFNGLDRAWBUFFERPROC glDrawBuffer_ptr;
static PFNGLDRAWARRAYSPROC glDrawArrays_ptr;
static PFNGLCREATESHADERPROC glCreateShader_ptr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation_ptr;
static PFNGLUNIFORM1FPROC glUniform1f_ptr;
static PFNGLUNIFORM2FVPROC glUniform2fv_ptr;
static PFNGLUNIFORM3FVPROC glUniform3fv_ptr;
static PFNGLUNIFORM1IPROC glUniform1i_ptr;
static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv_ptr;
static PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor_ptr;
static PFNGLACTIVETEXTUREPROC glActiveTexture_ptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData_ptr;
static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced_ptr;
static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer_ptr;
static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus_ptr;
static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers_ptr;
static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D_ptr;
static PFNGLDRAWBUFFERSPROC glDrawBuffers_ptr;
static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers_ptr;
static PFNGLBLENDFUNCIPROC glBlendFunci_ptr;
static PFNGLBLENDEQUATIONPROC glBlendEquation_ptr;
static PFNGLCLEARBUFFERFVPROC glClearBufferfv_ptr;
static PFNGLSHADERSOURCEPROC glShaderSource_ptr;
static PFNGLCOMPILESHADERPROC glCompileShader_ptr;
static PFNGLGETSHADERIVPROC glGetShaderiv_ptr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog_ptr;
static PFNGLATTACHSHADERPROC glAttachShader_ptr;
static PFNGLLINKPROGRAMPROC glLinkProgram_ptr;
static PFNGLVALIDATEPROGRAMPROC glValidateProgram_ptr;
static PFNGLGETPROGRAMIVPROC glGetProgramiv_ptr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog_ptr;
static PFNGLGENBUFFERSPROC glGenBuffers_ptr;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays_ptr;
static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation_ptr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray_ptr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray_ptr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer_ptr;
static PFNGLBINDBUFFERPROC glBindBuffer_ptr;
static PFNGLBINDBUFFERBASEPROC glBindBufferBase_ptr;
static PFNGLBUFFERDATAPROC glBufferData_ptr;
static PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv_ptr;
static PFNGLUSEPROGRAMPROC glUseProgram_ptr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays_ptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers_ptr;
static PFNGLDELETEPROGRAMPROC glDeleteProgram_ptr;
static PFNGLDETACHSHADERPROC glDetachShader_ptr;
static PFNGLDELETESHADERPROC glDeleteShader_ptr;
static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced_ptr;
static PFNGLGENERATEMIPMAPPROC glGenerateMipmap_ptr;
static PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback_ptr;

void glLoadFunc()
{
    PROC proc = wglGetProcAddress("glCreateProgram");
    if (!proc)
    {
        LOG_ASSERT(false, "Failed to load gl function %s", "glCreateProgram");
    }

    glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC)LoadGLFunc((const char*)"glCreateProgram");
    glDeleteTextures_ptr = (PFNGLDELETETEXTURESPROC)LoadGLFunc((const char*)"glDeleteTextures");
    glGenTextures_ptr = (PFNGLGENTEXTURESPROC)LoadGLFunc((const char*)"glGenTextures");
    glBindTexture_ptr = (PFNGLBINDTEXTUREPROC)LoadGLFunc((const char*)"glBindTexture");
    glDrawArrays_ptr = (PFNGLDRAWARRAYSPROC)LoadGLFunc((const char*)"glDrawArrays");
    glCreateShader_ptr = (PFNGLCREATESHADERPROC)LoadGLFunc((const char*)"glCreateShader");
    glGetUniformLocation_ptr = (PFNGLGETUNIFORMLOCATIONPROC)LoadGLFunc((const char*)"glGetUniformLocation");
    glUniform1f_ptr = (PFNGLUNIFORM1FPROC)LoadGLFunc((const char*)"glUniform1f");
    glUniform2fv_ptr = (PFNGLUNIFORM2FVPROC)LoadGLFunc((const char*)"glUniform2fv");
    glUniform3fv_ptr = (PFNGLUNIFORM3FVPROC)LoadGLFunc((const char*)"glUniform3fv");
    glUniform1i_ptr = (PFNGLUNIFORM1IPROC)LoadGLFunc((const char*)"glUniform1i");
    glUniformMatrix4fv_ptr = (PFNGLUNIFORMMATRIX4FVPROC)LoadGLFunc((const char*)"glUniformMatrix4fv");
    glVertexAttribDivisor_ptr = (PFNGLVERTEXATTRIBDIVISORPROC)LoadGLFunc((const char*)"glVertexAttribDivisor");
    glActiveTexture_ptr = (PFNGLACTIVETEXTUREPROC)LoadGLFunc((const char*)"glActiveTexture");
    glBufferSubData_ptr = (PFNGLBUFFERSUBDATAPROC)LoadGLFunc((const char*)"glBufferSubData");
    glDrawArraysInstanced_ptr = (PFNGLDRAWARRAYSINSTANCEDPROC)LoadGLFunc((const char*)"glDrawArraysInstanced");
    glBindFramebuffer_ptr = (PFNGLBINDFRAMEBUFFERPROC)LoadGLFunc((const char*)"glBindFramebuffer");
    glCheckFramebufferStatus_ptr = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)LoadGLFunc((const char*)"glCheckFramebufferStatus");
    glGenFramebuffers_ptr = (PFNGLGENFRAMEBUFFERSPROC)LoadGLFunc((const char*)"glGenFramebuffers");
    glFramebufferTexture2D_ptr = (PFNGLFRAMEBUFFERTEXTURE2DPROC)LoadGLFunc((const char*)"glFramebufferTexture2D");
    glDrawBuffers_ptr = (PFNGLDRAWBUFFERSPROC)LoadGLFunc((const char*)"glDrawBuffers");
    glDeleteFramebuffers_ptr = (PFNGLDELETEFRAMEBUFFERSPROC)LoadGLFunc((const char*)"glDeleteFramebuffers");
    glBlendFunci_ptr = (PFNGLBLENDFUNCIPROC)LoadGLFunc((const char*)"glBlendFunci");
    glBlendEquation_ptr = (PFNGLBLENDEQUATIONPROC)LoadGLFunc((const char*)"glBlendEquation");
    glClearBufferfv_ptr = (PFNGLCLEARBUFFERFVPROC)LoadGLFunc((const char*)"glClearBufferfv");
    glShaderSource_ptr = (PFNGLSHADERSOURCEPROC)LoadGLFunc((const char*)"glShaderSource");
    glCompileShader_ptr = (PFNGLCOMPILESHADERPROC)LoadGLFunc((const char*)"glCompileShader");
    glGetShaderiv_ptr = (PFNGLGETSHADERIVPROC)LoadGLFunc((const char*)"glGetShaderiv");
    glGetShaderInfoLog_ptr = (PFNGLGETSHADERINFOLOGPROC)LoadGLFunc((const char*)"glGetShaderInfoLog");
    glAttachShader_ptr = (PFNGLATTACHSHADERPROC)LoadGLFunc((const char*)"glAttachShader");
    glLinkProgram_ptr = (PFNGLLINKPROGRAMPROC)LoadGLFunc((const char*)"glLinkProgram");
    glValidateProgram_ptr = (PFNGLVALIDATEPROGRAMPROC)LoadGLFunc((const char*)"glValidateProgram");
    glGetProgramiv_ptr = (PFNGLGETPROGRAMIVPROC)LoadGLFunc((const char*)"glGetProgramiv");
    glGetProgramInfoLog_ptr = (PFNGLGETPROGRAMINFOLOGPROC)LoadGLFunc((const char*)"glGetProgramInfoLog");
    glGenBuffers_ptr = (PFNGLGENBUFFERSPROC)LoadGLFunc((const char*)"glGenBuffers");
    glGenVertexArrays_ptr = (PFNGLGENVERTEXARRAYSPROC)LoadGLFunc((const char*)"glGenVertexArrays");
    glGetAttribLocation_ptr = (PFNGLGETATTRIBLOCATIONPROC)LoadGLFunc((const char*)"glGetAttribLocation");
    glBindVertexArray_ptr = (PFNGLBINDVERTEXARRAYPROC)LoadGLFunc((const char*)"glBindVertexArray");
    glEnableVertexAttribArray_ptr = (PFNGLENABLEVERTEXATTRIBARRAYPROC)LoadGLFunc((const char*)"glEnableVertexAttribArray");
    glVertexAttribPointer_ptr = (PFNGLVERTEXATTRIBPOINTERPROC)LoadGLFunc((const char*)"glVertexAttribPointer");
    glBindBuffer_ptr = (PFNGLBINDBUFFERPROC)LoadGLFunc((const char*)"glBindBuffer");
    glBindBufferBase_ptr = (PFNGLBINDBUFFERBASEPROC)LoadGLFunc((const char*)"glBindBufferBase");
    glBufferData_ptr = (PFNGLBUFFERDATAPROC)LoadGLFunc((const char*)"glBufferData");
    glGetVertexAttribPointerv_ptr = (PFNGLGETVERTEXATTRIBPOINTERVPROC)LoadGLFunc((const char*)"glGetVertexAttribPointerv");
    glUseProgram_ptr = (PFNGLUSEPROGRAMPROC)LoadGLFunc((const char*)"glUseProgram");
    glDeleteVertexArrays_ptr = (PFNGLDELETEVERTEXARRAYSPROC)LoadGLFunc((const char*)"glDeleteVertexArrays");
    glDeleteBuffers_ptr = (PFNGLDELETEBUFFERSPROC)LoadGLFunc((const char*)"glDeleteBuffers");
    glDeleteProgram_ptr = (PFNGLDELETEPROGRAMPROC)LoadGLFunc((const char*)"glDeleteProgram");
    glDetachShader_ptr = (PFNGLDETACHSHADERPROC)LoadGLFunc((const char*)"glDetachShader");
    glDeleteShader_ptr = (PFNGLDELETESHADERPROC)LoadGLFunc((const char*)"glDeleteShader");
    glDrawElementsInstanced_ptr = (PFNGLDRAWELEMENTSINSTANCEDPROC)LoadGLFunc((const char*)"glDrawElementsInstanced");
    glGenerateMipmap_ptr = (PFNGLGENERATEMIPMAPPROC)LoadGLFunc((const char*)"glGenerateMipmap");
    glDebugMessageCallback_ptr = (PFNGLDEBUGMESSAGECALLBACKPROC)LoadGLFunc((const char*)"glDebugMessageCallback");
}

GLAPI GLuint APIENTRY glCreateProgram(void)
{
    return glCreateProgram_ptr();
}

GLAPI void APIENTRY glDeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures_ptr(n, textures);
}

GLAPI void APIENTRY glGenTextures(GLsizei n, GLuint *textures)
{
    glGenTextures_ptr(n, textures);
}

GLAPI void APIENTRY glBindTexture(GLenum target, GLuint texture)
{
    glBindTexture_ptr(target, texture);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays_ptr(mode, first, count);
}

GLuint glCreateShader(GLenum shaderType)
{
    return glCreateShader_ptr(shaderType);
}

GLint glGetUniformLocation(GLuint program, const GLchar *name)
{
    return glGetUniformLocation_ptr(program, name);
}

void glUniform1f(GLint location, GLfloat v0)
{
    glUniform1f_ptr(location, v0);
}

void glUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform2fv_ptr(location, count, value);
}

void glUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform3fv_ptr(location, count, value);
}

void glUniform1i(GLint location, GLint v0)
{
    glUniform1i_ptr(location, v0);
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv_ptr(location, count, transpose, value);
}

void glVertexAttribDivisor(GLuint index, GLuint divisor)
{
    glVertexAttribDivisor_ptr(index, divisor);
}

void glActiveTexture(GLenum texture)
{
    glActiveTexture_ptr(texture);
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
    glBufferSubData_ptr(target, offset, size, data);
}

void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei instanceCount)
{
    glDrawArraysInstanced_ptr(mode, first, count, instanceCount);
}

void glBindFramebuffer(GLenum target, GLuint framebuffer)
{
    glBindFramebuffer_ptr(target, framebuffer);
}

GLenum glCheckFramebufferStatus(GLenum target)
{
    return glCheckFramebufferStatus_ptr(target);
}

void glGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
    glGenFramebuffers_ptr(n, framebuffers);
}

void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
    glFramebufferTexture2D_ptr(target, attachment, textarget, texture, level);
}

void glDrawBuffers(GLsizei n, const GLenum *bufs)
{
    glDrawBuffers_ptr(n, bufs);
}

void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
    glDeleteFramebuffers_ptr(n, framebuffers);
}

void glBlendFunci(GLuint buf, GLenum src, GLenum dst)
{
    glBlendFunci_ptr(buf, src, dst);
}

void glBlendEquation(GLenum mode)
{
    glBlendEquation_ptr(mode);
}

void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value)
{
    glClearBufferfv_ptr(buffer, drawbuffer, value);
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar *const *strings, const GLint *lengths)
{
    glShaderSource_ptr(shader, count, strings, lengths);
}

void glCompileShader(GLuint shader)
{
    glCompileShader_ptr(shader);
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    glGetShaderiv_ptr(shader, pname, params);
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    glGetShaderInfoLog_ptr(shader, bufSize, length, infoLog);
}

void glAttachShader(GLuint program, GLuint shader)
{
    glAttachShader_ptr(program, shader);
}

void glLinkProgram(GLuint program)
{
    glLinkProgram_ptr(program);
}

void glValidateProgram(GLuint program)
{
    glValidateProgram_ptr(program);
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    glGetProgramiv_ptr(program, pname, params);
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    glGetProgramInfoLog_ptr(program, bufSize, length, infoLog);
}

void glGenBuffers(GLsizei n, GLuint *buffers)
{
    glGenBuffers_ptr(n, buffers);
}

void glGenVertexArrays(GLsizei n, GLuint *arrays)
{
    glGenVertexArrays_ptr(n, arrays);
}

GLint glGetAttribLocation(GLuint program, const GLchar *name)
{
    return glGetAttribLocation_ptr(program, name);
}

void glBindVertexArray(GLuint array)
{
    glBindVertexArray_ptr(array);
}

void glEnableVertexAttribArray(GLuint index)
{
    glEnableVertexAttribArray_ptr(index);
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    glVertexAttribPointer_ptr(index, size, type, normalized, stride, pointer);
}

void glBindBuffer(GLenum target, GLuint buffer)
{
    glBindBuffer_ptr(target, buffer);
}

void glBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
    glBindBufferBase_ptr(target, index, buffer);
}

void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
    glBufferData_ptr(target, size, data, usage);
}

void glGetVertexAttribPointerv(GLuint index, GLenum pname, void **pointer)
{
    glGetVertexAttribPointerv_ptr(index, pname, pointer);
}

void glUseProgram(GLuint program)
{
    glUseProgram_ptr(program);
}

void glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
    glDeleteVertexArrays_ptr(n, arrays);
}

void glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
    glDeleteBuffers_ptr(n, buffers);
}

void glDeleteProgram(GLuint program)
{
    glDeleteProgram_ptr(program);
}

void glDetachShader(GLuint program, GLuint shader)
{
    glDetachShader_ptr(program, shader);
}

void glDeleteShader(GLuint shader)
{
    glDeleteShader_ptr(shader);
}

void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
    glDrawElementsInstanced_ptr(mode, count, type, indices, instancecount);
}

void glGenerateMipmap(GLenum target)
{
    glGenerateMipmap_ptr(target);
}

void glDebugMessageCallback(GLDEBUGPROC callback, const void *userParam)
{
    glDebugMessageCallback_ptr(callback, userParam);
}