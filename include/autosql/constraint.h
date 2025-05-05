#pragma once

#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "autosql/expression.h"
#include "autosql/token.h"

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

  ForeignKey(std::string_view name, Tokenizer& tokens) : Constraint{name} {
    table_ = tokens->data;
    if ((++tokens)->type != TokenType::OPEN_PAR_T)
      throw std::runtime_error("Error: expected '('");
    column_ = (++tokens)->data;
    if ((++tokens)->type != TokenType::CLOSE_PAR_T)
      throw std::runtime_error("Error: expected ')'");
    ++tokens;
  }
};

template <>
class ForeignKey<Table> : public Constraint {
public:
  std::string table_;
  std::vector<std::pair<std::string, std::string>> columns_;

  ForeignKey(std::string_view name, Tokenizer& tokens) : Constraint{name} {
    if (tokens->type != TokenType::OPEN_PAR_T)
      throw std::runtime_error("Error: expected '('");
    while (tokens->type != TokenType::CLOSE_PAR_T) {
      columns_.emplace_back().first = (++tokens)->data;
      switch ((++tokens)->type) {
        case TokenType::COMMA_T:
        case TokenType::CLOSE_PAR_T: break;
        default: throw std::runtime_error("Error: Expected ')' or ','");
      }
    }

    if ((++tokens)->type != TokenType::REFERENCES_T)
      throw std::runtime_error("Error: Expected keyword 'REFERENCE'");

    table_ = (++tokens)->data;
    if ((++tokens)->type != TokenType::OPEN_PAR_T)
      throw std::runtime_error("Error: expected '('");
    for (auto& [col, ref] : std::span{columns_}.first(columns_.size() - 1)) {
      ref = (++tokens)->data;
      if ((++tokens)->type != TokenType::COMMA_T)
        throw std::runtime_error(
            "Error: Not enough references to match number of columns");
    }
    columns_.back().second = (++tokens)->data;
    if ((++tokens)->type != TokenType::CLOSE_PAR_T)
      throw std::runtime_error("Error: expected ')'");
    ++tokens;
  }
};

class Unique : public Constraint {
  using Constraint::Constraint;
};
}  // namespace asql
