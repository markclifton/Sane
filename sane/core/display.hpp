#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace Sane
{
  class Display
  {
  public:
    Display(const char* Name, size_t Width, size_t Height);
    ~Display();

    void Update();
    inline bool IsRunning() { return !glfwWindowShouldClose(window); }

    glm::mat4 GetOrthoProjection();
    glm::mat4 GetPersProjection();

    operator GLFWwindow* () { return window; }

  private:
    GLFWwindow* window;
    GLuint vao;
  };
}