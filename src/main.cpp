#include <iostream>
#include <string>

#include "autosql/sqlparse.h"
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
                  ");";
std::string t2 = "CREATE TABLE test ("
                  "  id INTEGER NOT NULL UNIQUE DEFAULT (5),"
                  "  id2 INTEGER UNIQUE DEFAULT (1) CHECK (info + 1 = 2 * (info + 4)),"
                  "  id3 TEXT DEFAULT ('abc')"
                  ");";

int main() {
  asql::Tokenizer parser1{t1};
  parser1.state = asql::ParseState::TABLE;
  asql::Tokenizer parser2{t2};
  parser2.state = asql::ParseState::TABLE;
  asql::Table table1{parser1};
  asql::Table table2{parser2};
  asql::TableDiff diff{table1, table2};
  std::cout << diff.sql() << std::endl;
  return 0;
}
