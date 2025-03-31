#pragma once

#include <cstddef>
#include <string_view>
#include <vector>

#include "autosql/constraint.h"
#include "autosql/table.h"

namespace asql {
inline std::vector<Constraint> parse_contraints(std::string_view sql) {
  std::vector<Constraint> constraints;

  size_t token_pos = sql.find_first_not_of(" \t\r\n");
  size_t token_end_pos;
  while (token_pos != std::string_view::npos) {
    Constraint curr;
    if (sql.substr(token_pos, 10) == "CONSTRAINT") {
      token_pos     = sql.find_first_not_of(" \t\r\n", token_pos + 10);
      token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
      curr.name     = sql.substr(token_pos, token_end_pos - token_pos);
      token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
    }

    if (sql.substr(token_pos, 3) == "NOT") {
      token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 3);
      if (sql.substr(token_pos, 4) == "NULL") {
        curr.type = ConstraintType::NOT_NULL;
        constraints.push_back(curr);
        token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 4);
        continue;
      }
      // TODO error
    }

    if (sql.substr(token_pos, 7) == "PRIMARY") {
      token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 7);
      if (sql.substr(token_pos, 3) == "KEY") {
        curr.type = ConstraintType::KEY;
        constraints.push_back(curr);
        token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 3);
        continue;
      }
      // TODO error
    }

    if (sql.substr(token_pos, 7) == "UNIQUE") {
      curr.type = ConstraintType::UNIQUE;
      constraints.push_back(curr);
      token_pos = sql.find_first_not_of(" \t\r\n", token_pos + 7);
      continue;
    }

    if (sql.substr(token_pos, 5) == "CHECK") {
      curr.type     = ConstraintType::CHECK;
      token_pos     = sql.find('(', token_pos + 5) + 1;
      token_end_pos = sql.find(')', token_pos);
      curr.expr     = sql.substr(token_pos, token_end_pos - token_pos);
      constraints.push_back(curr);
      token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
      continue;
    }

    if (sql.substr(token_pos, 7) == "DEFAULT") {
      curr.type     = ConstraintType::DEFAULT;
      token_pos     = sql.find('(', token_pos + 7) + 1;
      token_end_pos = sql.find(')', token_pos);
      curr.expr     = sql.substr(token_pos, token_end_pos - token_pos);
      constraints.push_back(curr);
      token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
      continue;
    }

    if (sql.substr(token_pos, 2) == "AS") {
      curr.type     = ConstraintType::GENERATED;
      token_pos     = sql.find('(', token_pos + 2) + 1;
      token_end_pos = sql.find(')', token_pos);
      curr.expr     = sql.substr(token_pos, token_end_pos - token_pos);
      constraints.push_back(curr);
      token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos + 1);
      continue;
    }
    // TODO Add Foreign Key constraints

    // TODO error
  }

  return constraints;
}

inline Column parse_column(std::string_view sql) {
  Column col;
  size_t token_pos     = sql.find_first_not_of(" \t\r\n");
  size_t token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
  col.name             = sql.substr(token_pos, token_end_pos - token_pos);

  token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
  token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
  col.type      = sql.substr(token_pos, token_end_pos - token_pos);

  token_pos = sql.find_first_not_of(" \t\r\n", token_end_pos);
  if (token_pos == std::string_view::npos) return col;
  if (sql.substr(token_pos, 7) == "STORAGE") {
    token_pos     = sql.find_first_not_of(" \t\r\n", token_pos + 7);
    token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
    col.storage   = sql.substr(token_pos, token_end_pos - token_pos);
    token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
    if (token_pos == std::string_view::npos) return col;
  }

  if (sql.substr(token_pos, 11) == "COMPRESSION") {
    token_pos       = sql.find_first_not_of(" \t\r\n", token_pos + 11);
    token_end_pos   = sql.find_first_of(" \t\r\n", token_pos);
    col.compression = sql.substr(token_pos, token_end_pos - token_pos);
    token_pos       = sql.find_first_not_of(" \t\r\n", token_end_pos);
    if (token_pos == std::string_view::npos) return col;
  }

  if (sql.substr(token_pos, 7) == "COLLATE") {
    token_pos     = sql.find_first_not_of(" \t\r\n", token_pos + 7);
    token_end_pos = sql.find_first_of(" \t\r\n", token_pos);
    col.collate   = sql.substr(token_pos, token_end_pos - token_pos);
    token_pos     = sql.find_first_not_of(" \t\r\n", token_end_pos);
    if (token_pos == std::string_view::npos) return col;
  }

  col.constraints = parse_contraints(sql.substr(token_pos));

  return col;
}

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
    column_list.push_back(parse_column(columns.substr(pos, next - pos)));
    pos  = next + 1;
    next = columns.find(',', pos);
  }
  column_list.push_back(parse_column(columns.substr(pos)));
  return Table(name, column_list);
}
}  // namespace asql
