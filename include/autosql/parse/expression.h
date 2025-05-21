#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "autosql/parse/parser.h"
#include "autosql/parse/token.h"

namespace asql {
namespace parse {

class ExpressionParse {
  std::vector<Token> tokens_;
public:
  ExpressionParse() = default;

  ExpressionParse(Tokenizer& tokens) {
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

      tokens_.push_back(*tokens);
    }
    ++tokens;
  }

  std::string str() const noexcept {
    std::string out;
    for (const Token& token: tokens_) {
      out += token.str();
      out += ' ';
    }
    return out;
  }
};
}  // namespace parse
}  // namespace asql
