cmake_minimum_required(VERSION 3.15)  # 推荐使用较新版本
project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 显式指定源文件（比 aux_source_directory 更可靠）
file(GLOB SRC_FILES 
    src/main.cpp
    src/calculate.cpp
    src/tool.cpp
)

# 添加可执行文件
add_executable(calculator ${SRC_FILES})

# 包含目录（现代写法）
target_include_directories(calculator PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# 关键修复：强制指定为控制台程序
if(WIN32)
    # 方法1：直接设置链接选项（推荐）
    target_link_options(calculator PRIVATE "-Wl,-subsystem,console")
    
    # 方法2：或者设置链接器标志
    # set_target_properties(calculator PROPERTIES
    #    LINK_FLAGS "-Wl,-subsystem,console"
    # )
endif()