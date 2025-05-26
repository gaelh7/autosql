#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "autosql/parse/parser.hpp"
#include "autosql/parse/table.hpp"
#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

class DatabaseParse {
public:
  std::unordered_map<std::string, TableParse> tables_;

  DatabaseParse(std::filesystem::path script) {
    Lexer tokens{script};
    while (tokens->type != TokenType::Eof) {
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
