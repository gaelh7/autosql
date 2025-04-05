#pragma once

#include <string>

namespace asql {

enum class ConstraintType { CHECK, REFERENCE, UNIQUE };

class Constraint {
public:
  std::string name;
  std::string reference;
  std::string expr;
};
}  // namespace asql
