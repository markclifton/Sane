#pragma once

#include <cstdint>

namespace Sane
{
  class Buffer
  {
  public:
    Buffer(int32_t target);
    ~Buffer();

    void Bind();
    void Unbind();
    void BufferData(int64_t size, const void* data, int32_t usage);

    operator uint32_t() { return buffer_; }

  private:
    uint32_t buffer_;
    int32_t target_;
  };
}