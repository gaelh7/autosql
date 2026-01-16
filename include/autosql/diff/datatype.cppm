module;

#include <optional>
#include <string>

export module asql.diff:datatype;

import asql.schema;

export namespace asql::diff {

class DatatypeDiff {
  Datatype old_;
  Datatype new_;
  std::optional<Expression> conversion_;

public:
  DatatypeDiff(Datatype old_type, Datatype new_type);

  std::string sql() const;
};

}  // namespace asql::diff
