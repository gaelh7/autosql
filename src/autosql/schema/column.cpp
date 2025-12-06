#include "autosql/schema/column.hpp"

#include "autosql/schema/constraint.hpp"

import asql.parse;

namespace asql {

Column::Column(const parse::ColumnParse& col)
  : name_{col.name},
    type_{col.type},
    expr_{col.expr},
    not_null_{col.not_null},
    generated_{col.generated} {}

}  // namespace asql
