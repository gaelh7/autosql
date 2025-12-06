#pragma once

#include <optional>

#include "autosql/schema/datatype.hpp"
#include "autosql/schema/expression.hpp"

import asql.parse;
import asql.symbols;

namespace asql {

class Database;

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
