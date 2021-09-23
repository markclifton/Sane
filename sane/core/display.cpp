#include "sane/core/display.hpp"

#include "sane/debugging/logging.hpp"

namespace
{
  void key_forwarder(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_R && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaZ = -.01f;
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
      deltaZ = 0;

    if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaZ = .01f;
    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
      deltaZ = 0;

    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaY = -.01f;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
      deltaY = 0;

    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaY = .01f;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
      deltaY = 0;

    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaX = -.01f;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
      deltaX = 0;

    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
      deltaX = .01f;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
      deltaX = 0;

    if (key == GLFW_KEY_1 && (action == GLFW_PRESS))
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (key == GLFW_KEY_2 && (action == GLFW_PRESS))
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  void cursor_forwarder(GLFWwindow* window, double xpos, double ypos)
  {
    glfwGetWindowUserPointer(window);
  }

  void mouse_forwarder(GLFWwindow* window, int button, int action, int mods)
  {
    glfwGetWindowUserPointer(window);
  }

  static void error_callback(int error, const char* description) {
    SANE_WARN(description);
  }
}

namespace Sane
{
  Display::Display(const char* Name, size_t Width, size_t Height)
  {
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

    glfwSetErrorCallback(error_callback);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_forwarder);
    glfwSetCursorPosCallback(window, cursor_forwarder);
    glfwSetMouseButtonCallback(window, mouse_forwarder);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SANE_INFO("Successfully created display");
  }

  Display::~Display() {
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();
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