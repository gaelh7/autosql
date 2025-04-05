#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "autosql/column.h"
#include "autosql/constraint.h"
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
  std::vector<std::pair<Column, Column>> alter;
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
      if (!col.expr.empty()) {
        if (col.generated) result += " GENERATED ALWAYS AS (";
        else result += " DEFAULT (";
        result += col.expr;
        result += ")";
      }
      for (auto& [type, con] : col.constraints) {
        result += " CONSTRAINT ";
        result += con.name;
        switch (type) {
          case ConstraintType::CHECK:
            result += " CHECK (";
            result += con.expr;
            result += ')';
            break;
          case ConstraintType::REFERENCE:
            result += " REFERENCES ";
            result += con.reference;
            result += '(';
            result += con.expr;
            result += ')';
            break;
          case ConstraintType::UNIQUE:
            result += " UNIQUE";
            break;
            result += " DEFAULT (";
            result += con.expr;
            result += ')';
            break;
        }
      }
      result += ';';
    }
    for (auto& [lhs, rhs] : alter) {
      if (lhs.type != rhs.type) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " TYPE ";
        result += rhs.type;
        result += ';';
      }
      if (lhs.not_null) {
        if (!rhs.not_null) {
          result += "ALTER TABLE ";
          result += name;
          result += " ALTER COLUMN ";
          result += rhs.name;
          result += " DROP NOT NULL;";
        }
      } else if (rhs.not_null) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " SET NOT NULL;";
      }
      if (!rhs.expr.empty() && !rhs.generated) {
        if (lhs.expr.empty() || lhs.generated || lhs.expr != rhs.expr) {
          result += "ALTER TABLE ";
          result += name;
          result += " ALTER COLUMN ";
          result += rhs.name;
          result += " SET DEFAULT (";
          result += rhs.expr;
          result += ");";
        }
      } else if (!lhs.expr.empty() && !lhs.generated) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " DROP DEFAULT;";
      }
      if (!rhs.expr.empty() && rhs.generated) {
        if (lhs.expr.empty() || !lhs.generated || lhs.expr != rhs.expr) {
          result += "ALTER TABLE ";
          result += name;
          result += " ALTER COLUMN ";
          result += rhs.name;
          result += " SET EXPRESSION (";
          result += rhs.expr;
          result += ");";
        }
      } else if (!lhs.expr.empty() && lhs.generated) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " DROP EXPRESSION;";
      }
      for (auto& [type, con] : lhs.constraints) {
        if (!rhs.constraints.contains(type)) {
          result += "ALTER TABLE ";
          result += name;
          result += " DROP CONSTRAINT ";
          result += con.name;
          result += ';';
        }
      }
      for (auto& [type, con] : rhs.constraints) {
        auto lhs_it = lhs.constraints.find(type);
        if (lhs_it != lhs.constraints.end()) {
          bool is_equal = true;
          switch (type) {
            case ConstraintType::CHECK:
              is_equal = lhs_it->second.expr == con.expr;
              break;
            case ConstraintType::REFERENCE:
              is_equal = lhs_it->second.expr != con.expr &&
                         lhs_it->second.reference != con.reference;
              break;
            case ConstraintType::UNIQUE: break;
          }
          if (is_equal) {
            if (lhs_it->second.name != con.name) {
              result += "ALTER TABLE ";
              result += name;
              result += " RENAME CONSTRAINT ";
              result += lhs_it->second.name;
              result += " TO ";
              result += con.name;
              result += ';';
            }
            continue;
          } else {
            result += "ALTER TABLE ";
            result += name;
            result += " DROP CONSTRAINT ";
            result += con.name;
            result += ';';
          }
        }
        result += "ALTER TABLE ";
        result += name;
        result += " ADD ";
        result += con.name;
        switch (type) {
          case ConstraintType::CHECK:
            result += " CHECK (";
            result += con.expr;
            result += ");";
            break;
          case ConstraintType::REFERENCE:
            result += " REFERENCES ";
            result += con.reference;
            result += '(';
            result += con.expr;
            result += ");";
            break;
          case ConstraintType::UNIQUE:
            result += " UNIQUE (";
            result += rhs.name;
            result += ");";
            break;
        }
      }
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
