#pragma once

#include <glad/gl.h>

#include "sane/utils/noncopyable.hpp"

namespace Sane {
  class ShaderProgram : public NonCopyable {
  public:
    ShaderProgram(const char* VS_Contents, const char* FS_Contents);
    ~ShaderProgram();

    void Bind();

    const GLuint GetUniformLocaition(const char* name);
    const GLuint GetAttribLocation(const char* name);

    operator GLuint() { return program; }

  private:
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
  };

  class VertexAttrib {
  public:
    VertexAttrib(GLint location, GLint size, GLenum type, GLboolean normalized,
      GLsizei stride, const void* pointer);
    ~VertexAttrib();

    void Enable();

  private:
    GLint location;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void* pointer;
  };
}