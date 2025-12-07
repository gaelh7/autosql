module;

#include <optional>

export module asql.schema:column;

import :datatype;
import :expression;

import asql.parse;
import asql.symbols;

export namespace asql {

class Column {
  Identifier name_;
  Datatype type_;
  std::optional<Expression> expr_;
  bool not_null_;
  bool generated_;

public:
  Column() = default;

  Column(const parse::ColumnParse& col);
};

}  // namespace asql
