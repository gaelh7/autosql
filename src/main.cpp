#include <iostream>
#include <sstream>
#include <string>
#include <autosql/sqlparse.h>

std::string sql = "\t \nCREATE \t\n TABLE\t\r \nT1 ("
                  "  id INTEGER PRIMARY KEY,"
                  "  info VARCHAR(255)"
                  ");"
                  "\n"
                  "CREATE TABLE T2 ("
                  "  id INTEGER PRIMARY KEY,"
                  "  id2 INTEGER PRIMARY KEY,"
                  "  info VARCHAR(100)"
                  ")";

int main() {
  std::istringstream sstream(sql);
  // Opens the file for reading
  std::string line;
  while (std::getline(sstream, line, ';')) {
    asql::Table t = asql::parse_table(line);
    std::cout << "name: " << t.name << "\n";
    for (auto& column: t.columns) {
      std::cout << "  column name: " << column.name << "\n";
      std::cout << "  column type: " << column.type << "\n";
      std::cout << "  column annotation: " << column.anno << "\n";
    }
  }
  return 0;
}
