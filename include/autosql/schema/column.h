#pragma once

#include <string>

#include "autosql/parse/column.h"
#include "autosql/schema/constraint.h"
#include "autosql/schema/expression.h"
#include "autosql/schema/datatype.h"

namespace asql {

class Database;

class Column {
  std::string name_;
  Datatype type_;
  std::optional<Expression> expr_;
  std::optional<Check> check_;
  std::optional<ForeignKey<Column>> reference_;
  std::optional<Unique> unique_;
  bool not_null_ = false;

public:
  Column(const parse::ColumnParse& col);

  void set_constraints(const Database& database, const parse::ColumnParse& col);
};

}  // namespace asql
