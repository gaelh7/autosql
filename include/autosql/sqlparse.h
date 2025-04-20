#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

#include "autosql/token.h"

namespace asql {

enum class ParseState {
  UNKNOWN,
  SCRIPT,
  TABLE,
  COLUMN,
  CONSTRAINT,
  EXPRESSION,
  PRIMARY_KEY
};

class Tokenizer {
  std::string_view data_;
  Token curr_;

  void skip_comments() {
    size_t cursor = data_.find_first_not_of(" \t\r\n");
    while (cursor != std::string::npos) {
      if (data_.substr(cursor, 2) == "--") {
        cursor = data_.find('\n', cursor) + 1;
        cursor = data_.find_first_not_of(" \t\r\n", cursor);
      } else if (data_.substr(cursor, 2) == "/*") {
        cursor = data_.find("*/", cursor) + 2;
        cursor = data_.find_first_not_of(" \t\r\n", cursor);
      } else break;
    }
    data_.remove_prefix(std::min(cursor, data_.length()));
  }

  void parse_string() {
    size_t end_pos = data_.find(data_[0], 1);
    if (end_pos == std::string_view::npos)
      throw std::runtime_error("Unterminated string literal");
    curr_ = Token(data_.substr(1, end_pos - 1), TokenType::STRING_LITERAL_T);
    data_.remove_prefix(end_pos + 1);
  }

  void next_token() {
    if (data_.empty()) return;
    switch (data_[0]) {
      case '"':
      case '\'': parse_string(); break;
      case '(':
      case ')':
      case ',':
      case ';':
        curr_ = Token(data_.substr(0, 1));
        data_.remove_prefix(1);
        break;
      default: {
        size_t end_pos = data_.find_first_of(" \t\r\n'\"(),;");
        curr_          = Token(data_.substr(0, end_pos));
        data_.remove_prefix(end_pos);
      }
    }
    skip_comments();
  }

public:
  ParseState state;

  Tokenizer(std::string_view input_data)
    : data_{input_data}, state{ParseState::SCRIPT} {
    skip_comments();
    next_token();
  }

  Token& operator*() { return curr_; }

  Token* operator->() { return &curr_; }

  Tokenizer& operator++() {
    next_token();
    return *this;
  }

  Tokenizer operator++(int) {
    Tokenizer tmp{*this};
    next_token();
    return tmp;
  }

  bool operator==(const Tokenizer& other) {
    return data_ == other.data_;
  }

  bool done() { return data_.empty(); }
};

class Parser {
  std::string data_;

public:
  Parser(std::string_view script) : data_{script} {}

  Tokenizer begin() {
    return Tokenizer(data_);
  }

  Tokenizer end() {
    return Tokenizer("");
  }
};
}  // namespace asql
