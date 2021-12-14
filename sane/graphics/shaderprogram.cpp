#include "sane/graphics/shaderprogram.hpp"

#include <glad/gl.h>

#include "sane/logging/log.hpp"

namespace Sane
{
  ShaderProgram::ShaderProgram()
  {
    program_ = glCreateProgram();
  }

  ShaderProgram::ShaderProgram(const char* VS_Contents, const char* FS_Contents)
    : ShaderProgram()
  {
    AddVertexShader(VS_Contents);
    AddFragmentShader(FS_Contents);
    Finalize();
  }

  ShaderProgram::~ShaderProgram()
  {
    glDeleteProgram(program_);
    SANE_DEBUG("Destroyed shader program: {}", program_);
  }

  ShaderProgram& ShaderProgram::Finalize()
  {
    if (shaders_.empty())
      return *this;

    glLinkProgram(program_);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glGetProgramiv(program_, GL_LINK_STATUS, &Success);
    if (Success == 0) {
      glGetProgramInfoLog(program_, sizeof(ErrorLog), NULL, ErrorLog);
      SANE_ERROR("Error linking shader program: {}", ErrorLog);
      return *this;
    }

    glValidateProgram(program_);
    glGetProgramiv(program_, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
      glGetProgramInfoLog(program_, sizeof(ErrorLog), NULL, ErrorLog);
      SANE_ERROR("Invalid shader program: {}", ErrorLog);
    }

    for (auto shader : shaders_)
      glDeleteShader(shader);

    shaders_.clear();

    return *this;
  }

  void ShaderProgram::AddShader(const char* contents, uint32_t type)
  {
    uint32_t shader = glCreateShader(type);
    shaders_.push_back(shader);

    glShaderSource(shader, 1, &contents, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
      GLint log_length;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
      std::vector<char> v(static_cast<size_t>(log_length));
      glGetShaderInfoLog(shader, log_length, nullptr, v.data());
      std::string err(begin(v), end(v));

      err.pop_back();
      err.pop_back();

      glDeleteShader(shader);
      SANE_ERROR("Failed to compile {} shader: {}", type, err);
    }

    glAttachShader(program_, shader);
  }

  ShaderProgram& ShaderProgram::AddVertexShader(const char* contents)
  {
    AddShader(contents, GL_VERTEX_SHADER);
    return *this;
  }

  ShaderProgram& ShaderProgram::AddFragmentShader(const char* contents)
  {
    AddShader(contents, GL_FRAGMENT_SHADER);
    return *this;
  }

  ShaderProgram& ShaderProgram::AddTessControlShader(const char* contents)
  {
    AddShader(contents, GL_TESS_CONTROL_SHADER);
    return *this;
  }

  ShaderProgram& ShaderProgram::AddTessEvaluationShader(const char* contents)
  {
    AddShader(contents, GL_TESS_EVALUATION_SHADER);
    return *this;
  }

  ShaderProgram& ShaderProgram::AddGeometryShader(const char* contents)
  {
    AddShader(contents, GL_GEOMETRY_SHADER);
    return *this;
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