#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "autosql/parser.h"
#include "autosql/table.h"
#include "autosql/token.h"

namespace asql {

class Database {
  Parser parser_;

public:
  std::unordered_map<std::string, Table> tables_;

  Database(std::filesystem::path script) : parser_(script) {
    Tokenizer tokens = parser_.begin();
    while (!tokens.done()) {
      if (tokens->type == TokenType::Create &&
          (++tokens)->type == TokenType::Table) {
        ++tokens;
        tables_.try_emplace(std::string{tokens->str()}, tokens);
      }
      if (tokens->type != TokenType::Semicolon) {
        throw std::runtime_error("Expected ';'");
      }
      ++tokens;
    }
  }
};
}  // namespace asql
