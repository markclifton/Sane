#include "sane/graphics/shaderprogram.hpp"

#include <string>
#include <vector>

#include "sane/debugging/logging.hpp"

namespace Sane
{
  ShaderProgram::ShaderProgram(const char* VS_Contents, const char* FS_Contents)
  {
    GLint compiled;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &VS_Contents, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint log_length;
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> v(static_cast<size_t>(log_length));
      glGetShaderInfoLog(vertex_shader, log_length, nullptr, v.data());
      std::string err(begin(v), end(v));

      err.pop_back();
      err.pop_back();

      glDeleteShader(vertex_shader);
      SANE_FATAL("Failed to compile vertex shader: " << err);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &FS_Contents, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint log_length;
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> v(static_cast<size_t>(log_length));
      glGetShaderInfoLog(fragment_shader, log_length, nullptr, v.data());
      std::string err(begin(v), end(v));

      err.pop_back();
      err.pop_back();

      glDeleteShader(fragment_shader);
      SANE_FATAL("Failed to compile fragment shader: " << err);
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }

  ShaderProgram::~ShaderProgram()
  {
    glDeleteProgram(program);
  }

  void ShaderProgram::Bind()
  {
    glUseProgram(program);
  }

  void ShaderProgram::Unbind()
  {
    glUseProgram(0);
  }

  const GLuint ShaderProgram::GetUniformLocaition(const char* name)
  {
    return glGetUniformLocation(program, name);
  }

  const GLuint ShaderProgram::GetAttribLocation(const char* name)
  {
    return glGetAttribLocation(program, name);
  }

  VertexAttrib::VertexAttrib(GLint location, GLint size, GLenum type,
    GLboolean normalized, GLsizei stride,
    const void* pointer)
    : location(location), size(size), type(type), normalized(normalized),
    stride(stride), pointer(pointer) {}

  VertexAttrib::~VertexAttrib()
  {
  }

  void VertexAttrib::Enable()
  {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, normalized, stride, pointer);
  }

  void VertexAttrib::Disable()
  {
    glDisableVertexAttribArray(location);
  }
}