#include "autosql/parse/constraint.hpp"

#include <span>
#include <stdexcept>

#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

ForeignKeyParse<ColumnParse>::ForeignKeyParse(std::string_view name, Lexer& tokens)
  : ConstraintParse{name} {
  table_ = tokens->str();
  if ((++tokens)->type != TokenType::OpenPar)
    throw std::runtime_error("Error: expected '('");
  column_ = (++tokens)->str();
  if ((++tokens)->type != TokenType::ClosePar)
    throw std::runtime_error("Error: expected ')'");
  ++tokens;
}

ForeignKeyParse<TableParse>::ForeignKeyParse(std::string_view name, Lexer& tokens)
  : ConstraintParse{name} {
  if (tokens->type != TokenType::OpenPar)
    throw std::runtime_error("Error: expected '('");
  while (tokens->type != TokenType::ClosePar) {
    columns_.emplace_back().first = (++tokens)->str();
    switch ((++tokens)->type) {
      case TokenType::Comma:
      case TokenType::ClosePar: break;
      default: throw std::runtime_error("Error: Expected ')' or ','");
    }
  }

  if ((++tokens)->type != TokenType::References)
    throw std::runtime_error("Error: Expected keyword 'REFERENCE'");

  table_ = (++tokens)->str();
  if ((++tokens)->type != TokenType::OpenPar)
    throw std::runtime_error("Error: expected '('");
  for (auto& [col, ref] : std::span{columns_}.first(columns_.size() - 1)) {
    ref = (++tokens)->str();
    if ((++tokens)->type != TokenType::Comma)
      throw std::runtime_error(
          "Error: Not enough references to match number of columns");
  }
  columns_.back().second = (++tokens)->str();
  if ((++tokens)->type != TokenType::ClosePar)
    throw std::runtime_error("Error: expected ')'");
  ++tokens;
}
}  // namespace parse
}  // namespace asql
