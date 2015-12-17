cmake_minimum_required(VERSION 3.3)
project(twitterutils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_subdirectory(cpr)
include_directories(include)

include_directories(${CPR_INCLUDE_DIRS})
add_executable(twitterutils simple_request.cpp jsoncpp.cpp)
target_link_libraries(twitterutils /usr/local/lib/liboauthcpp.a ${CPR_LIBRARIES} )
