project(ucan_cpp_lib)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.58.0 COMPONENTS log log_setup thread date_time filesystem system  REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

    set (CMAKE_CXX_FLAGS "-g -std=c++14")
    add_executable(${PROJECT_NAME} ${SRC_LIST})

#    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()



