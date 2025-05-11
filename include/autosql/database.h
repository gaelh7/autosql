#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "autosql/parser.h"
#include "autosql/table.h"
#include "autosql/token.h"

namespace asql {
namespace parse {

class DatabaseParse {
public:
  std::unordered_map<std::string, TableParse> tables_;

  DatabaseParse(std::filesystem::path script) {
    Tokenizer tokens{script};
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
}  // namespace parse
}  // namespace asql
