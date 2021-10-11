#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "sane/events/common.hpp"

namespace Sane
{
  class Display : public Events::Dispatcher
  {
  public:
    Display(const char* name, int32_t width, int32_t height);
    ~Display();

    void Update();
    inline bool IsRunning() { return !glfwWindowShouldClose(window_); }

    glm::mat4 GetOrthoProjection();
    glm::mat4 GetPersProjection();

    operator GLFWwindow* () { return window_; }

    friend void resize_forwarder(GLFWwindow* window, int width, int height);

  private:
    GLFWwindow* window_;
    uint32_t vao_;
  };
}