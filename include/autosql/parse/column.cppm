module;

#include <optional>

export module asql.parse:column;

import :parser;
import :datatype;
import :expression;
import :constraint;
import asql.symbols;

export namespace asql::parse {

class ColumnParse {
  void parse_constraints(Lexer& tokens);

public:
  Identifier name;
  DatatypeParse type;
  std::optional<ExpressionParse> expr;
  std::optional<CheckParse> check;
  std::optional<ForeignKeyColumnParse> reference;
  std::optional<UniqueColumnParse> unique;
  bool not_null  = false;
  bool generated = false;

  ColumnParse() = default;

  ColumnParse(Lexer& tokens);
};
}  // namespace asql::parse
