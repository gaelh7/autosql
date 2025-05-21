#pragma once

#include <string>

#include "autosql/parse/column.h"
#include "autosql/parse/constraint.h"
#include "autosql/schema/expression.h"

namespace asql {

class Constraint {
protected:
  std::string name_;

public:
  Constraint(std::string_view name) : name_{name} {}
};

class Check : public Constraint {
  Expression expr_;

public:
  Check(const parse::CheckParse check)
    : Constraint{check.name_}, expr_{check.expr_} {}
};

class Column;
class Table;
class Database;

template <typename T>
class ForeignKey;

template <>
class ForeignKey<Column> : public Constraint {
  const Table* table_;
  const Column* column_;

public:
  ForeignKey(const Database& database, const parse::ForeignKeyParse<parse::ColumnParse>& fk);
};

class Unique : public Constraint {
public:
  Unique(const parse::UniqueParse& unique) : Constraint{unique.name_} {}
};

}  // namespace asql
