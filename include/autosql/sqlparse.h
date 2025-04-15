#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "autosql/token.h"

namespace asql {

enum class ParseState {
  UNKNOWN,
  TABLE,
  COLUMN,
  CONSTRAINT,
  EXPRESSION,
  PRIMARY_KEY
};

class Tokenizer {
std::string data_;
size_t cursor_;

void skip_comments() {
  while (cursor_ != std::string::npos) {
    std::string_view next = std::string_view(data_).substr(cursor_, 2);
    if (next == "--") {
      cursor_ = data_.find('\n', cursor_);
      cursor_ = data_.find_first_not_of(" \t\r\n", cursor_);
    } else if (next == "/*") {
      cursor_ = data_.find("*/", cursor_) + 2;
      cursor_ = data_.find_first_not_of(" \t\r\n", cursor_);
    } else break;
  }
}
public:
  ParseState state;

  Tokenizer(std::string_view input_data) : data_{input_data} {
    cursor_ = input_data.find_first_not_of(" \t\r\n");
    skip_comments();
  }

  Token next_token() {
    if (cursor_ == std::string::npos)
      return Token();  // TODO out of bounds error
    size_t start_pos = cursor_;
    size_t end_pos;
    switch (data_[start_pos]) {
      case '"':
      case '\'':
        end_pos = data_.find(data_[start_pos], start_pos + 1);
        ++start_pos;
        cursor_ = end_pos + 1;
        break;
      case '(':
      case ')':
      case ',':
      case ';':
        end_pos = start_pos + 1;
        cursor_  = end_pos;
        break;
      default:
        end_pos = data_.find_first_of(" \t\r\n'\"(),;", start_pos);
        cursor_  = end_pos;
    }

    if (cursor_ >= data_.length()) {
      cursor_ = std::string::npos;
    } else {
      cursor_ = data_.find_first_not_of(" \t\r\n", cursor_);
      skip_comments();
    }

    return Token(std::string_view(data_).substr(start_pos, end_pos - start_pos));
  }

  bool done() { return cursor_ == std::string::npos; }
};
}  // namespace asql
