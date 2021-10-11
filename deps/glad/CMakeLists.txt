cmake_minimum_required (VERSION 3.14)
project (glad)

file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/**.c
)

add_library(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
