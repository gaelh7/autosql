#pragma once

#include <optional>
#include <string>

#include "autosql/constraint.h"
#include "autosql/expression.h"
#include "autosql/parser.h"

namespace asql {

class Column {
  void parse_constraints(Tokenizer& tokens);

public:
  std::string name;
  std::string type;
  Expression expr;
  std::optional<Check> check;
  std::optional<ForeignKey<Column>> reference;
  std::optional<Unique> unique;
  bool not_null  = false;
  bool generated = false;

  Column() = default;

  Column(Tokenizer& tokens);
};
}  // namespace asql
