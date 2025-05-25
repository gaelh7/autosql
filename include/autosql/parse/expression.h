#pragma once

#include <string>
#include <vector>

#include "autosql/parse/parser.h"
#include "autosql/parse/token.h"

namespace asql {
namespace parse {

class ExpressionParse {
  std::vector<Token> tokens_;

  void parse_func(Tokenizer& tokens);

  void pratt(Tokenizer& tokens, unsigned int precedence);

public:
  ExpressionParse() = default;

  ExpressionParse(Tokenizer& tokens);

  std::string str() const noexcept;
};
}  // namespace parse
}  // namespace asql
