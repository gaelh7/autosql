#pragma once

#include <string>
#include <vector>

#include "autosql/parse/parser.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

class ExpressionParse {
  std::vector<Operator> tokens_;

  unsigned int parse_func(Lexer& tokens);

  void pratt(Lexer& tokens, unsigned int precedence);

public:
  ExpressionParse() = default;

  ExpressionParse(Lexer& tokens);

  std::string str() const noexcept;
};
}  // namespace asql::parse
