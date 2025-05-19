#include <unordered_map>

#include "autosql/parse/database.h"
#include "autosql/schema/table.h"

namespace asql {

class Database {
  std::unordered_map<std::string, Table> tables;

public:
  Database(const parse::DatabaseParse& database);
};

}  // namespace asql
