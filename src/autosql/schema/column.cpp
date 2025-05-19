#include "autosql/schema/column.h"

#include "autosql/parse/column.h"

namespace asql {

Column::Column(const parse::ColumnParse& col)
  : name_{col.name}, type_{col.type.str()} {}

void Column::set_constraints(const Database& _,
                             const parse::ColumnParse& column) {
  expr_      = column.expr;
  check_     = column.check;
  reference_ = column.reference;
  unique_    = column.unique;
  not_null_  = column.not_null;
}

}  // namespace asql
