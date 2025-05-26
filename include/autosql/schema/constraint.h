#pragma once

#include <string>
#include <utility>

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
  Check(const parse::CheckParse& check)
    : Constraint{check.name_}, expr_{check.expr_} {}
};

class Column;
class Table;
class Database;

class ForeignKey : public Constraint {
  const Table* table_;
  std::vector<std::pair<const Column*, const Column*>> columns_;

public:
  ForeignKey(const Database& database, const Column& column,
             const parse::ForeignKeyParse<parse::ColumnParse>& fk);

  ForeignKey(const Database& database, const Table& table,
             const parse::ForeignKeyParse<parse::TableParse>& fk);
};

class Unique : public Constraint {
public:
  Unique(const parse::UniqueParse& unique) : Constraint{unique.name_} {}
};

}  // namespace asql
