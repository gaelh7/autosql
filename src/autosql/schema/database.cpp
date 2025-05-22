#include "autosql/schema/database.h"

namespace asql {

Database::Database(const parse::DatabaseParse& database) {
  for (const auto& [name, table] : database.tables_) {
    tables_.try_emplace(name, table);
  }

  for (auto& [name, table] : tables_) {
    table.set_constraints(*this, database.tables_.at(name));
  }
}

const Table* Database::table(std::string_view name) const {
  auto it = tables_.find(std::string{name});
  if (it == tables_.end()) return nullptr;
  return &it->second;
}

}  // namespace asql
