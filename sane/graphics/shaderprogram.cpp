#include "sane/graphics/shaderprogram.hpp"

#include <glad/gl.h>

#include "sane/logging/log.hpp"

namespace Sane
{
  ShaderProgram::ShaderProgram(const char* VS_Contents, const char* FS_Contents)
  {
    GLint compiled;

    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &VS_Contents, NULL);
    glCompileShader(vertex_shader_);
    glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint log_length;
      glGetShaderiv(vertex_shader_, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> v(static_cast<size_t>(log_length));
      glGetShaderInfoLog(vertex_shader_, log_length, nullptr, v.data());
      std::string err(begin(v), end(v));

      err.pop_back();
      err.pop_back();

      glDeleteShader(vertex_shader_);
      SANE_ERROR("Failed to compile vertex shader: {}", err);
    }

    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_, 1, &FS_Contents, NULL);
    glCompileShader(fragment_shader_);
    glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint log_length;
      glGetShaderiv(fragment_shader_, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> v(static_cast<size_t>(log_length));
      glGetShaderInfoLog(fragment_shader_, log_length, nullptr, v.data());
      std::string err(begin(v), end(v));

      err.pop_back();
      err.pop_back();

      glDeleteShader(fragment_shader_);
      SANE_ERROR("Failed to compile fragment shader: {}", err);
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader_);
    glAttachShader(program_, fragment_shader_);
    glLinkProgram(program_);

    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);

    SANE_INFO("Created shader program: {}", program_);
  }

  ShaderProgram::~ShaderProgram()
  {
    glDeleteProgram(program_);
    SANE_INFO("Destroyed shader program: {}", program_);
  }

  void ShaderProgram::Bind()
  {
    glUseProgram(program_);
  }

  void ShaderProgram::Unbind()
  {
    glUseProgram(0);
  }

  const GLuint ShaderProgram::GetUniformLocaition(const char* name)
  {
    return glGetUniformLocation(program_, name);
  }

  const GLuint ShaderProgram::GetAttribLocation(const char* name)
  {
    return glGetAttribLocation(program_, name);
  }

  VertexAttrib::VertexAttrib(int32_t location, int32_t size, int32_t type, bool normalized, int32_t stride, const void* pointer)
    : location(location), size(size), type(type), normalized(normalized), stride(stride), pointer(pointer)
  {}

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