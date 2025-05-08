#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

#include "autosql/database.h"
#include "autosql/table.h"

std::string sql = "\t \nCREATE \t\n TABLE\t\r \nT1 ("
                  "  id INTEGER NOT NULL DEFAULT (0), -- This is a comment\n"
                  "  info VARCHAR CONSTRAINT with_name CHECK (info + 1 = 2 * (info + 4)) "
                  "DEFAULT ('abc')"
                  ");"
                  "\n"
                  "CREATE TABLE T2 ("
                  "  id INTEGER UNIQUE NOT NULL,"
                  "  id2 INTEGER AS (id * 2),"
                  "  info VARCHAR REFERENCES T1(info)"
                  ") PRIMARY KEY (id, id2);";

std::string t1 = "CREATE TABLE test (\n"
                  "  id INTEGER DEFAULT (.5),\n"
                  "  ignored INTEGER\n"
                  ");\n"
                  "CREATE TABLE test2 (\n"
                  "  id INTEGER DEFAULT (0),\n"
                  "  ignored INTEGER\n"
                  ");";
std::string t2 = "CREATE TABLE test ("
                  "  id INTEGER /* NOT NULL */ UNIQUE DEFAULT (5),"
                  "  id2 INTEGER UNIQUE DEFAULT (1) CHECK (info + 1 = 2 * (info + 4)),"
                  "  id3 TEXT DEFAULT ('abc')"
                  ");"
                  "CREATE TABLE test2 ("
                  "  id INTEGER /* NOT NULL */ UNIQUE DEFAULT (5),"
                  "  id2 INTEGER UNIQUE DEFAULT (1) CHECK (info + 1 = 2 * (info + 4)),"
                  "  FOREIGN KEY (id, id2) REFERENCES test(id2, id),"
                  "  id3 TEXT AS ('abc') REFERENCES test(id2),"
                  "  CHECK (id + id2 < 10)"
                  ");";

int main() {
  std::filesystem::path f1 = std::filesystem::temp_directory_path().append("t1.sql");
  std::filesystem::path f2 = std::filesystem::temp_directory_path().append("t2.sql");
  std::ofstream{f1} << t1 << std::flush;
  std::ofstream{f2} << t2 << std::flush;
  asql::Database db1{f1};
  asql::Database db2{f2};
  asql::TableDiff diff{db1.tables_["test"], db2.tables_["test"]};
  asql::TableDiff diff2{db1.tables_["test2"], db2.tables_["test2"]};
  std::cout << diff.sql() << std::endl;
  std::cout << diff2.sql() << std::endl;
  return 0;
}
