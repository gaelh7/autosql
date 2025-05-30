#pragma once

#include <functional>
#include <unordered_map>

#include "autosql/parse/database.hpp"
#include "autosql/schema/table.hpp"
#include "autosql/symbols.hpp"

namespace asql {

class Database {
  std::unordered_map<Identifier, Table, IdHash, std::equal_to<>> tables_;

public:
  Database(const parse::DatabaseParse& database);

  const Table* table(std::string_view name) const;
};

}  // namespace asql
