#pragma once

#include <string>
#include <unordered_map>

#include "autosql/constraint.h"
#include "autosql/sqlparse.h"

namespace asql {

class Column {
public:
  std::string name;
  std::string type;
  std::string expr;
  std::unordered_map<ConstraintType, Constraint> constraints;
  bool not_null  = false;
  bool generated = false;

  Column() = default;

  Column(std::string_view sql) {
    Tokenizer parser{sql};

    name = parser.next_token();
    type = parser.next_token();

    parse_contraints(parser);
  }

  void parse_contraints(Tokenizer parser) {
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
        if (curr.name.empty()) {
          curr.name += name;
          curr.name += "_uq";
        }
        constraints[ConstraintType::UNIQUE] = curr;
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
        if (curr.name.empty()) {
          curr.name += name;
          curr.name += "_fk";
        }
        curr.reference                         = parser.next_token();
        curr.expr                              = parser.next_token();
        constraints[ConstraintType::REFERENCE] = curr;
        continue;
      }

      if (token == "CHECK") {
        if (curr.name.empty()) {
          curr.name += name;
          curr.name += "_ck";
        }
        curr.expr                          = parser.next_token();
        constraints[ConstraintType::CHECK] = curr;
        continue;
      }

      // TODO error
      break;
    }
  }
};
}  // namespace asql
