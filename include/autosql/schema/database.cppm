module;

#include <functional>
#include <unordered_map>
#include <string_view>

export module asql.schema:database;

import :table;

import asql.parse;
import asql.symbols;

export namespace asql {

class Database {
  std::unordered_map<Identifier, Table, IdHash, std::equal_to<>> tables_;

public:
  Database(const parse::DatabaseParse& database);

  const Table* table(std::string_view name) const;
};

}  // namespace asql
