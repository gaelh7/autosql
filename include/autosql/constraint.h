#pragma once

#include <string>

namespace asql {
enum class ConstraintType {
  CHECK,
  NOT_NULL,
  DEFAULT,
  GENERATED,
  UNIQUE,
  KEY,
  REFERENCE
};

class Constraint {
public:
  std::string name;
  ConstraintType type;
  std::string expr;
  std::string index_params;
};
}  // namespace asql
