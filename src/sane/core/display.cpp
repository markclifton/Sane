#include "sane/core/display.hpp"

#include "sane/debugging/logging.hpp"

namespace Sane {
  Display::Display(const char* Name, size_t Width, size_t Height) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
      exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(static_cast<int>(Width), static_cast<int>(Height), Name, NULL, NULL);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnable(GL_DEPTH_TEST);

    SANE_INFO("Created OGL Window and Context");
  }

  Display::~Display() {
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
  }

  void Display::Update() {
    glfwSwapBuffers(window);
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  glm::mat4 Display::GetOrthoProjection() {
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;

    return glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
  }

  glm::mat4 Display::GetPersProjection() {
    float ratio;
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;
    return glm::perspective(45.0f, ratio, 1.0f, 200.0f);
  }
}