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
}  // namespace asql
