#pragma once

#include <string>
#include <vector>

#include "autosql/column.h"
#include "autosql/sqlparse.h"

namespace asql {

class Table {
public:
  std::string name;
  std::vector<Column> columns;
  std::vector<std::string> primary_key;

  Table(std::string_view sql) {
    Tokenizer parser{sql};
    if (parser.next_token() != "CREATE" || parser.next_token() != "TABLE") {
      return;
    }
    name = parser.next_token();
    Tokenizer column_parser{parser.next_token()};
    while (!column_parser.done()) {
      columns.push_back(Column(column_parser.until(",")));
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
}  // namespace asql
