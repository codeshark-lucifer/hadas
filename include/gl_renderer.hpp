#include "gl_renderer.h"
#include <config.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils/mathf.h"
#include "interface.h"

struct GLContext
{
  GLuint programID;
  GLuint textureID;
  GLuint VAO;
  GLuint transformSBOID;
  //   GLuint materialSBOID;
  GLuint screenSizeID;
  GLuint orthoProjectionID;
  //   GLuint fontAtlasID;

  //   long long textureTimestamp;
  //   long long shaderTimestamp;
};

static GLContext glContext;

static void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                       GLsizei length, const GLchar *message, const void *user)
{
  if (
      severity == GL_DEBUG_SEVERITY_LOW ||
      severity == GL_DEBUG_SEVERITY_MEDIUM ||
      severity == GL_DEBUG_SEVERITY_HIGH)
  {
    LOG_ASSERT(0, "OpenGL Error: %s", message);
  }
  else
  {
    LOG_DEBUG((char *)message);
  }
}

bool glInit(BumpAllocator *transientStorage, RenderData *renderDataPtr)
{
  renderData = renderDataPtr;
  glLoadFunc();

  glDebugMessageCallback(&gl_debug_callback, nullptr);

  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glEnable(GL_DEBUG_OUTPUT);

  GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  int fileSize = 0;
  char *vertShader = read_file(const_cast<char *>("assets/shaders/quad.vert"), &fileSize, transientStorage);
  char *fragShader = read_file(const_cast<char *>("assets/shaders/quad.frag"), &fileSize, transientStorage);

  if (!vertShader || !fragShader)
  {
    LOG_ASSERT(false, "Failed to load shaders");
    return false;
  }

  glShaderSource(vertShaderID, 1, &vertShader, 0);
  glShaderSource(fragShaderID, 1, &fragShader, 0);

  glCompileShader(vertShaderID);
  glCompileShader(fragShaderID);

  {
    int success;
    char log[2048] = {};

    glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertShaderID, 2048, 0, log);
      LOG_ERROR("Failed to compile Vertex Shaders %s", log);
      return false;
    }
  }

  {
    int success;
    char log[2048] = {};

    glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragShaderID, 2048, 0, log);
      LOG_ERROR("Failed to compile Fragment Shaders %s", log);
      return false;
    }
  }

  glContext.programID = glCreateProgram();

  glAttachShader(glContext.programID, vertShaderID);
  glAttachShader(glContext.programID, fragShaderID);
  glLinkProgram(glContext.programID);

  { // Program link check
    int success;
    char log[2048] = {};

    glGetProgramiv(glContext.programID, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(glContext.programID, 2048, 0, log);
      LOG_ERROR("Failed to link program: %s", log);
      return false;
    }
  }

  glDetachShader(glContext.programID, vertShaderID);
  glDetachShader(glContext.programID, fragShaderID);
  glDeleteShader(vertShaderID);
  glDeleteShader(fragShaderID);

  glValidateProgram(glContext.programID); // Add this call
  {                                       // Program validation check
    int success;
    char log[2048] = {};

    glGetProgramiv(glContext.programID, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(glContext.programID, 2048, 0, log);
      LOG_ERROR("Failed to validate program: %s", log);
      return false;
    }
  }

  glGenVertexArrays(1, &glContext.VAO);
  glBindVertexArray(glContext.VAO);

  {
    int w, h, ch;
    char *data = (char *)stbi_load("assets/textures/sample.png", &w, &h, &ch, 4);
    if (!data)
    {
      LOG_ASSERT(false, "Failed Load Textures");
      return false;
    }

    glGenTextures(1, &glContext.textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glContext.textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8,
                 w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  {
    // Transform Storage Buffer
    glGenBuffers(1, &glContext.transformSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Transform) * MAX_TRANSFORMS,
                 renderData->transforms, GL_DYNAMIC_DRAW);
  }
  glUseProgram(glContext.programID);

  GLint loc0 = glGetUniformLocation(glContext.programID, "textureAtlas");
  glUniform1i(loc0, 0); // texture unit 0

  glContext.orthoProjectionID = glGetUniformLocation(glContext.programID, "projection");
  glContext.screenSizeID = glGetUniformLocation(glContext.programID, "screenSize");
  vec2 screen_size = {(float)input->size.x, (float)input->size.y};
  glUniform2fv(glContext.screenSizeID, 1, &screen_size.x);

  glBindVertexArray(0);

  return true;
}

void glRender()
{
  glEnable(GL_FRAMEBUFFER_SRGB);
  glDisable(0x809D); // disable multi sample

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_GREATER);
  glClearDepth(0.0f);

  float gamma = 1.0f;
  Color bg_color = {0.01f, 0.01f, 0.01f, 1.0f};
  glClearColor(pow(bg_color.r, gamma), pow(bg_color.g, gamma), pow(bg_color.b, gamma), bg_color.a);
  glClearDepth(0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, input->size.x, input->size.y);

  glUseProgram(glContext.programID);
  glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, renderData->gameCamera.matrix().m);

  vec2 screen_size = {(float)input->size.x, (float)input->size.y};
  glUniform2fv(glContext.screenSizeID, 1, &screen_size.x);

  glBindVertexArray(glContext.VAO);
  glBindTexture(GL_TEXTURE_2D, glContext.textureID);

  {
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Transform) * renderData->transformCount,
                    renderData->transforms);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData->transformCount);

    renderData->transformCount = 0;
  }

  glBindVertexArray(0);
}