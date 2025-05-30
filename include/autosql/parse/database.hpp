#pragma once

#include <filesystem>
#include <functional>
#include <unordered_map>

#include "autosql/parse/parser.hpp"
#include "autosql/parse/table.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

class DatabaseParse {
public:
  std::unordered_map<Identifier, TableParse, IdHash, std::equal_to<>> tables_;

  DatabaseParse(std::filesystem::path script) {
    Lexer tokens{script};
    while (tokens->type != TokenId::Eof) {
      if (tokens->type == TokenId::Create &&
          (++tokens)->type == TokenId::Table) {
        ++tokens;
        tables_.try_emplace(tokens->str(), tokens);
      }
      tokens.expect(TokenId::Semicolon);
      ++tokens;
    }
  }
};
}  // namespace asql::parse
