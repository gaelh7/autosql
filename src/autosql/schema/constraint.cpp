#include "autosql/schema/constraint.hpp"

#include <stdexcept>

#include "autosql/parse/constraint.hpp"
#include "autosql/schema/database.hpp"

namespace asql {

ForeignKey::ForeignKey(
    const Database& database, const Column& column,
    const parse::ForeignKeyParse<parse::ColumnParse>& fk)
  : Constraint{fk.name_} {
  table_ = database.table(fk.table_);
  if (!table_)
    throw std::runtime_error("Error: Undefined reference to table: " +
                             fk.table_);

  columns_.push_back({&column, table_->column(fk.column_)});
  if (!columns_[0].second)
    throw std::runtime_error("Error: Column '" + fk.column_ +
                             "' not found in table '" + fk.table_ + "'");
}

ForeignKey::ForeignKey(
    const Database& database, const Table& table,
    const parse::ForeignKeyParse<parse::TableParse>& fk)
  : Constraint{fk.name_} {
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

}  // namespace asql
