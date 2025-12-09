cmake_minimum_required(VERSION 3.10)
project(HWID_Utility)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(hwid_modifier hwid_modifier.cpp)

# Windows特定设置
if(WIN32)
    # 链接必要的Windows库
    target_link_libraries(hwid_modifier 
        iphlpapi
        advapi32
    )
    
    # 设置为控制台应用
    set_target_properties(hwid_modifier PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# 编译选项
if(MSVC)
    target_compile_options(hwid_modifier PRIVATE /W4)
else()
    target_compile_options(hwid_modifier PRIVATE -Wall -Wextra)
endif()
