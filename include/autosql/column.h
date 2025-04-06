#pragma once

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
    name = parser.next_token().raw_;
    type = parser.next_token().raw_;

    parse_contraints(parser);
  }

  void parse_contraints(Tokenizer& parser) {
    while (!parser.done()) {
      Constraint curr;
      Token token = parser.next_token();
      if (token.type_ == TokenType::CONSTRAINT_T) {
        curr.name_ = parser.next_token().raw_;
        token      = parser.next_token();
      }
      switch (token.type_) {
        case TokenType::NOT_T:
          if (parser.next_token().type_ == TokenType::NULL_T) {
            not_null = true;
            continue;
          }
          // TODO error
          continue;
        case TokenType::UNIQUE_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_uq";
          }
          constraints[ConstraintType::UNIQUE] = curr;
          continue;
        case TokenType::DEFAULT_T: expr = Expression(parser); continue;
        case TokenType::AS_T:
          expr      = Expression(parser);
          generated = true;
          continue;
        case TokenType::REFERENCES_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_fk";
          }
          {
            std::get<1>(curr.val_).first = parser.next_token().raw_;
            parser.next_token();
            std::get<1>(curr.val_).second = parser.next_token().raw_;
            parser.next_token();
            constraints[ConstraintType::REFERENCE] = curr;
          }
          continue;
        case TokenType::CHECK_T:
          if (curr.name_.empty()) {
            curr.name_ += name;
            curr.name_ += "_ck";
          }
          curr.val_                          = Expression(parser);
          constraints[ConstraintType::CHECK] = curr;
          continue;
        case TokenType::CLOSE_PAR_T: parser.state = ParseState::TABLE; return;
        case TokenType::COMMA_T: return;
        default: break;  // TODO error
      }
      // TODO error
      break;
    }
  }
};
}  // namespace asql
