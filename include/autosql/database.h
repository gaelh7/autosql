#pragma once

#include <string>
#include <unordered_map>

#include "autosql/sqlparse.h"
#include "autosql/table.h"
#include "autosql/token.h"

namespace asql {
class Database {
  Tokenizer parser_;

public:
  std::unordered_map<std::string, Table> tables_;

  Database(std::string_view script) : parser_(script) {
    while (!parser_.done()) {
      if (parser_.next_token().type_ == TokenType::CREATE_T &&
          parser_.next_token().type_ == TokenType::TABLE_T) {
        parser_.state = ParseState::TABLE;
        Table t{parser_};
        tables_[t.name] = t;
      }
    }
  }
};
}  // namespace asql
