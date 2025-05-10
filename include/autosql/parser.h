#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>

#include "autosql/token.h"

namespace asql {

class Tokenizer {
  std::ifstream file_;
  std::string data_;
  size_t line_;
  size_t column_;
  Token curr_;

  void skip_comments();

  void parse_string();

  void parse_num() noexcept;

  void parse_float() noexcept;

  void next_token();

public:
  Tokenizer(std::filesystem::path filename) : file_{filename}, line_{0}, column_{0} {
    if (!file_.is_open()) throw std::runtime_error("Error: couldn't open file");
    file_.exceptions(std::ifstream::failbit);
    std::getline(file_, data_);
    skip_comments();
    next_token();
  }

  const Token& operator*() const noexcept { return curr_; }

  const Token* operator->() const noexcept { return &curr_; }

  Tokenizer& operator++() {
    next_token();
    return *this;
  }

  bool done() const { return data_.empty(); }
};

class Parser {
  std::filesystem::path filename_;

public:
  Parser(std::filesystem::path file) : filename_{file} {}

  Tokenizer begin() { return Tokenizer{filename_}; }

  Tokenizer end() { return Tokenizer{""}; }
};
}  // namespace asql
