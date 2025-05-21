#pragma once

#include <string>
#include <unordered_map>

#include "autosql/parse/table.h"
#include "autosql/schema/column.h"

namespace asql {

class Table {
  std::string name_;
  std::unordered_map<std::string, Column> columns_;

public:
  Table(const parse::TableParse& table);

  void set_constraints(const Database& database, const parse::TableParse& table);

  const Column* column(std::string_view name) const;
};

}  // namespace asql
