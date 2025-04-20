#include <iostream>
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

std::string t1 = "CREATE TABLE test ("
                  "  id INTEGER DEFAULT (0),"
                  "  ignored INTEGER"
                  ");"
                  "CREATE TABLE test2 ("
                  "  id INTEGER DEFAULT (0),"
                  "  ignored INTEGER"
                  ");";
std::string t2 = "CREATE TABLE test ("
                  "  id INTEGER /* NOT NULL */ UNIQUE DEFAULT (5),"
                  "  id2 INTEGER UNIQUE DEFAULT (1) CHECK (info + 1 = 2 * (info + 4)),"
                  "  id3 TEXT DEFAULT ('abc')"
                  ");"
                  "CREATE TABLE test2 ("
                  "  id INTEGER /* NOT NULL */ UNIQUE DEFAULT (5),"
                  "  id2 INTEGER UNIQUE DEFAULT (1) CHECK (info + 1 = 2 * (info + 4)),"
                  "  id3 TEXT AS ('abc')"
                  ");";

int main() {
  asql::Database db1{t1};
  asql::Database db2{t2};
  asql::TableDiff diff{db1.tables_["test"], db2.tables_["test"]};
  asql::TableDiff diff2{db1.tables_["test2"], db2.tables_["test2"]};
  std::cout << diff.sql() << std::endl;
  std::cout << diff2.sql() << std::endl;
  return 0;
}
