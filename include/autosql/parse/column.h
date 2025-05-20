#pragma once

#include <optional>
#include <string>

#include "autosql/parse/constraint.h"
#include "autosql/parse/datatype.h"
#include "autosql/parse/expression.h"
#include "autosql/parse/parser.h"

namespace asql {
namespace parse {

class ColumnParse {
  void parse_constraints(Tokenizer& tokens);

public:
  std::string name;
  DatatypeParse type;
  std::optional<ExpressionParse> expr;
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
