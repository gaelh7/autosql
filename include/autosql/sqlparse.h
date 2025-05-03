#pragma once

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>

#include "autosql/token.h"

namespace asql {

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
    curr_ = Token{data_.substr(1, end_pos - 1), TokenType::STRING_LITERAL_T};
    data_.remove_prefix(end_pos + 1);
  }

  void parse_num() {
    size_t end = data_.find_first_not_of("0123456789", 1);
    if (end == std::string_view::npos) {
      curr_ = Token{data_, TokenType::INT_LITERAL_T};
      data_.remove_prefix(data_.size());
      return;
    }
    switch (data_[end]) {
      case '.':
        end   = data_.find_first_not_of("0123456789", end + 1);
        curr_ = Token{data_.substr(0, end), TokenType::FLOAT_LITERAL_T};
        break;
      default:
        curr_ = Token{data_.substr(0, end), TokenType::INT_LITERAL_T};
        break;
    }
    data_.remove_prefix(end);
  }

  void parse_float() {
    size_t end = data_.find_first_not_of("0123456789", 1);
    if (end == std::string_view::npos) {
      curr_ = Token{data_, TokenType::FLOAT_LITERAL_T};
      data_.remove_prefix(data_.size());
      return;
    }
    curr_ = Token{data_.substr(0, end), TokenType::FLOAT_LITERAL_T};
    data_.remove_prefix(end);
  }

  void next_token() {
    if (data_.empty()) return;
    switch (data_[0]) {
      case '"':
      case '\'': parse_string(); break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': parse_num(); break;
      case '.': parse_float(); break;
      case '(':
        curr_ = Token{"", TokenType::OPEN_PAR_T};
        data_.remove_prefix(1);
        break;
      case ')':
        curr_ = Token{"", TokenType::CLOSE_PAR_T};
        data_.remove_prefix(1);
        break;
      case ',':
        curr_ = Token{"", TokenType::COMMA_T};
        data_.remove_prefix(1);
        break;
      case ';':
        curr_ = Token{"", TokenType::SEMICOLON_T};
        data_.remove_prefix(1);
        break;
      default: {
        size_t end_pos       = data_.find_first_of(" \t\r\n'\"(),;");
        std::string_view raw = data_.substr(0, end_pos);
        auto it              = keyword_map.find(raw);
        if (it != keyword_map.end()) curr_ = Token{"", it->second};
        else curr_ = Token{raw, TokenType::IDENTIFIER_T};
        data_.remove_prefix(end_pos);
      }
    }
    skip_comments();
  }

public:
  Tokenizer(std::string_view input_data) : data_{input_data} {
    skip_comments();
    next_token();
  }

  const Token& operator*() const { return curr_; }

  const Token* operator->() const { return &curr_; }

  Tokenizer& operator++() {
    next_token();
    return *this;
  }

  Tokenizer operator++(int) {
    Tokenizer tmp{*this};
    next_token();
    return tmp;
  }

  bool operator==(const Tokenizer& other) const { return data_ == other.data_; }

  bool done() const { return data_.empty(); }
};

class Parser {
  std::string data_;

public:
  Parser(std::string_view script) : data_{script} {}

  Tokenizer begin() { return Tokenizer(data_); }

  Tokenizer end() { return Tokenizer(""); }
};
}  // namespace asql
