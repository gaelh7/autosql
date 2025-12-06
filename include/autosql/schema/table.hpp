#pragma once

#include <functional>
#include <string_view>
#include <unordered_map>

#include "autosql/schema/column.hpp"
#include "autosql/schema/constraint.hpp"


import asql.parse;
import asql.symbols;

namespace asql {

class Table {
  Identifier name_;
  std::unordered_map<Identifier, Column, IdHash, std::equal_to<>> columns_;
  std::vector<Unique> unique_cons;
  std::vector<Check> check_cons;
  std::vector<ForeignKey> ref_cons;
  std::vector<Column*> primary_key;

public:
  Table(const parse::TableParse& table);

  void set_constraints(const Database& database,
                       const parse::TableParse& table);

  const Column* column(std::string_view name) const;
};

}  // namespace asql
