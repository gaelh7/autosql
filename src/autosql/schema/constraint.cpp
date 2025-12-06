#include "autosql/schema/constraint.hpp"

#include <format>
#include <stdexcept>

#include "autosql/schema/database.hpp"


import asql.parse;

namespace asql {

ForeignKey::ForeignKey(const Database& database, const Column& column,
                       const parse::ForeignKeyColumnParse& fk)
  : name_{fk.name_} {
  table_ = database.table(fk.table_);
  if (!table_)
    throw std::runtime_error("Error: Undefined reference to table: " +
                             fk.table_);

  columns_.push_back({&column, table_->column(fk.column_)});
  if (!columns_[0].second)
    throw std::runtime_error("Error: Column '" + fk.column_ +
                             "' not found in table '" + fk.table_ + "'");
}

ForeignKey::ForeignKey(const Database& database, const Table& table,
                       const parse::ForeignKeyTableParse& fk)
  : name_{fk.name_} {
  table_ = database.table(fk.table_);
  if (!table_)
    throw std::runtime_error("Error: Undefined reference to table: " +
                             fk.table_);

  columns_.reserve(fk.columns_.size());
  for (const auto& [col, ref] : fk.columns_) {
    columns_.push_back({table.column(col), table_->column(ref)});
    if (!columns_.back().second)
      throw std::runtime_error("Error: Column '" + ref +
                               "' not found in table '" + fk.table_ + "'");
  }
}

Unique::Unique(const Table& table, const parse::UniqueTableParse& unique)
  : name_{unique.name_} {
  columns_.reserve(unique.columns_.size());
  for (std::string_view col : unique.columns_) {
    const Column* column = table.column(col);
    if (!column)
      throw std::runtime_error(
          std::format("Error: Column '{}' does not exist", col));
    columns_.push_back(column);
  }
}

}  // namespace asql
