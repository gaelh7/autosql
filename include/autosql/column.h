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
    Tokenizer parser{sql};

    name = parser.next_token();
    type = parser.next_token();

    parse_contraints(parser);
  }

  inline void parse_contraints(Tokenizer parser) {
    while (!parser.done()) {
      Constraint curr;
      std::string_view token = parser.next_token();
      if (token == "CONSTRAINT") {
        curr.name = parser.next_token();
        token     = parser.next_token();
      }

      if (token == "NOT") {
        if (parser.next_token() == "NULL") {
          not_null = true;
          continue;
        }
        // TODO error
      }

      if (token == "UNIQUE") {
        unique = true;
        continue;
      }

      if (token == "DEFAULT") {
        expr = parser.next_token();
        continue;
      }

      if (token == "AS") {
        expr      = parser.next_token();
        generated = true;
        continue;
      }

      if (token == "REFERENCES") {
        reference.first  = parser.next_token();
        reference.second = parser.next_token();
        continue;
      }

      if (token == "CHECK") {
        curr.type = ConstraintType::CHECK;
        curr.expr = parser.next_token();
        constraints.push_back(curr);
        continue;
      }

      // TODO error
      break;
    }
  }
};
}  // namespace asql
