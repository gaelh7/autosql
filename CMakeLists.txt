cmake_minimum_required(VERSION 3.28)

project(autosql VERSION 1.0
                DESCRIPTION "Automatically produce sql schema diffs"
                LANGUAGES C CXX)

add_library(parse STATIC)
target_compile_features(parse PUBLIC cxx_std_23)
target_compile_options(parse PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,
                        /W4 /WX,
                        -Wall -Wextra -Wpedantic -Werror>)
target_sources(parse
               PRIVATE
                src/autosql/parse/parser.cpp
                src/autosql/parse/table.cpp
                src/autosql/parse/column.cpp
                src/autosql/parse/constraint.cpp
                src/autosql/parse/datatype.cpp
                src/autosql/parse/expression.cpp
               PUBLIC
               FILE_SET HEADERS TYPE HEADERS
               BASE_DIRS include
               FILES
                include/autosql/parse/parser.hpp
                include/autosql/parse/table.hpp
                include/autosql/parse/column.hpp
                include/autosql/parse/constraint.hpp
                include/autosql/parse/datatype.hpp
                include/autosql/parse/expression.hpp
)

add_library(schema STATIC)
target_compile_features(schema PUBLIC cxx_std_23)
target_link_libraries(schema PRIVATE parse)
# target_compile_options(schema PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,
#                         /W4 /WX,
#                         -Wall -Wextra -Wpedantic -Werror>)
target_sources(schema
               PRIVATE
                src/autosql/schema/database.cpp
                src/autosql/schema/table.cpp
                src/autosql/schema/column.cpp
                src/autosql/schema/constraint.cpp
                src/autosql/schema/datatype.cpp
               PUBLIC
               FILE_SET HEADERS TYPE HEADERS
               BASE_DIRS include
               FILES
                include/autosql/schema/database.hpp
                include/autosql/schema/table.hpp
                include/autosql/schema/column.hpp
                include/autosql/schema/constraint.hpp
                include/autosql/schema/datatype.hpp
)

add_executable(main src/main.cpp)
target_compile_features(main PUBLIC cxx_std_23)
target_link_libraries(main PRIVATE schema parse)
target_compile_options(main PRIVATE -Wall)
