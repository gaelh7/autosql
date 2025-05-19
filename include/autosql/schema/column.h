#include <string>

#include "autosql/parse/column.h"
#include "autosql/parse/constraint.h"
#include "autosql/parse/expression.h"

namespace asql {

class Database;

class Column {
  std::string name_;
  std::string type_;
  parse::ExpressionParse expr_;
  std::optional<parse::CheckParse> check_;
  std::optional<parse::ForeignKeyParse<parse::ColumnParse>> reference_;
  std::optional<parse::UniqueParse> unique_;
  bool not_null_  = false;

public:
  Column(const parse::ColumnParse& col);

  void set_constraints(const Database& database, const parse::ColumnParse& col);
};

}  // namespace asql
