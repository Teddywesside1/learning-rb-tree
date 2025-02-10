cmake_minimum_required(VERSION 3.8)
project(simple_tests)

add_compile_options(-std=c++17)
add_compile_options(-O3 -Wextra -Wdeprecated -fPIC)
set(CMAKE_CXX_STANDARD 17)



find_package(GTest REQUIRED)
find_package(glog REQUIRED)

set(source_file 
  src/rbtree_test.cpp
)

enable_testing()

include_directories(
  include
)

add_executable(${PROJECT_NAME} ${source_file})   

target_link_libraries(${PROJECT_NAME} PUBLIC 
  GTest::gtest_main
  glog::glog
  rbtree
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)