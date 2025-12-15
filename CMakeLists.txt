#指定最低版本
cmake_minimum_required(VERSION 3.0)
#自定义项目名称
project(bookingsystem)
# 设置C++标准（根据你的代码需求选，比如C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)  # 编译器没有C++17的话，会降级用更低的标准
#生成可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory(SRC/client)
add_subdirectory(SRC/server)
