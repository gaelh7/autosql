#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "autosql/constraint.h"
#include "autosql/expression.h"
#include "autosql/sqlparse.h"
#include "autosql/token.h"

namespace asql {

class Column {
public:
  std::string name;
  std::string type;
  Expression expr;
  std::unordered_map<ConstraintType, Constraint> constraints;
  bool not_null  = false;
  bool generated = false;

  Column() = default;

  Column(Tokenizer& parser) {
    name = parser->data;
    type = (++parser)->data;

    parse_contraints(++parser);
  }

  void parse_contraints(Tokenizer& parser) {
    while (!parser.done()) {
      Constraint curr;
      if (parser->type == TokenType::CONSTRAINT_T) {
        curr.name_ = (++parser)->data;
        ++parser;
      }
      switch (parser->type) {
        case TokenType::NOT_T:
          if ((++parser)->type == TokenType::NULL_T) {
            not_null = true;
            ++parser;
            continue;
          }
          throw std::runtime_error(
              "Error: Expected symbol 'NULL' following 'NOT'");
        case TokenType::UNIQUE_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_uq";
          }
          constraints[ConstraintType::UNIQUE] = curr;
          ++parser;
          continue;
        case TokenType::DEFAULT_T: expr = Expression(++parser); continue;
        case TokenType::AS_T:
          expr      = Expression(++parser);
          generated = true;
          continue;
        case TokenType::REFERENCES_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_fk";
          }
          std::get<1>(curr.val_).first = (++parser)->data;
          ++parser;
          std::get<1>(curr.val_).second = (++parser)->data;
          ++parser;
          constraints[ConstraintType::REFERENCE] = curr;
          ++parser;
          continue;
        case TokenType::CHECK_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_ck";
          }
          curr.val_                          = Expression(++parser);
          constraints[ConstraintType::CHECK] = curr;
          continue;
        case TokenType::CLOSE_PAR_T:
        case TokenType::COMMA_T: return;
        default: break;  // TODO error
      }
      // TODO error
      break;
    }
  }
};
}  // namespace asql
