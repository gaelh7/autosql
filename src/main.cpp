#include <iostream>
#include <sstream>
#include <string>
#include <autosql/sqlparse.h>

std::string sql = "\t \nCREATE \t\n TABLE\t\r \nT1 ("
                  "  id INTEGER NOT NULL DEFAULT (0),"
                  "  info VARCHAR(255) CONSTRAINT with_name CHECK (conditions) DEFAULT ('abc')"
                  ");"
                  "\n"
                  "CREATE TABLE T2 ("
                  "  id INTEGER UNIQUE NOT NULL,"
                  "  id2 INTEGER PRIMARY KEY,"
                  "  info VARCHAR(100) REFERENCES T1(info)"
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
      std::cout << "    type: " << column.type << "\n";
      std::cout << "    unique: " << column.unique << "\n";
      std::cout << "    not null: " << column.not_null << "\n";
      std::cout << "    default: " << column.expr << "\n";
      std::cout << "    references: " << column.reference.first << ", " << column.reference.second << "\n";
      std::cout << "    constraints: \n";
      for (auto& constraint: column.constraints) {
        std::cout << "      name: " << constraint.name << "\n";
        std::cout << "      type: " << static_cast<int>(constraint.type) << "\n";
        std::cout << "      expression: " << constraint.expr << "\n";
      }
    }
  }
  return 0;
}
