cmake_minimum_required (VERSION 3.14)
project (SaneEngine)

set(BuildDir ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/dependencies/sane/src
    ${CMAKE_SOURCE_DIR}/dependencies/imgui_wrapper/imgui
    ${CMAKE_SOURCE_DIR}/dependencies/imgui_wrapper/imgui/backends       
)

target_link_libraries(${PROJECT_NAME}
    imgui
)
