# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.12)

# 项目信息
# project (cpp2048 LANGUAGE CXX) # cmake构建不成功，TODO
project(cpp2048)

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(main ${DIR_SRCS})

find_package(Curses  REQUIRED) # 这里要用大写C才能搜索到
target_link_libraries(main PUBLIC ncurses) # 这里引入curses 或者 ncurses都可以

# 为什么有的库可以直接include有的需要在CMakelists里写出。
# 标准库可以直接include，三方库需要额外写明.虽然ncurses 安装在系统里，但是编译器不会帮你自动链接，必须你指明 

# CMake如何做到帮你链接第三方库的？