#pragma once

#include <string>
#include <vector>

#include "autosql/column.h"

namespace asql {

class Table {
public:
  std::string name;
  std::vector<Column> columns;
  std::vector<std::string> primary_key;

  Table(std::string_view name, std::vector<Column> columns)
    : name(name), columns(columns) {}
};
}  // namespace asql
