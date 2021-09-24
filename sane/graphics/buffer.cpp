#include "sane/graphics/buffer.hpp"

#include <sane/debugging/logging.hpp>

namespace Sane
{
  Buffer::Buffer(GLenum target) : target(target)
  {
    glGenBuffers(1, &buffer);
    SANE_INFO("Successfully created buffer of type: {0}", target);
  }

  Buffer::~Buffer()
  {
    glDeleteBuffers(1, &buffer);
  }

  void Buffer::Bind()
  {
    glBindBuffer(target, buffer);
  }

  void Buffer::Unbind()
  {
    glBindBuffer(target, 0);
  }

  void Buffer::BufferData(GLsizeiptr size, const void* data, GLenum usage)
  {
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, usage);
    // SANE_INFO("Buffered data for buffer: {0}", buffer);
  }
}