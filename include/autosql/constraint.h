#pragma once

#include <string>

namespace asql {
enum class ConstraintType {
  CHECK
};

class Constraint {
public:
  std::string name;
  ConstraintType type;
  std::string expr;
};
}  // namespace asql
