module;

#include <string>

module asql.schema;

import :table;

import asql.parse;

namespace asql {

Database::Database(const parse::DatabaseParse& database) {
  for (const auto& [name, table] : database.tables_) {
    tables_.try_emplace(name, table);
  }

  for (auto& [name, table] : tables_) {
    auto table_parse = database.tables_.at(name);
    for (const auto& [name, col] : table_parse.columns) {
      if (col.reference)
        table.add_reference_constraint(
            ForeignKey(*this->table(col.reference->table_), *table.column(name), *col.reference));
    }
    for (const auto& con : table_parse.ref_cons) {
      table.add_reference_constraint(ForeignKey(*this->table(con.table_), table, con));
    }
  }
}

const Table* Database::table(std::string_view name) const {
  auto it = tables_.find(std::string{name});
  if (it == tables_.end()) return nullptr;
  return &it->second;
}

}  // namespace asql
