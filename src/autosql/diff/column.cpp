module;

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>


module asql.diff;

namespace asql::diff {
ColumnDiff::ColumnDiff(Column old_col, Column new_col) : name_(old_col.name()) {
  if (old_col.name() != new_col.name()) {
    throw std::runtime_error("Column rename operations are not supported");
  }

  if (old_col.type() != new_col.type()) {
    type_diff_ = DatatypeDiff(old_col.type(), new_col.type());
  }
}

std::string ColumnDiff::sql() const {
  return std::format("ALTER COLUMN \"{}\"", std::string_view{name_});
}
}  // namespace asql::diff
