#pragma once

#include "GL/glcorearb.h"
#include <utils.h>

static PFNGLCREATEPROGRAMPROC glCreateProgram_ptr;

void glLoadFunc()
{
    PROC proc = wglGetProcAddress("glCreateProgram");
    if (!proc)
    {
        LOG_ASSERT(false, "Failed to load gl function %s", "glCreateProgram");
    }

    glCreateProgram_ptr = (PFNGLCREATEPROGRAMPROC)LoadGLFunc((char*)"glCreateProgram");
}

GLAPI GLuint APIENTRY glCreateProgram(void)
{
    return glCreateProgram_ptr();
}