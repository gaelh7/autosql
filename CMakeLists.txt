cmake_minimum_required(VERSION 3.10)

project(autosql VERSION 1.0
              DESCRIPTION "Automatically produce sql schema diffs"
              LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(autosql INTERFACE
    include/autosql/sqlparse.h
    include/autosql/table.h)
target_include_directories(autosql INTERFACE include)
target_compile_features(autosql INTERFACE cxx_std_20)

add_executable(main src/main.cpp)
target_link_libraries(main autosql)
target_compile_options(main PRIVATE)
