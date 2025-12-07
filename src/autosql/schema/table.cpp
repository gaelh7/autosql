module;

#include <stdexcept>

module asql.schema;

import asql.parse;

namespace asql {

Table::Table(const parse::TableParse& table) : name_{table.name} {
  for (const auto& [name, col] : table.columns) columns_.try_emplace(name, col);

  primary_key.reserve(table.primary_key.size());
  for (std::string_view col : table.primary_key) {
    auto it = columns_.find(std::string{col});
    if (it == columns_.end())
      throw std::runtime_error("Error: Column '" + std::string{col} +
                               "' does not exist");
    primary_key.push_back(&it->second);
  }
  for (const auto& [name, col] : table.columns) {
    if (col.check) check_cons.emplace_back(*col.check);
    if (col.unique) unique_cons.emplace_back(col, *col.unique);
  }
  check_cons.insert(check_cons.end(), table.check_cons.begin(),
                    table.check_cons.end());
  for (const auto& unique : table.unique_cons) {
    unique_cons.emplace_back(*this, unique);
  }
}

void Table::add_reference_constraint(const ForeignKey& fk) {
  ref_cons.push_back(fk);
}

const Column* Table::column(std::string_view name) const {
  auto it = columns_.find(std::string{name});
  if (it == columns_.end()) return nullptr;
  return &it->second;
}
}  // namespace asql
