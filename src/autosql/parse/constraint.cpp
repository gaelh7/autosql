module;

#include <span>
#include <stdexcept>

module asql.parse;

import asql.symbols;

namespace asql::parse {

ForeignKeyColumnParse::ForeignKeyColumnParse(std::string_view name,
                                             Lexer& tokens)
  : name_{name} {
  table_ = tokens->str();
  (++tokens).expect(TokenId::OpenPar);
  column_ = (++tokens)->str();
  (++tokens).expect(TokenId::ClosePar);
  ++tokens;
}

ForeignKeyTableParse::ForeignKeyTableParse(std::string_view name, Lexer& tokens)
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

UniqueTableParse::UniqueTableParse(std::string_view name, Lexer& tokens)
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
