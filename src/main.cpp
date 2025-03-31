#include <iostream>
#include <sstream>
#include <string>
#include <autosql/sqlparse.h>

std::string sql = "\t \nCREATE \t\n TABLE\t\r \nT1 ("
                  "  id INTEGER CONSTRAINT has_value NOT NULL,"
                  "  info VARCHAR(255) STORAGE DEFAULT COMPRESSION pglz COLLATE \"C\" CONSTRAINT with_name CHECK (conditions) DEFAULT ('abc')"
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
      std::cout << "  column storage: " << column.storage << "\n";
      std::cout << "  column compression: " << column.compression << "\n";
      std::cout << "  column collate: " << column.collate << "\n";
      std::cout << "  column constraints: \n";
      for (auto& constraint: column.constraints) {
        std::cout << "    name: " << constraint.name << "\n";
        std::cout << "    type: " << static_cast<int>(constraint.type) << "\n";
        std::cout << "    expression: " << constraint.expr << "\n";
      }
    }
  }
  return 0;
}
