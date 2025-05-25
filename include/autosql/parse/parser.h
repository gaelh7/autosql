#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>

#include "autosql/parse/token.h"

namespace asql {
namespace parse {

class Tokenizer {
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
  Tokenizer(std::filesystem::path filename)
    : file_{filename}, line_{}, column_{} {
    if (!file_.is_open()) throw std::runtime_error("Error: couldn't open file");
    file_.exceptions(std::ifstream::failbit);
    std::getline(file_, data_);
    read_token();
  }

  const Token& operator*() const noexcept { return curr_; }

  const Token* operator->() const noexcept { return &curr_; }

  Tokenizer& operator++();

  bool done() const { return curr_.type == TokenType::Eof; }
};

class Parser {
  std::filesystem::path filename_;

public:
  Parser(std::filesystem::path file) : filename_{file} {}

  Tokenizer begin() { return Tokenizer{filename_}; }

  Tokenizer end() { return Tokenizer{""}; }
};
}  // namespace parse
}  // namespace asql
