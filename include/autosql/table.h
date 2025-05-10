#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "autosql/column.h"
#include "autosql/constraint.h"
#include "autosql/parser.h"

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

  Table(Tokenizer& tokens);
};

class TableDiff {
public:
  std::string name;
  std::vector<Column> add;
  std::vector<std::string> drop;
  std::vector<std::pair<Column, Column>> alter;
  std::optional<std::vector<std::string>> primary_key;

  TableDiff(const Table& lhs, const Table& rhs);

  std::string sql();
};
}  // namespace asql
