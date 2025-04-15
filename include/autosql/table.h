#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "autosql/column.h"
#include "autosql/constraint.h"
#include "autosql/sqlparse.h"
#include "autosql/token.h"

namespace asql {

class Table {
public:
  std::string name;
  std::unordered_map<std::string, Column> columns;
  std::vector<std::string> primary_key;

  Table(Tokenizer& parser) {
    if (parser.next_token().type_ != TokenType::CREATE_T ||
        parser.next_token().type_ != TokenType::TABLE_T ||
        parser.state != ParseState::TABLE) {
      return;
    }
    name         = parser.next_token().raw_;
    parser.next_token();
    parser.state = ParseState::COLUMN;
    while (parser.state == ParseState::COLUMN) {
      Column col{parser};
      columns[col.name] = col;
    }
    if (parser.done()) return;

    if (parser.next_token().type_ == TokenType::PRIMARY_T) {
      if (parser.next_token().type_ == TokenType::KEY_T &&
          parser.next_token().type_ == TokenType::OPEN_PAR_T) {
        parser.state = ParseState::PRIMARY_KEY;
        while (parser.state == ParseState::PRIMARY_KEY) {
          Token t = parser.next_token();
          if (t.type_ != TokenType::IDENTIFIER_T) break;  // TODO error
          primary_key.push_back(t.raw_);

          switch (parser.next_token().type_) {
            case TokenType::COMMA_T: break;
            case TokenType::CLOSE_PAR_T:
              parser.state = ParseState::TABLE;
              break;
            default: break;  // TODO error
          }
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
      if (!col.expr.raw_.empty()) {
        if (col.generated) result += " GENERATED ALWAYS AS (";
        else result += " DEFAULT (";
        result += col.expr.raw_;
        result += ")";
      }
      for (auto& [type, con] : col.constraints) {
        result += " CONSTRAINT ";
        result += con.name_;
        switch (type) {
          case ConstraintType::CHECK:
            result += " CHECK (";
            result += std::get<0>(con.val_).raw_;
            result += ')';
            break;
          case ConstraintType::REFERENCE:
            result += " REFERENCES ";
            result += std::get<1>(con.val_).first;
            result += '(';
            result += std::get<1>(con.val_).second;
            result += ')';
            break;
          case ConstraintType::UNIQUE:
            result += " UNIQUE";
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
      if (!rhs.expr.raw_.empty() && !rhs.generated) {
        if (lhs.expr.raw_.empty() || lhs.generated || lhs.expr.raw_ != rhs.expr.raw_) {
          result += "ALTER TABLE ";
          result += name;
          result += " ALTER COLUMN ";
          result += rhs.name;
          result += " SET DEFAULT (";
          result += rhs.expr.raw_;
          result += ");";
        }
      } else if (!lhs.expr.raw_.empty() && !lhs.generated) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " DROP DEFAULT;";
      }
      if (!rhs.expr.raw_.empty() && rhs.generated) {
        if (lhs.expr.raw_.empty() || !lhs.generated || lhs.expr.raw_ != rhs.expr.raw_) {
          result += "ALTER TABLE ";
          result += name;
          result += " ALTER COLUMN ";
          result += rhs.name;
          result += " SET EXPRESSION (";
          result += rhs.expr.raw_;
          result += ");";
        }
      } else if (!lhs.expr.raw_.empty() && lhs.generated) {
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
          result += con.name_;
          result += ';';
        }
      }
      for (auto& [type, con] : rhs.constraints) {
        auto lhs_it = lhs.constraints.find(type);
        if (lhs_it != lhs.constraints.end()) {
          bool is_equal = true;
          switch (type) {
            case ConstraintType::CHECK:
              is_equal = std::get<0>(lhs_it->second.val_).raw_ == std::get<0>(con.val_).raw_;
              break;
            case ConstraintType::REFERENCE:
              is_equal = std::get<1>(lhs_it->second.val_) == std::get<1>(con.val_);
              break;
            case ConstraintType::UNIQUE: break;
          }
          if (is_equal) {
            if (lhs_it->second.name_ != con.name_) {
              result += "ALTER TABLE ";
              result += name;
              result += " RENAME CONSTRAINT ";
              result += lhs_it->second.name_;
              result += " TO ";
              result += con.name_;
              result += ';';
            }
            continue;
          } else {
            result += "ALTER TABLE ";
            result += name;
            result += " DROP CONSTRAINT ";
            result += con.name_;
            result += ';';
          }
        }
        result += "ALTER TABLE ";
        result += name;
        result += " ADD ";
        result += con.name_;
        switch (type) {
          case ConstraintType::CHECK:
            result += " CHECK (";
            result += std::get<0>(con.val_).raw_;
            result += ");";
            break;
          case ConstraintType::REFERENCE:
            result += " FOREIGN KEY ";
            result += std::get<1>(con.val_).first;
            result += '(';
            result += std::get<1>(con.val_).second;
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
