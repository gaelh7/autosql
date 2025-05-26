#pragma once

#include <string>
#include <unordered_map>

#include "autosql/parse/table.h"
#include "autosql/schema/column.h"
#include "autosql/schema/constraint.h"

namespace asql {

class Table {
  std::string name_;
  std::unordered_map<std::string, Column> columns_;
  std::vector<Unique> unique_cons;
  std::vector<Check> check_cons;
  std::vector<ForeignKey> ref_cons;
  std::vector<Column*> primary_key;

public:
  Table(const parse::TableParse& table);

  void set_constraints(const Database& database, const parse::TableParse& table);

  const Column* column(std::string_view name) const;
};

}  // namespace asql
