cmake_minimum_required(VERSION 3.3)
project(twitterutils)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})
include_directories(include)
set(SOURCE_FILES main.cpp jsoncpp.cpp)
add_executable(twitterutils ${SOURCE_FILES})

add_executable(libauthdemo simple_request.cpp jsoncpp.cpp)
target_link_libraries(libauthdemo /usr/local/lib/liboauthcpp.a ${CURL_LIBRARY} )

target_link_libraries(twitterutils ${CURL_LIBRARY} liboauthcpp.a)
#add_subdirectory(deps)