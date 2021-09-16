cmake_minimum_required (VERSION 3.14)
project (SaneEngine)

set(BuildDir ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")

add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/dependencies/sane-engine/src
    ${CMAKE_SOURCE_DIR}/dependencies/glad/include
    ${CMAKE_SOURCE_DIR}/dependencies/glfw/include
    ${CMAKE_SOURCE_DIR}/dependencies/glm
    ${CMAKE_SOURCE_DIR}/dependencies/imgui_wrapper/imgui
    ${CMAKE_SOURCE_DIR}/dependencies/imgui_wrapper/imgui/backends 
)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME}
    glad
    glfw
    glm
    imgui
    OpenGL::GL
)
