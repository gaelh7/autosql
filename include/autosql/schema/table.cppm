module;

#include <functional>
#include <string_view>
#include <unordered_map>

export module asql.schema:table;

import :column;
import :constraint;

import asql.parse;
import asql.symbols;

export namespace asql {

class Table {
  Identifier name_;
  std::unordered_map<Identifier, Column, IdHash, std::equal_to<>> columns_;
  std::vector<Unique> unique_cons;
  std::vector<Check> check_cons;
  std::vector<ForeignKey> ref_cons;
  std::vector<Column*> primary_key;

public:
  Table(const parse::TableParse& table);

  void add_reference_constraint(const ForeignKey& fk);

  const Column* column(std::string_view name) const;
};

}  // namespace asql
