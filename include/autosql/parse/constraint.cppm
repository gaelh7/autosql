module;

#include <string>
#include <string_view>
#include <utility>
#include <vector>

export module asql.parse:constraint;

import :parser;
import :expression;

import asql.symbols;

export namespace asql::parse {

class CheckParse {
public:
  Identifier name_;
  ExpressionParse expr_;

  CheckParse(std::string_view name, Lexer& tokens) : name_{name} {
    tokens.expect(TokenId::OpenPar);
    expr_ = ExpressionParse{++tokens};
    tokens.expect(TokenId::ClosePar);
    ++tokens;
  }
};

class ForeignKeyColumnParse {
public:
  Identifier name_;
  std::string table_;
  std::string column_;

  ForeignKeyColumnParse(std::string_view name, Lexer& tokens);
};

class ForeignKeyTableParse {
public:
  Identifier name_;
  std::string table_;
  std::vector<std::pair<std::string, std::string>> columns_;

  ForeignKeyTableParse(std::string_view name, Lexer& tokens);
};

class UniqueColumnParse {
public:
  Identifier name_;

  UniqueColumnParse(std::string_view name) : name_{name} {}
};

class UniqueTableParse {
public:
  Identifier name_;
  std::vector<std::string> columns_;

  UniqueTableParse(std::string_view name, Lexer& tokens);
};
}  // namespace asql::parse
