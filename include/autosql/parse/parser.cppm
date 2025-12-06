module;

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>

export module asql.parse:parser;

import asql.symbols;

export namespace asql::parse {

class Lexer {
  std::ifstream file_;
  size_t line_;
  size_t column_;
  std::string data_;
  Token curr_;

  bool seek_next();

  void parse_string();

  void parse_num() noexcept;

  void parse_float() noexcept;

  void read_token();

public:
  Lexer(std::filesystem::path filename);

  void expect(TokenId id);

  const Token& operator*() const noexcept { return curr_; }

  const Token* operator->() const noexcept { return &curr_; }

  Lexer& operator++();
};
}  // namespace asql::parse
