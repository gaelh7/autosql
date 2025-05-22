#include "autosql/schema/column.h"

#include "autosql/parse/column.h"
#include "autosql/schema/constraint.h"

namespace asql {

Column::Column(const parse::ColumnParse& col)
  : name_{col.name}, type_{col.type}, expr_{col.expr}, not_null_{col.not_null} {}

}  // namespace asql
