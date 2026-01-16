module;

#include <optional>
#include <string_view>

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

  std::string_view name() const;

  const Datatype& type() const;

  const std::optional<Expression>& expr() const;

  bool nullable() const;

  bool is_generated() const;
};

}  // namespace asql
