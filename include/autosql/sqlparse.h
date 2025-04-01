#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "autosql/table.h"

namespace asql {

inline Table parse_table(std::string_view sql) {
  size_t create_pos        = sql.find("CREATE");
  size_t table_pos         = sql.find("TABLE");
  size_t name_pos          = sql.find_first_not_of(" \t\r\n", table_pos + 5);
  size_t name_end_pos      = sql.find_first_of(" \t\r\n(", name_pos);
  std::string_view name    = sql.substr(name_pos, name_end_pos - name_pos);
  size_t open_par          = sql.find('(', name_end_pos);
  size_t end_par           = sql.rfind(')');
  std::string_view columns = sql.substr(open_par + 1, end_par - open_par - 1);
  size_t pos               = 0;
  size_t next              = columns.find(',');
  std::vector<Column> column_list;
  while (next != std::string_view::npos) {
    column_list.push_back(Column(columns.substr(pos, next - pos)));
    pos  = next + 1;
    next = columns.find(',', pos);
  }
  column_list.push_back(Column(columns.substr(pos)));
  return Table(name, column_list);
}
}  // namespace asql
