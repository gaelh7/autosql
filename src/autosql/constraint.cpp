#include "autosql/constraint.h"

#include <span>
#include <stdexcept>

#include "autosql/token.h"

namespace asql {

ForeignKey<Column>::ForeignKey(std::string_view name, Tokenizer& tokens)
  : Constraint{name} {
  table_ = tokens->str();
  if ((++tokens)->type != TokenType::OPEN_PAR_T)
    throw std::runtime_error("Error: expected '('");
  column_ = (++tokens)->str();
  if ((++tokens)->type != TokenType::CLOSE_PAR_T)
    throw std::runtime_error("Error: expected ')'");
  ++tokens;
}

ForeignKey<Table>::ForeignKey(std::string_view name, Tokenizer& tokens)
  : Constraint{name} {
  if (tokens->type != TokenType::OPEN_PAR_T)
    throw std::runtime_error("Error: expected '('");
  while (tokens->type != TokenType::CLOSE_PAR_T) {
    columns_.emplace_back().first = (++tokens)->str();
    switch ((++tokens)->type) {
      case TokenType::COMMA_T:
      case TokenType::CLOSE_PAR_T: break;
      default: throw std::runtime_error("Error: Expected ')' or ','");
    }
  }

  if ((++tokens)->type != TokenType::REFERENCES_T)
    throw std::runtime_error("Error: Expected keyword 'REFERENCE'");

  table_ = (++tokens)->str();
  if ((++tokens)->type != TokenType::OPEN_PAR_T)
    throw std::runtime_error("Error: expected '('");
  for (auto& [col, ref] : std::span{columns_}.first(columns_.size() - 1)) {
    ref = (++tokens)->str();
    if ((++tokens)->type != TokenType::COMMA_T)
      throw std::runtime_error(
          "Error: Not enough references to match number of columns");
  }
  columns_.back().second = (++tokens)->str();
  if ((++tokens)->type != TokenType::CLOSE_PAR_T)
    throw std::runtime_error("Error: expected ')'");
  ++tokens;
}
}  // namespace asql
