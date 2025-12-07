cmake_minimum_required(VERSION 3.28)

project(autosql VERSION 1.0
                DESCRIPTION "Automatically produce sql schema diffs"
                LANGUAGES C CXX)

add_library(symbols STATIC)
target_compile_features(symbols PUBLIC cxx_std_23)
target_compile_options(symbols PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,
                        /W4 /WX,
                        -Wall -Wextra -Wpedantic -Werror>)
target_sources(symbols
               PUBLIC FILE_SET CXX_MODULES TYPE CXX_MODULES
               BASE_DIRS include
                FILES
                  include/autosql/symbols.cppm
)

add_library(parse STATIC)
target_link_libraries(parse PRIVATE symbols)
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
               FILE_SET CXX_MODULES TYPE CXX_MODULES
               BASE_DIRS include
               FILES
                include/autosql/parse/parse.cppm
                include/autosql/parse/parser.cppm
                include/autosql/parse/datatype.cppm
                include/autosql/parse/expression.cppm
                include/autosql/parse/constraint.cppm
                include/autosql/parse/column.cppm
                include/autosql/parse/table.cppm
                include/autosql/parse/database.cppm
)

add_library(schema STATIC)
target_link_libraries(schema PRIVATE symbols parse)
target_compile_features(schema PUBLIC cxx_std_23)
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
               FILE_SET CXX_MODULES TYPE CXX_MODULES
               BASE_DIRS include
               FILES
                include/autosql/schema/column.cppm
                include/autosql/schema/constraint.cppm
                include/autosql/schema/database.cppm
                include/autosql/schema/datatype.cppm
                include/autosql/schema/expression.cppm
                include/autosql/schema/schema.cppm
                include/autosql/schema/table.cppm
)

add_executable(main src/main.cpp)
target_compile_features(main PUBLIC cxx_std_23)
target_link_libraries(main PRIVATE schema parse)
target_compile_options(main PRIVATE -Wall)
