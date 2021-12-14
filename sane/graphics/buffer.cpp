#include "sane/graphics/buffer.hpp"

#include <glad/gl.h>

#include "sane/logging/log.hpp"

namespace Sane
{
  Buffer::Buffer(int32_t target) : target_(target)
  {
    glGenBuffers(1, &buffer_);
    SANE_DEBUG("Created {} buffer: {}", target_, buffer_);
  }

  Buffer::~Buffer()
  {
    glDeleteBuffers(1, &buffer_);
    SANE_DEBUG("Destroyed {} buffer: {}", target_, buffer_);
  }

  void Buffer::Bind()
  {
    glBindBuffer(target_, buffer_);
  }

  void Buffer::Unbind()
  {
    glBindBuffer(target_, 0);
  }

  void Buffer::BufferData(int64_t size, const void* data, int32_t usage)
  {
    glBindBuffer(target_, buffer_);
    glBufferData(target_, (size_t)size, data, usage);
  }
}