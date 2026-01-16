module;

#include <optional>
#include <string>
#include <utility>


export module asql.diff:column;

import :datatype;

import asql.symbols;
import asql.schema;

export namespace asql::diff {

class ColumnDiff {
  Identifier name_;
  std::optional<DatatypeDiff> type_diff_;
  std::optional<Expression> new_expr_;
  std::pair<bool, bool> not_null_diff_;
  std::pair<bool, bool> generated_diff_;

public:
  ColumnDiff(Column old_col, Column new_col);

  std::string sql() const;
};

}  // namespace asql::diff
