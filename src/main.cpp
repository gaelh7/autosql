#include <iostream>
#include <sstream>
#include <string>

#include "autosql/table.h"
#include "autosql/sqlparse.h"

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

int main() {
  std::istringstream sstream(sql);
  asql::Tokenizer parser{sql};

  for (auto token = parser.next_token(); token != ""; token = parser.next_token()) {
    std::cout << token << '\n';
  }
  // Opens the file for reading
  std::string line;
  while (std::getline(sstream, line, ';')) {
    asql::Table t(line);
    std::cout << "name: " << t.name << "\n";
    std::cout << "  primary key: ";
    for (auto& key: t.primary_key) {
      std::cout << key << ", ";
    }
    std::cout << '\n';
    for (auto& column : t.columns) {
      std::cout << "  column name: " << column.name << "\n";
      std::cout << "    type: " << column.type << "\n";
      std::cout << "    unique: " << column.unique << "\n";
      std::cout << "    not null: " << column.not_null << "\n";
      std::cout << "    generated: " << column.generated << "\n";
      std::cout << "    default: " << column.expr << "\n";
      std::cout << "    references: " << column.reference.first << ", "
                << column.reference.second << "\n";
      std::cout << "    constraints: \n";
      for (auto& constraint : column.constraints) {
        std::cout << "      name: " << constraint.name << "\n";
        std::cout << "      type: " << static_cast<int>(constraint.type)
                  << "\n";
        std::cout << "      expression: " << constraint.expr << "\n";
      }
    }
    std::cout << std::flush;
  }
  return 0;
}
