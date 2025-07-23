cmake_minimum_required(VERSION 3.29.0)
project(calculator)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_RUNTIME_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LISTS)
add_library(calculator_lib 
SHARED
${SRC_LISTS}
)

target_include_directories(calculator_lib
PUBLIC
${PROJECT_SOURCE_DIR}/include
)

add_executable(calculator_main main.cpp)

target_include_directories(calculator_main
PUBLIC
${PROJECT_BINARY_DIR}/include
)

target_link_libraries(calculator_main
PUBLIC
calculator_lib
)

