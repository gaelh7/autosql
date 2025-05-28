#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "autosql/parse/expression.hpp"
#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

class ConstraintParse {
public:
  std::string name_;

  ConstraintParse() = default;

  ConstraintParse(std::string_view name) : name_{name} {}
};

class CheckParse : public ConstraintParse {
public:
  ExpressionParse expr_;

  CheckParse(std::string_view name, Lexer& tokens) : ConstraintParse{name} {
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
class ForeignKeyParse<ColumnParse> : public ConstraintParse {
public:
  std::string table_;
  std::string column_;

  ForeignKeyParse(std::string_view name, Lexer& tokens);
};

template <>
class ForeignKeyParse<TableParse> : public ConstraintParse {
public:
  std::string table_;
  std::vector<std::pair<std::string, std::string>> columns_;

  ForeignKeyParse(std::string_view name, Lexer& tokens);
};

class UniqueParse : public ConstraintParse {
  using ConstraintParse::ConstraintParse;
};
}  // namespace parse
}  // namespace asql
