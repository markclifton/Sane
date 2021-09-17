#pragma once

#include <glad/gl.h>

#include "sane/utils/noncopyable.hpp"

namespace Sane {
  class Buffer : public NonCopyable {
  public:
    Buffer(GLenum target);
    ~Buffer();

    void Bind();
    void BufferData(GLsizeiptr size, const void* data, GLenum usage);

    operator GLuint() { return buffer; }

  private:
    GLuint buffer;
    GLenum target;
  };
}