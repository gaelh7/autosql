#pragma once

#include <stdexcept>
#include <string>

#include "autosql/sqlparse.h"
#include "autosql/token.h"

namespace asql {

class Expression {
public:
  std::string raw_;

  Expression() = default;

  Expression(Tokenizer& tokens) {
    if (tokens->type != TokenType::OPEN_PAR_T)
      throw std::runtime_error(
          "Error: Expressions must be within parentheses.");

    size_t par_count = 1;

    while (!tokens.done()) {
      switch ((++tokens)->type) {
        case TokenType::OPEN_PAR_T: ++par_count; break;
        case TokenType::CLOSE_PAR_T: --par_count; break;
        default: break;
      }
      if (par_count == 0) break;
      raw_ += tokens->data;
      raw_ += ' ';
    }
    ++tokens;
  }
};
}  // namespace asql
