#include "autosql/schema/table.h"

#include "autosql/parse/table.h"

namespace asql {

Table::Table(const parse::TableParse& table) {
  for (const auto& [name, col] : table.columns) { columns_.emplace(name, col); }
}

void Table::set_constraints(const Database& database,
                            const parse::TableParse& table) {
  for (auto& [name, col] : columns_) {
    col.set_constraints(database, table.columns.at(name));
  }
}

const Column* Table::column(std::string_view name) const {
  auto it = columns_.find(std::string{name});
  if (it == columns_.end()) return nullptr;
  return &it->second;
}
}  // namespace asql
