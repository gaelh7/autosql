#pragma once

#include <stdexcept>
#include <string>

#include "autosql/parser.h"
#include "autosql/token.h"

namespace asql {

class Expression {
public:
  std::string raw_;

  Expression() = default;

  Expression(Tokenizer& tokens) {
    if (tokens->type != TokenType::OpenPar)
      throw std::runtime_error(
          "Error: Expressions must be within parentheses.");

    size_t par_count = 1;

    while (!tokens.done()) {
      switch ((++tokens)->type) {
        case TokenType::OpenPar: ++par_count; break;
        case TokenType::ClosePar: --par_count; break;
        default: break;
      }
      if (par_count == 0) break;
      raw_ += tokens->str();
      raw_ += ' ';
    }
    ++tokens;
  }
};
}  // namespace asql
