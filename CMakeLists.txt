cmake_minimum_required (VERSION 3.14)
project (Sane)

set(BuildDir ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/imgui_wrapper)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/entt)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE SRC_FILES
    sane/*.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")

add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/deps/glad/include
    ${PROJECT_SOURCE_DIR}/deps/glfw/include
    ${PROJECT_SOURCE_DIR}/deps/glm
    ${PROJECT_SOURCE_DIR}/deps/entt/single_include/
    ${PROJECT_SOURCE_DIR}/deps/imgui_wrapper/imgui
    ${PROJECT_SOURCE_DIR}/deps/imgui_wrapper/imgui/backends
    
)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME}
    glad
    glfw
    glm
    imgui
    OpenGL::GL
    EnTT::EnTT
)
