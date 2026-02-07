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
  LOG_ASSERT(input, "Failed input is nullptr");
  renderData = renderDataPtr;

  if (renderData)
  {
    renderData->gameCamera.zoom = 1.0f;
    renderData->gameCamera.dimentions = {(float)input->size.x, (float)input->size.y};

    renderData->uiCamera.zoom = 1.0f;
    renderData->uiCamera.dimentions = {(float)input->size.x, (float)input->size.y};
  }

  glLoadFunc();

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(&gl_debug_callback, nullptr);

  // --- Compile Shaders ---
  GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  int fileSize = 0;
  char *vertShader = read_file("assets/shaders/quad.vert", &fileSize, transientStorage);
  char *fragShader = read_file("assets/shaders/quad.frag", &fileSize, transientStorage);
  LOG_ASSERT(vertShader && fragShader, "Failed to load shaders");

  glShaderSource(vertShaderID, 1, &vertShader, nullptr);
  glShaderSource(fragShaderID, 1, &fragShader, nullptr);

  glCompileShader(vertShaderID);
  glCompileShader(fragShaderID);

  auto checkShader = [](GLuint shader, const char *name)
  {
    int success;
    char log[2048] = {};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 2048, nullptr, log);
      LOG_ERROR("Failed to compile %s: %s", name, log);
    }
    LOG_ASSERT(success, "Shader compilation failed");
  };

  checkShader(vertShaderID, "Vertex Shader");
  checkShader(fragShaderID, "Fragment Shader");

  // --- Link Program ---
  glContext.programID = glCreateProgram();
  glAttachShader(glContext.programID, vertShaderID);
  glAttachShader(glContext.programID, fragShaderID);
  glLinkProgram(glContext.programID);

  {
    int success;
    char log[2048] = {};
    glGetProgramiv(glContext.programID, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(glContext.programID, 2048, nullptr, log);
      LOG_ERROR("Failed to link program: %s", log);
    }
    LOG_ASSERT(success, "Program linking failed");
  }

  glDetachShader(glContext.programID, vertShaderID);
  glDetachShader(glContext.programID, fragShaderID);
  glDeleteShader(vertShaderID);
  glDeleteShader(fragShaderID);

  glValidateProgram(glContext.programID);

  {
    int success;
    char log[2048] = {};
    glGetProgramiv(glContext.programID, GL_VALIDATE_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(glContext.programID, 2048, nullptr, log);
      LOG_ERROR("Failed to validate program: %s", log);
    }
    LOG_ASSERT(success, "Program validation failed");
  }

  // --- VAO ---
  glGenVertexArrays(1, &glContext.VAO);
  glBindVertexArray(glContext.VAO);

  // --- Texture ---
  int w, h, ch;
  char *data = (char *)stbi_load("assets/textures/sample.png", &w, &h, &ch, 4);
  LOG_ASSERT(data, "Failed to load texture");

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

  // --- Transform SSBO ---
  glGenBuffers(1, &glContext.transformSBOID);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, glContext.transformSBOID);

  GLsizeiptr initialCapacity = sizeof(Transform) * 1024; // reserve enough for dynamic transforms
  glBufferData(GL_SHADER_STORAGE_BUFFER, initialCapacity, nullptr, GL_DYNAMIC_DRAW);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glContext.transformSBOID);

  glUseProgram(glContext.programID);

  GLint loc0 = glGetUniformLocation(glContext.programID, "textureAtlas");
  glUniform1i(loc0, 0);

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

    // Clear screen
    float gamma = 1.0f;
    Color bg_color = {0.01f, 0.01f, 0.01f, 1.0f};
    glClearColor(pow(bg_color.r, gamma), pow(bg_color.g, gamma), pow(bg_color.b, gamma), bg_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, input->size.x, input->size.y);

    glUseProgram(glContext.programID);

    // Upload camera matrix
    glUniformMatrix4fv(glContext.orthoProjectionID, 1, GL_FALSE, renderData->gameCamera.matrix().m);

    // Upload screen size
    vec2 screen_size = {(float)input->size.x, (float)input->size.y};
    glUniform2fv(glContext.screenSizeID, 1, &screen_size.x);

    glBindVertexArray(glContext.VAO);
    glBindTexture(GL_TEXTURE_2D, glContext.textureID);

    size_t transformCount = renderData->transforms.size();
    if (transformCount > 0)
    {
        // Upload transforms to GPU
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, glContext.transformSBOID);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                        sizeof(Transform) * transformCount,
                        renderData->transforms.data());

        // Draw instanced quads
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, transformCount);

        // Clear for next frame
        renderData->transforms.clear();
    }

    glBindVertexArray(0);
}
