module;

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

export module asql.parse:table;

import :parser;
import :column;
import :constraint;
import asql.symbols;

export namespace asql::parse {

class TableParse {
public:
  Identifier name;
  std::unordered_map<Identifier, ColumnParse, IdHash, std::equal_to<>> columns;
  std::vector<UniqueTableParse> unique_cons;
  std::vector<CheckParse> check_cons;
  std::vector<ForeignKeyTableParse> ref_cons;
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
}  // namespace asql::parse
