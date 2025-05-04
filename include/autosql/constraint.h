#pragma once

#include <string>
#include <string_view>

#include "autosql/expression.h"

namespace asql {

class Constraint {
public:
  std::string name_;

  Constraint() = default;

  Constraint(std::string_view name) : name_{name} {}
};

class Check : public Constraint {
public:
  Expression expr_;

  Check(std::string_view name, const Expression& expr)
    : Constraint(name), expr_{expr} {}
};

class ForeignKey : public Constraint {
public:
  std::string table_;
  std::string column_;

  ForeignKey(std::string_view name, std::string_view table,
             std::string_view column)
    : Constraint{name}, table_{table}, column_{column} {}
};

class Unique : public Constraint {
  using Constraint::Constraint;
};
}  // namespace asql
