#pragma once

#include <unordered_map>

#include "autosql/parse/database.hpp"
#include "autosql/schema/table.hpp"

namespace asql {

class Database {
  std::unordered_map<std::string, Table> tables_;

public:
  Database(const parse::DatabaseParse& database);

  const Table* table(std::string_view name) const;
};

}  // namespace asql
