#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

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

  Check(std::string_view name, Tokenizer& tokens)
    : Constraint{name}, expr_{tokens} {}
};

class Column;
class Table;

template <typename T>
class ForeignKey;

template <>
class ForeignKey<Column> : public Constraint {
public:
  std::string table_;
  std::string column_;

  ForeignKey(std::string_view name, Tokenizer& tokens);
};

template <>
class ForeignKey<Table> : public Constraint {
public:
  std::string table_;
  std::vector<std::pair<std::string, std::string>> columns_;

  ForeignKey(std::string_view name, Tokenizer& tokens);
};

class Unique : public Constraint {
  using Constraint::Constraint;
};
}  // namespace asql
