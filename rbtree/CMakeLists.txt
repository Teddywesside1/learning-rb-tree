cmake_minimum_required(VERSION 3.8)
project(rbtree)

add_compile_options(-std=c++17)
add_compile_options(-O3)
set(CMAKE_CXX_STANDARD 17)

include_directories(
  include
)

set(source_file 
  src/rbtree.c
)

add_library(${PROJECT_NAME} SHARED ${source_file})   


target_link_libraries(${PROJECT_NAME} PUBLIC

) 

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)