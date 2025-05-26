#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "autosql/parse/column.hpp"
#include "autosql/parse/constraint.hpp"
#include "autosql/parse/parser.hpp"

namespace asql {
namespace parse {

class TableParse {
public:
  std::string name;
  std::unordered_map<std::string, ColumnParse> columns;
  std::vector<UniqueParse> unique_cons;
  std::vector<CheckParse> check_cons;
  std::vector<ForeignKeyParse<TableParse>> ref_cons;
  std::vector<std::string> primary_key;

  TableParse() = default;

  TableParse(Lexer& tokens);
};

class TableDiff {
public:
  std::string name;
  std::vector<ColumnParse> add;
  std::vector<std::string> drop;
  std::vector<std::pair<ColumnParse, ColumnParse>> alter;
  std::optional<std::vector<std::string>> primary_key;

  TableDiff(const TableParse& lhs, const TableParse& rhs);

  std::string sql();
};
}  // namespace parse
}  // namespace asql
