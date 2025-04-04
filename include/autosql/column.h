#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

#include "autosql/constraint.h"
#include "autosql/sqlparse.h"

namespace asql {
enum Type {};

enum Storage { PLAIN, EXTERNAL, EXTENDED, MAIN, DEFAULT };

class Column {
public:
  std::string name;
  std::string type;
  std::string expr;
  std::pair<std::string, std::string> reference;
  Constraint constraint;
  bool not_null  = false;
  bool unique    = false;
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
        curr.expr  = parser.next_token();
        constraint = curr;
        continue;
      }

      // TODO error
      break;
    }
  }
};

class ColumnDiff {
public:
  std::string name;
  std::optional<std::string> type;
  std::optional<std::string> expr;
  std::optional<std::pair<std::string, std::string>> reference;
  std::optional<ConstraintDiff> constraint;
  std::optional<bool> not_null;
  std::optional<bool> unique;
  std::optional<bool> generated;

  ColumnDiff(Column lhs, Column rhs) : name{rhs.name} {
    if (lhs.name != rhs.name)
      throw std::invalid_argument("Rename operations not supported");
    if (lhs.type != rhs.type) type = rhs.type;
    if (lhs.expr != rhs.expr) expr = rhs.expr;
    if (lhs.reference != rhs.reference) reference = rhs.reference;
    if (lhs.constraint.expr != rhs.constraint.expr)
      constraint = ConstraintDiff(lhs.constraint, rhs.constraint);
    if (lhs.not_null != rhs.not_null) not_null = rhs.not_null;
    if (lhs.unique != rhs.unique) unique = rhs.unique;
    if (lhs.generated != rhs.generated) generated = rhs.generated;
  }
};
}  // namespace asql
