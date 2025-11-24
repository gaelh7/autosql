#pragma once

#include <utility>

#include "autosql/parse/column.hpp"
#include "autosql/parse/constraint.hpp"
#include "autosql/schema/expression.hpp"
#include "autosql/symbols.hpp"

namespace asql {

class Check {
  Identifier name_;
  Expression expr_;

public:
  Check(const parse::CheckParse& check)
    : name_{check.name_}, expr_{check.expr_} {}
};

class Column;
class Table;
class Database;

class ForeignKey {
  Identifier name_;
  const Table* table_;
  std::vector<std::pair<const Column*, const Column*>> columns_;

public:
  ForeignKey(const Database& database, const Column& column,
             const parse::ForeignKeyParse<parse::ColumnParse>& fk);

  ForeignKey(const Database& database, const Table& table,
             const parse::ForeignKeyParse<parse::TableParse>& fk);
};

class Unique {
  Identifier name_;
  std::vector<const Column*> columns_;

public:
  Unique(const Column& col,
         const parse::UniqueParse<parse::ColumnParse>& unique)
    : name_{unique.name_}, columns_{&col} {}

  Unique(const Table& table, const parse::UniqueParse<parse::TableParse>& unique);
};

}  // namespace asql
