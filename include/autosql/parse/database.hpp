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
    while (tokens->type != TokenId::Eof) {
      if (tokens->type == TokenId::Create &&
          (++tokens)->type == TokenId::Table) {
        ++tokens;
        tables_.try_emplace(std::string{tokens->str()}, tokens);
      }
      if (tokens->type != TokenId::Semicolon) {
        throw std::runtime_error("Expected ';'");
      }
      ++tokens;
    }
  }
};
}  // namespace parse
}  // namespace asql
