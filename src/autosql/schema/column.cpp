#include "autosql/schema/column.h"

#include "autosql/parse/column.h"
#include "autosql/schema/constraint.h"

namespace asql {

Column::Column(const parse::ColumnParse& col)
  : name_{col.name}, type_{col.type} {}

void Column::set_constraints(const Database& database,
                             const parse::ColumnParse& column) {
  expr_      = column.expr;
  check_     = column.check;
  if (column.reference)
    reference_ = ForeignKey<Column>{database, *column.reference};
  unique_    = column.unique;
  not_null_  = column.not_null;
}

}  // namespace asql
