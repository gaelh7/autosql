#pragma once

#include <string>

#include "autosql/parse/column.h"
#include "autosql/schema/expression.h"
#include "autosql/schema/datatype.h"

namespace asql {

class Database;

class Column {
  std::string name_;
  Datatype type_;
  std::optional<Expression> expr_;
  bool not_null_ = false;

public:
  Column() = default;

  Column(const parse::ColumnParse& col);
};

}  // namespace asql
