cmake_minimum_required(VERSION 3.10)

project(autosql VERSION 1.0
                DESCRIPTION "Automatically produce sql schema diffs"
                LANGUAGES C CXX)

add_library(autosql STATIC
              src/autosql/parse/parser.cpp
              src/autosql/parse/table.cpp
              src/autosql/parse/column.cpp
              src/autosql/parse/constraint.cpp
              src/autosql/parse/datatype.cpp
              src/autosql/schema/database.cpp
              src/autosql/schema/table.cpp
              src/autosql/schema/column.cpp
)
target_include_directories(autosql PUBLIC include)
target_compile_features(autosql PUBLIC cxx_std_20)
target_compile_options(autosql PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,
                        /W4 /WX,
                        -Wall -Wextra -Wpedantic -Werror>)

add_executable(main src/main.cpp)
target_link_libraries(main autosql)
target_compile_options(main PRIVATE -Wall)
