#pragma once

#include <optional>
#include <string>

#include "autosql/constraint.h"
#include "autosql/expression.h"
#include "autosql/parser.h"

namespace asql {
namespace parse {

class ColumnParse {
  void parse_constraints(Tokenizer& tokens);

public:
  std::string name;
  std::string type;
  ExpressionParse expr;
  std::optional<CheckParse> check;
  std::optional<ForeignKeyParse<ColumnParse>> reference;
  std::optional<UniqueParse> unique;
  bool not_null  = false;
  bool generated = false;

  ColumnParse() = default;

  ColumnParse(Tokenizer& tokens);
};
}  // namespace parse
}  // namespace asql
