#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "autosql/column.h"
#include "autosql/sqlparse.h"

namespace asql {

class Table {
public:
  std::string name;
  std::unordered_map<std::string, Column> columns;
  std::vector<std::string> primary_key;

  Table(std::string_view sql) {
    Tokenizer parser{sql};
    if (parser.next_token() != "CREATE" || parser.next_token() != "TABLE") {
      return;
    }
    name = parser.next_token();
    Tokenizer column_parser{parser.next_token()};
    while (!column_parser.done()) {
      Column col{column_parser.until(",")};
      columns[col.name] = col;
    }
    if (parser.done()) return;

    if (parser.next_token() == "PRIMARY") {
      if (parser.next_token() == "KEY") {
        Tokenizer pkey_parser{parser.next_token()};
        while (!pkey_parser.done()) {
          primary_key.push_back(std::string{pkey_parser.until(",")});
        }
      }
      // TODO error
    }
  }
};

class TableDiff {
public:
  std::string name;
  std::vector<Column> add;
  std::vector<std::string> drop;
  std::vector<ColumnDiff> alter;
  std::optional<std::vector<std::string>> primary_key;

  TableDiff(Table lhs, Table rhs) : name{rhs.name} {
    if (lhs.name != rhs.name)
      throw std::invalid_argument("Rename operations not supported");

    if (lhs.primary_key != rhs.primary_key) primary_key = rhs.primary_key;

    for (auto& [name, column] : lhs.columns) {
      auto it = rhs.columns.find(name);
      if (it == rhs.columns.end()) drop.emplace_back(name);
      else alter.emplace_back(column, it->second);
    }

    for (auto& [name, column] : rhs.columns) {
      auto it = lhs.columns.find(name);
      if (it == lhs.columns.end()) add.push_back(column);
    }
  }

  std::string sql() {
    std::string result;
    for (Column& col : add) {
      result += "ALTER TABLE ";
      result += name;
      result += " ADD COLUMN ";
      result += col.name;
      result += " ";
      result += col.type;
      if (col.not_null) result += " NOT NULL";
      if (col.unique) result += " UNIQUE";
      if (col.expr != "") {
        if (col.generated) result += " GENERATED ALWAYS AS (";
        else result += " DEFAULT (";
        result += col.expr;
        result += ')';
      }
      if (col.reference.first != "") {
        result += ' ';
        result += col.reference.first;
        result += '(';
        result += col.reference.second;
        result += ')';
      }
      if (col.constraint.expr != "") {
        if (col.constraint.name != "") {
          result += " CONSTRAINT ";
          result += col.constraint.name;
        }
        result += " CHECK ";
        result += '(';
        result += col.constraint.expr;
        result += ')';
      }
      result += ';';
    }
    for (std::string_view col : drop) {
      result += "ALTER TABLE ";
      result += name;
      result += " DROP COLUMN ";
      result += col;
      result += ';';
    }
    return result;
  }
};
}  // namespace asql
