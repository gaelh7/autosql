module;

#include <format>
#include <string>

module asql.diff;

namespace asql::diff {
DatatypeDiff::DatatypeDiff(Datatype old_type, Datatype new_type)
  : old_{old_type}, new_{new_type} {}

std::string DatatypeDiff::sql() const {
  return std::format("SET DATA TYPE {}", new_.name());
}
}  // namespace asql::diff
