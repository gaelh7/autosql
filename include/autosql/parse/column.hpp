#pragma once

#include <optional>

#include "autosql/parse/constraint.hpp"
#include "autosql/parse/datatype.hpp"
#include "autosql/parse/expression.hpp"
#include "autosql/parse/parser.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

class ColumnParse {
  void parse_constraints(Lexer& tokens);

public:
  Identifier name;
  DatatypeParse type;
  std::optional<ExpressionParse> expr;
  std::optional<CheckParse> check;
  std::optional<ForeignKeyParse<ColumnParse>> reference;
  std::optional<UniqueParse> unique;
  bool not_null  = false;
  bool generated = false;

  ColumnParse() = default;

  ColumnParse(Lexer& tokens);
};
}  // namespace asql::parse
