#include "sane/graphics/buffer.hpp"

namespace Sane {
  Buffer::Buffer(GLenum target) : target(target) { glGenBuffers(1, &buffer); }

  Buffer::~Buffer() { glDeleteBuffers(1, &buffer); }

  void Buffer::Bind() { glBindBuffer(target, buffer); }

  void Buffer::BufferData(GLsizeiptr size, const void* data, GLenum usage) {
    glBufferData(target, size, data, usage);
  }

}