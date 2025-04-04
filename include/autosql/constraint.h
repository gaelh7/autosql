#pragma once

#include <stdexcept>
#include <string>

namespace asql {

class Constraint {
public:
  std::string name;
  std::string expr;
};

class ConstraintDiff {
public:
  std::string name;
  std::string expr;

  ConstraintDiff(Constraint lhs, Constraint rhs) : name{rhs.name}, expr{rhs.expr} {
    if (lhs.name != rhs.name)
      throw std::invalid_argument("Rename operations not supported");
  }
};
}  // namespace asql
