cmake_minimum_required(VERSION 3.22)
project(cpp20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/reactor ${CMAKE_CURRENT_SOURCE_DIR}/asyncio)

file(GLOB SINGLE_FILE_EXE "${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

foreach(file ${SINGLE_FILE_EXE})
    get_filename_component(filename ${file} NAME_WE)
    if (NOT ${filename} IN_LIST NOT_COMPILE)
        add_executable(${filename} ${file})
        message("add ${filename}")
    endif()
endforeach()

set(REACTOR_FILE reactor/reactor.cpp reactor/acceptor.cpp reactor/common.cpp reactor/epoll_selector.cpp reactor/handler.cpp reactor/tcp_conn.cpp)

add_executable(my_reactor_test reactor/reactor_test.cpp ${REACTOR_FILE})
message("add my_reactor_test")

# add_executable(simple_co_server test/simple_co_server.cpp)
# message("add simple_co_server")
