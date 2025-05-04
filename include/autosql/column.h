#pragma once

#include <optional>
#include <stdexcept>
#include <string>

#include "autosql/constraint.h"
#include "autosql/expression.h"
#include "autosql/sqlparse.h"
#include "autosql/token.h"

namespace asql {

class Column {
  void parse_contraints(Tokenizer& tokens) {
    while (!tokens.done()) {
      std::string con_name;
      if (tokens->type == TokenType::CONSTRAINT_T) {
        con_name = (++tokens)->data;
        ++tokens;
      }
      switch (tokens->type) {
        case TokenType::NOT_T:
          if ((++tokens)->type == TokenType::NULL_T) {
            not_null = true;
            ++tokens;
            continue;
          }
          throw std::runtime_error(
              "Error: Expected symbol 'NULL' following 'NOT'");
        case TokenType::UNIQUE_T:
          if (con_name.empty()) con_name = name + "_uq";
          unique = Unique{con_name};
          ++tokens;
          continue;
        case TokenType::DEFAULT_T: expr = Expression(++tokens); continue;
        case TokenType::AS_T:
          expr      = Expression(++tokens);
          generated = true;
          continue;
        case TokenType::REFERENCES_T: {
          if (con_name.empty()) con_name = name + "_fk";
          std::string_view table = (++tokens)->data;
          ++tokens;
          std::string_view column = (++tokens)->data;
          ++tokens;
          reference = ForeignKey{con_name, table, column};
          ++tokens;
          continue;
        }
        case TokenType::CHECK_T:
          if (con_name.empty()) con_name = name + "_ck";
          check = Check{con_name, Expression(++tokens)};
          continue;
        case TokenType::CLOSE_PAR_T:
        case TokenType::COMMA_T: return;
        default: throw std::runtime_error("Error: Unknown constraint type");
      }
    }
  }

public:
  std::string name;
  std::string type;
  Expression expr;
  std::optional<Check> check;
  std::optional<ForeignKey> reference;
  std::optional<Unique> unique;
  bool not_null  = false;
  bool generated = false;

  Column() = default;

  Column(Tokenizer& tokens) {
    name = tokens->data;
    type = (++tokens)->data;

    parse_contraints(++tokens);
  }
};
}  // namespace asql
