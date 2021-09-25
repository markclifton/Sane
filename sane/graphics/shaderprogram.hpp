#pragma once

#include <cstdint>

namespace Sane
{
  class ShaderProgram
  {
  public:
    ShaderProgram(const char* VS_Contents, const char* FS_Contents);
    ~ShaderProgram();

    void Bind();
    void Unbind();

    const uint32_t GetUniformLocaition(const char* name);
    const uint32_t GetAttribLocation(const char* name);

    operator uint32_t() { return program_; }

  private:
    uint32_t program_;
    uint32_t vertex_shader_;
    uint32_t fragment_shader_;
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