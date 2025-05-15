#include <string>
#include <unordered_map>

#include "autosql/parse/database.h"
#include "autosql/parse/table.h"
#include "autosql/schema/column.h"

namespace asql {

class Table {
  std::string name_;
  std::unordered_map<std::string, Column> columns;

public:
  Table(const parse::DatabaseParse& database, const parse::TableParse& table);
};

}  // namespace asql
