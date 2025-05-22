#include "autosql/schema/table.h"

#include "autosql/parse/table.h"

namespace asql {

Table::Table(const parse::TableParse& table) {
  for (const auto& [name, col] : table.columns) { columns_.emplace(name, col); }
}

void Table::set_constraints(const Database& database,
                            const parse::TableParse& table) {
  for (const auto& [name, col] : table.columns) {
    if (col.check)
      check_cons.emplace_back(*col.check);
    if (col.reference)
      ref_cons.emplace_back(database, columns_[name], *col.reference);
    if (col.unique)
      unique_cons.emplace_back(*col.unique);
  }
  check_cons.insert(check_cons.end(), table.check_cons.begin(), table.check_cons.end());
  unique_cons.insert(unique_cons.end(), table.unique_cons.begin(), table.unique_cons.end());

  for (const auto& con : table.ref_cons) {
    ref_cons.emplace_back(database, *this, con);
  }

}

const Column* Table::column(std::string_view name) const {
  auto it = columns_.find(std::string{name});
  if (it == columns_.end()) return nullptr;
  return &it->second;
}
}  // namespace asql
