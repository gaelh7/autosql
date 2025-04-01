#pragma once

#include <string>
#include <utility>
#include <vector>

#include "autosql/constraint.h"
#include "autosql/sqlparse.h"

namespace asql {
enum Type {};

enum Storage { PLAIN, EXTERNAL, EXTENDED, MAIN, DEFAULT };

class Column {
public:
  std::string name;
  std::string type;
  std::vector<Constraint> constraints;
  std::string expr;
  bool not_null  = false;
  bool unique    = false;
  bool generated = false;
  std::pair<std::string, std::string> reference;

  Column() = default;

  Column(std::string_view sql) {
    size_t token_pos     = sql.find_first_not_of(" \t\r\n");
    size_t token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
    name                 = sql.substr(token_pos, token_end_pos - token_pos);

    token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
    token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
    type          = sql.substr(token_pos, token_end_pos - token_pos);

    token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos);

    parse_contraints(sql.substr(token_pos));
  }

  inline void parse_contraints(std::string_view sql) {
    size_t token_pos = sql.find_first_not_of(" \t\r\n");
    size_t token_end_pos;
    while (token_pos != std::string_view::npos) {
      Constraint curr;
      if (sql.substr(token_pos, 10) == "CONSTRAINT") {
        token_pos     = sql.find_first_not_of(" \t\r\n", token_pos + 10);
        token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
        curr.name     = sql.substr(token_pos, token_end_pos - token_pos);
        token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
      }

      if (sql.substr(token_pos, 3) == "NOT") {
        token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 3);
        if (sql.substr(token_pos, 4) == "NULL") {
          not_null  = true;
          token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 4);
          continue;
        }
        // TODO error
      }

      if (sql.substr(token_pos, 6) == "UNIQUE") {
        unique    = true;
        token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 6);
        continue;
      }

      if (sql.substr(token_pos, 7) == "DEFAULT") {
        token_pos     = sql.find('(', token_pos + 7) + 1;
        token_end_pos = closing_par(sql, token_pos - 1);
        expr          = sql.substr(token_pos, token_end_pos - token_pos);
        token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
        continue;
      }

      if (sql.substr(token_pos, 2) == "AS") {
        token_pos     = sql.find('(', token_pos + 2) + 1;
        token_end_pos = closing_par(sql, token_pos - 1);
        expr          = sql.substr(token_pos, token_end_pos - token_pos);
        generated     = true;
        token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
        continue;
      }

      if (sql.substr(token_pos, 10) == "REFERENCES") {
        token_pos        = sql.find_first_not_of(" \t\r\n", token_pos + 10);
        token_end_pos    = sql.find_first_of(" \t\r\n(", token_pos);
        reference.first  = sql.substr(token_pos, token_end_pos - token_pos);
        token_pos        = sql.find('(', token_end_pos) + 1;
        token_end_pos    = closing_par(sql, token_pos - 1);
        reference.second = sql.substr(token_pos, token_end_pos - token_pos);
        token_pos        = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
        continue;
      }

      if (sql.substr(token_pos, 5) == "CHECK") {
        curr.type     = ConstraintType::CHECK;
        token_pos     = sql.find('(', token_pos + 5) + 1;
        token_end_pos = closing_par(sql, token_pos - 1);
        curr.expr     = sql.substr(token_pos, token_end_pos - token_pos);
        constraints.push_back(curr);
        token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
        continue;
      }

      // TODO error
      break;
    }
  }
};
}  // namespace asql
