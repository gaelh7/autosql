module;

#include <optional>
#include <string_view>


module asql.schema;

import asql.parse;

namespace asql {

Column::Column(const parse::ColumnParse& col)
  : name_{col.name},
    type_{col.type},
    expr_{col.expr},
    not_null_{col.not_null},
    generated_{col.generated} {}

std::string_view Column::name() const { return name_; }

const Datatype& Column::type() const { return type_; }

const std::optional<Expression>& Column::expr() const { return expr_; }

bool Column::nullable() const { return !not_null_; }

bool Column::is_generated() const { return generated_; }

}  // namespace asql
