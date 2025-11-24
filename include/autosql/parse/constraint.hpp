#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "autosql/parse/expression.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

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

class ColumnParse;
class TableParse;

template <typename T>
class ForeignKeyParse;

template <>
class ForeignKeyParse<ColumnParse> {
public:
  Identifier name_;
  std::string table_;
  std::string column_;

  ForeignKeyParse(std::string_view name, Lexer& tokens);
};

template <>
class ForeignKeyParse<TableParse> {
public:
  Identifier name_;
  std::string table_;
  std::vector<std::pair<std::string, std::string>> columns_;

  ForeignKeyParse(std::string_view name, Lexer& tokens);
};

template <typename T>
class UniqueParse;

template <>
class UniqueParse<ColumnParse> {
public:
  Identifier name_;

  UniqueParse(std::string_view name) : name_{name} {}
};

template <>
class UniqueParse<TableParse> {
public:
  Identifier name_;
  std::vector<std::string> columns_;

  UniqueParse(std::string_view name, Lexer& tokens);
};
}  // namespace asql::parse
