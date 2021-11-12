#pragma once

#include <cstdint>
#include <vector>

namespace Sane
{
  class ShaderProgram
  {
  public:
    ShaderProgram();
    ShaderProgram(const char* VS_Contents, const char* FS_Contents);
    ~ShaderProgram();

    ShaderProgram& Finalize();
    void AddShader(const char* contents, uint32_t type);
    ShaderProgram& AddVertexShader(const char* contents);
    ShaderProgram& AddFragmentShader(const char* contents);
    ShaderProgram& AddTessControlShader(const char* contents);
    ShaderProgram& AddTessEvaluationShader(const char* contents);
    ShaderProgram& AddGeometryShader(const char* contents);

    void Bind();
    void Unbind();

    const uint32_t GetUniformLocaition(const char* name);
    const uint32_t GetAttribLocation(const char* name);

    operator uint32_t() { return program_; }

  private:
    uint32_t program_;
    std::vector<uint32_t> shaders_;
  };

  class VertexAttrib
  {
  public:
    VertexAttrib(int32_t location, int32_t size, int32_t type, bool normalized, int32_t stride, const void* pointer);
    ~VertexAttrib();

    void Enable();
    void Disable();

  private:
    int32_t location;
    int32_t size;
    uint32_t type;
    bool normalized;
    int32_t stride;
    const void* pointer;
  };
}