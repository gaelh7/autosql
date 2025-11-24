#include "autosql/parse/table.hpp"

#include "autosql/parse/parser.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

TableParse::TableParse(Lexer& tokens) {
  name = tokens->str();

  (++tokens).expect(TokenId::OpenPar);
  while (tokens->type != TokenId::ClosePar) {
    std::string con_name;
    if ((++tokens)->type == TokenId::Constraint) {
      con_name = (++tokens)->str();
      ++tokens;
    }
    switch (tokens->type) {
      case TokenId::Identifier:
        if (!con_name.empty())
          throw std::runtime_error("Error: Expected constraint");
        columns.try_emplace(tokens->str(), tokens);
        break;
      case TokenId::Unique:
        if (con_name.empty())
          con_name = std::string{name} + "_uq" + std::to_string(unique_cons.size());
        unique_cons.emplace_back(con_name, ++tokens);
        break;
      case TokenId::Check:
        if (con_name.empty())
          con_name = std::string{name} + "_ck" + std::to_string(check_cons.size());
        check_cons.emplace_back(con_name, ++tokens);
        break;
      case TokenId::Foreign:
        (++tokens).expect(TokenId::Key);
        if (con_name.empty())
          con_name = std::string{name} + "_fk" + std::to_string(ref_cons.size());
        ref_cons.emplace_back(con_name, ++tokens);
        break;
      default: throw std::runtime_error("Error: Expected column or constraint");
    }
  }
  ++tokens;

  if (tokens->type == TokenId::Primary) {
    (++tokens).expect(TokenId::Key);
    (++tokens).expect(TokenId::OpenPar);
    while (tokens->type != TokenId::ClosePar) {
      (++tokens).expect(TokenId::Identifier);

      primary_key.push_back(std::string{tokens->str()});
      switch ((++tokens)->type) {
        case TokenId::Comma:
        case TokenId::ClosePar: break;
        default:
          throw std::runtime_error("Error: Unexpected token in primary key");
      }
    }
    ++tokens;
  }
}

TableDiff::TableDiff(const TableParse& lhs, const TableParse& rhs)
  : name{rhs.name} {
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

std::string TableDiff::sql() {
  std::string result;
  for (ColumnParse& col : add) {
    result += "ALTER TABLE ";
    result += name;
    result += " ADD COLUMN ";
    result += col.name;
    result += " ";
    result += col.type.str();
    if (col.not_null) result += " NOT NULL";
    if (col.expr) {
      if (col.generated) result += " GENERATED ALWAYS AS (";
      else result += " DEFAULT (";
      result += col.expr->str();
      result += ")";
    }
    if (col.check) {
      result += " CONSTRAINT ";
      result += col.check->name_;
      result += " CHECK (";
      result += col.check->expr_.str();
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
    if (lhs.type.str() != rhs.type.str()) {
      result += "ALTER TABLE ";
      result += name;
      result += " ALTER COLUMN ";
      result += rhs.name;
      result += " TYPE ";
      result += rhs.type.str();
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
    if (rhs.expr && !rhs.generated) {
      if (!lhs.expr || lhs.generated || lhs.expr->str() != rhs.expr->str()) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " SET DEFAULT (";
        result += rhs.expr->str();
        result += ");";
      }
    } else if (lhs.expr && !lhs.generated) {
      result += "ALTER TABLE ";
      result += name;
      result += " ALTER COLUMN ";
      result += rhs.name;
      result += " DROP DEFAULT;";
    }
    if (rhs.expr && rhs.generated) {
      if (!lhs.expr || !lhs.generated || lhs.expr->str() != rhs.expr->str()) {
        result += "ALTER TABLE ";
        result += name;
        result += " ALTER COLUMN ";
        result += rhs.name;
        result += " SET EXPRESSION (";
        result += rhs.expr->str();
        result += ");";
      }
    } else if (lhs.expr && lhs.generated) {
      result += "ALTER TABLE ";
      result += name;
      result += " ALTER COLUMN ";
      result += rhs.name;
      result += " DROP EXPRESSION;";
    }

    if (lhs.check && rhs.check &&
        lhs.check->expr_.str() == rhs.check->expr_.str()) {
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
        result += rhs.check->expr_.str();
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
}  // namespace asql::parse
