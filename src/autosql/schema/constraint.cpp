module;

#include <format>
#include <stdexcept>

module asql.schema;

import asql.parse;

namespace asql {

ForeignKey::ForeignKey(const Table& parent, const Column& column,
                       const parse::ForeignKeyColumnParse& fk)
  : table_{&parent}, name_{fk.name_} {
  columns_.push_back({&column, table_->column(fk.column_)});
  if (!columns_[0].second)
    throw std::runtime_error("Error: Column '" + fk.column_ +
                             "' not found in table '" + fk.table_ + "'");
}

ForeignKey::ForeignKey(const Table& parent, const Table& table,
                       const parse::ForeignKeyTableParse& fk)
  : table_{&parent}, name_{fk.name_} {
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
