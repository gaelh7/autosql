#pragma once

#include <string>
#include <vector>

#include "autosql/column.h"
#include "autosql/sqlparse.h"

namespace asql {

class Table {
public:
  std::string name;
  std::vector<Column> columns;
  std::vector<std::string> primary_key;

  Table(std::string_view sql) {
    size_t create_pos   = sql.find("CREATE");
    size_t table_pos    = sql.find("TABLE");
    size_t name_pos     = sql.find_first_not_of(" \t\r\n", table_pos + 5);
    size_t name_end_pos = sql.find_first_of(" \t\r\n(", name_pos);
    name                = sql.substr(name_pos, name_end_pos - name_pos);
    size_t open_par     = sql.find('(', name_end_pos);
    size_t end_par      = closing_par(sql, open_par);
    std::string_view column_str =
        sql.substr(open_par + 1, end_par - open_par - 1);
    size_t pos  = 0;
    size_t next = column_str.find(',');
    while (next != std::string_view::npos) {
      columns.push_back(Column(column_str.substr(pos, next - pos)));
      pos  = next + 1;
      next = column_str.find(',', pos);
    }
    columns.push_back(Column(column_str.substr(pos)));
    if (end_par + 1 == sql.length()) return;

    size_t token_pos = sql.find_first_not_of(" \t\r\n", end_par + 1);
    if (sql.substr(token_pos, 7) == "PRIMARY") {
      token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 7);
      if (sql.substr(token_pos, 3) == "KEY") {
        token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 3);
        if (sql[token_pos] == '(') {
          size_t close_par = closing_par(sql, token_pos);
          std::string_view pkey_str = sql.substr(token_pos + 1, close_par - token_pos - 1);
          token_pos = pkey_str.find_first_not_of(" \t\r\n");
          size_t token_end_pos = pkey_str.find_first_of(" \t\r\n,", token_pos);
          next = pkey_str.find(',', token_end_pos);
          while (next != std::string_view::npos) {
            primary_key.push_back(std::string{pkey_str.substr(token_pos, token_end_pos - token_pos)});
            token_pos = pkey_str.find_first_not_of(" \t\r\n", next + 1);
            token_end_pos = pkey_str.find_first_of(" \t\r\n,", token_pos);
            next = pkey_str.find(',', token_end_pos);
          }
          primary_key.push_back(std::string{pkey_str.substr(token_pos, token_end_pos - token_pos)});
        }
      }
      // TODO error
    }
  }
};
}  // namespace asql
