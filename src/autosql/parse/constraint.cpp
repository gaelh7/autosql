#include "autosql/parse/constraint.hpp"

#include <span>
#include <stdexcept>

#include "autosql/symbols.hpp"

namespace asql::parse {

ForeignKeyParse<ColumnParse>::ForeignKeyParse(std::string_view name,
                                              Lexer& tokens)
  : name_{name} {
  table_ = tokens->str();
  (++tokens).expect(TokenId::OpenPar);
  column_ = (++tokens)->str();
  (++tokens).expect(TokenId::ClosePar);
  ++tokens;
}

ForeignKeyParse<TableParse>::ForeignKeyParse(std::string_view name,
                                             Lexer& tokens)
  : name_{name} {
  tokens.expect(TokenId::OpenPar);
  while (tokens->type != TokenId::ClosePar) {
    columns_.emplace_back().first = (++tokens)->str();
    switch ((++tokens)->type) {
      case TokenId::Comma:
      case TokenId::ClosePar: break;
      default: throw std::runtime_error("Error: Expected ')' or ','");
    }
  }

  (++tokens).expect(TokenId::References);

  table_ = (++tokens)->str();
  (++tokens).expect(TokenId::OpenPar);
  for (auto& [col, ref] : std::span{columns_}.first(columns_.size() - 1)) {
    ref = (++tokens)->str();
    (++tokens).expect(TokenId::Comma);
  }
  columns_.back().second = (++tokens)->str();
  (++tokens).expect(TokenId::ClosePar);
  ++tokens;
}

UniqueParse<TableParse>::UniqueParse(std::string_view name, Lexer& tokens)
  : name_{name} {
  tokens.expect(TokenId::OpenPar);
  while (tokens->type != TokenId::ClosePar) {
    columns_.emplace_back((++tokens)->str());
    switch ((++tokens)->type) {
      case TokenId::Comma:
      case TokenId::ClosePar: break;
      default: throw std::runtime_error("Error: Expected ')' or ','");
    }
  }
  ++tokens;
}
}  // namespace asql::parse
