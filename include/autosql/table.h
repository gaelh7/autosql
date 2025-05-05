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
  std::vector<Unique> unique_cons;
  std::vector<Check> check_cons;
  std::vector<ForeignKey<Table>> ref_cons;
  std::vector<std::string> primary_key;

  Table() = default;

  Table(Tokenizer& tokens) {
    name = tokens->data;

    ++tokens;
    if (tokens->type != TokenType::OPEN_PAR_T)
      throw std::runtime_error("Error: Expected '('");
    while (tokens->type != TokenType::CLOSE_PAR_T) {
      std::string con_name;
      if ((++tokens)->type == TokenType::CONSTRAINT_T) {
        con_name = (++tokens)->data;
        ++tokens;
      }
      switch (tokens->type) {
        case TokenType::IDENTIFIER_T:
          if (!con_name.empty())
            throw std::runtime_error("Error: Expected constraint");
          columns.try_emplace(tokens->data, tokens);
          break;
        case TokenType::UNIQUE_T:
          if (con_name.empty())
            con_name = name + "_uq" + std::to_string(unique_cons.size());
          unique_cons.emplace_back(con_name);
          ++tokens;
          break;
        case TokenType::CHECK_T:
          if (con_name.empty())
            con_name = name + "_ck" + std::to_string(check_cons.size());
          check_cons.emplace_back(con_name, ++tokens);
          break;
        case TokenType::FOREIGN_T:
          if ((++tokens)->type != TokenType::KEY_T)
            throw std::runtime_error(
                "Error: Expected keyword 'KEY' in FOREIGN KEY constraint");
          if (con_name.empty())
            con_name = name + "_fk" + std::to_string(ref_cons.size());
          ref_cons.emplace_back(con_name, ++tokens);
          break;
        default:
          throw std::runtime_error("Error: Expected column or constraint");
      }
    }
    ++tokens;

    if (tokens->type == TokenType::PRIMARY_T) {
      if ((++tokens)->type == TokenType::KEY_T &&
          (++tokens)->type == TokenType::OPEN_PAR_T) {
        while (tokens->type != TokenType::CLOSE_PAR_T) {
          if ((++tokens)->type != TokenType::IDENTIFIER_T)
            throw std::runtime_error(
                "Error: Expected column name in PRIMARY KEY");
          primary_key.push_back(tokens->data);

          switch ((++tokens)->type) {
            case TokenType::COMMA_T:
            case TokenType::CLOSE_PAR_T: break;
            default:
              throw std::runtime_error(
                  "Error: Unexpected token in primary key");
          }
        }
      }
      throw std::runtime_error("Error: Unexpected token after PRIMARY");
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
      if (col.check) {
        result += " CONSTRAINT ";
        result += col.check->name_;
        result += " CHECK (";
        result += col.check->expr_.raw_;
        result += ')';
      }
      if (col.reference) {
        result += " CONSTRAINT ";
        result += col.reference->name_;
        result += " REFERENCES ";
        result += col.reference->table_;
        result += '(';
        result += col.reference->column_;
        result += ')';
      }
      if (col.unique) {
        result += " CONSTRAINT ";
        result += col.unique->name_;
        result += " UNIQUE";
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
        if (lhs.expr.raw_.empty() || lhs.generated ||
            lhs.expr.raw_ != rhs.expr.raw_) {
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
        if (lhs.expr.raw_.empty() || !lhs.generated ||
            lhs.expr.raw_ != rhs.expr.raw_) {
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

      if (lhs.check && rhs.check &&
          lhs.check->expr_.raw_ == rhs.check->expr_.raw_) {
        if (lhs.check->name_ == rhs.check->name_) {
          result += "ALTER TABLE ";
          result += name;
          result += " RENAME CONSTRAINT ";
          result += lhs.check->name_;
          result += " TO ";
          result += rhs.check->name_;
          result += ';';
        }
      } else {
        if (lhs.check) {
          result += "ALTER TABLE ";
          result += name;
          result += " DROP CONSTRAINT ";
          result += lhs.check->name_;
          result += ';';
        }
        if (rhs.check) {
          result += "ALTER TABLE ";
          result += name;
          result += " ADD ";
          result += rhs.check->name_;
          result += " CHECK (";
          result += rhs.check->expr_.raw_;
          result += ");";
        }
      }

      if (lhs.reference && rhs.reference &&
          lhs.reference->table_ == rhs.reference->table_ &&
          lhs.reference->column_ == rhs.reference->column_) {
        if (lhs.reference->name_ == rhs.reference->name_) {
          result += "ALTER TABLE ";
          result += name;
          result += " RENAME CONSTRAINT ";
          result += lhs.reference->name_;
          result += " TO ";
          result += rhs.reference->name_;
          result += ';';
        }
      } else {
        if (lhs.reference) {
          result += "ALTER TABLE ";
          result += name;
          result += " DROP CONSTRAINT ";
          result += lhs.reference->name_;
          result += ';';
        }
        if (rhs.reference) {
          result += "ALTER TABLE ";
          result += name;
          result += " ADD ";
          result += rhs.reference->name_;
          result += " FOREIGN KEY ";
          result += rhs.reference->table_;
          result += '(';
          result += rhs.reference->column_;
          result += ");";
        }
      }

      if (lhs.unique && rhs.unique) {
        if (lhs.unique->name_ == rhs.unique->name_) {
          result += "ALTER TABLE ";
          result += name;
          result += " RENAME CONSTRAINT ";
          result += lhs.unique->name_;
          result += " TO ";
          result += rhs.unique->name_;
          result += ';';
        }
      } else if (lhs.unique) {
        result += "ALTER TABLE ";
        result += name;
        result += " DROP CONSTRAINT ";
        result += lhs.unique->name_;
        result += ';';
      } else if (rhs.unique) {
        result += "ALTER TABLE ";
        result += name;
        result += " ADD ";
        result += rhs.unique->name_;
        result += " UNIQUE ";
        result += '(';
        result += rhs.name;
        result += ");";
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
