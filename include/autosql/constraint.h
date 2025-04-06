#pragma once

#include <string>
#include <utility>
#include <variant>

#include "autosql/expression.h"

namespace asql {

enum class ConstraintType { CHECK, REFERENCE, UNIQUE };

class Constraint {
public:
  std::string name_;
  std::variant<Expression, std::pair<std::string, std::string>> val_;
};
}  // namespace asql
