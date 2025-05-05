#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "autosql/sqlparse.h"
#include "autosql/table.h"
#include "autosql/token.h"

namespace asql {

class Database {
  Parser parser_;

public:
  std::unordered_map<std::string, Table> tables_;

  Database(std::string_view script) : parser_(script) {
    Tokenizer tokens = parser_.begin();
    while (!tokens.done()) {
      if (tokens->type == TokenType::CREATE_T &&
          (++tokens)->type == TokenType::TABLE_T) {
        ++tokens;
        tables_.try_emplace(std::string{tokens->str()}, tokens);
      }
      if (tokens->type != TokenType::SEMICOLON_T) {
        throw std::runtime_error("Expected ';'");
      }
      ++tokens;
    }
  }
};
}  // namespace asql
