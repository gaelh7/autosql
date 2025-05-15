#include <string>

#include "autosql/parse/column.h"
#include "autosql/parse/database.h"
#include "autosql/parse/table.h"

namespace asql {

class Column {
  std::string name_;

public:
  Column(const parse::DatabaseParse& database, const parse::TableParse& table,
         const parse::ColumnParse& col);
};

}  // namespace asql
