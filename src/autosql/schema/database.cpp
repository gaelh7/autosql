#include "autosql/schema/database.h"

namespace asql {

Database::Database(const parse::DatabaseParse& database) {
  for (const auto& [name, table] : database.tables_) {
    tables.try_emplace(name, table);
  }

  for (auto& [name, table] : tables) {
    table.set_constraints(database, database.tables_.at(name));
  }
}

}  // namespace asql
