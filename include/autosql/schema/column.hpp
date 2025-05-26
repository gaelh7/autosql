#pragma once

#include <string>

#include "autosql/parse/column.hpp"
#include "autosql/schema/expression.hpp"
#include "autosql/schema/datatype.hpp"

namespace asql {

class Database;

class Column {
  std::string name_;
  Datatype type_;
  std::optional<Expression> expr_;
  bool not_null_;
  bool generated_;

public:
  Column() = default;

  Column(const parse::ColumnParse& col);
};

}  // namespace asql
