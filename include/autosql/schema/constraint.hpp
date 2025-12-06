#pragma once

#include <utility>
#include <vector>

// #include "autosql/schema/column.hpp"
#include "autosql/schema/expression.hpp"

import asql.parse;
import asql.symbols;

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
             const parse::ForeignKeyColumnParse& fk);

  ForeignKey(const Database& database, const Table& table,
             const parse::ForeignKeyTableParse& fk);
};

class Unique {
  Identifier name_;
  std::vector<const Column*> columns_;

public:
  Unique(const Column& col, const parse::UniqueColumnParse& unique)
    : name_{unique.name_}, columns_{&col} {}

  Unique(const Table& table, const parse::UniqueTableParse& unique);
};

}  // namespace asql
