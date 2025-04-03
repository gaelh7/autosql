#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace asql {

class Tokenizer {
public:
  std::string data;
  size_t cursor;

  Tokenizer(std::string_view input_data) : data{input_data} {
    cursor = input_data.find_first_not_of(" \t\r\n");

    while (input_data.substr(cursor, 2) == "--") {
      cursor = input_data.find('\n', cursor);
      cursor = input_data.find_first_not_of(" \t\r\n", cursor);
    }
  }

  size_t closing_par() {
    size_t pos   = cursor;
    size_t count = 1;

    while (count != 0) {
      ++pos;
      if (data[pos] == '(') ++count;
      else if (data[pos] == ')') --count;
    }
    return pos;
  }

  std::string_view next_token() {
    if (cursor == std::string::npos) return "";
    size_t start_pos = cursor;
    size_t end_pos;
    switch (data[start_pos]) {
      case '"':
        end_pos = data.find('"', start_pos + 1) + 1;
        cursor  = end_pos;
        break;
      case '\'':
        end_pos = data.find('\'', start_pos + 1) + 1;
        cursor  = end_pos;
        break;
      case '(':
        ++start_pos;
        end_pos = closing_par();
        cursor  = end_pos + 1;
        break;
      case ')':
      case ',':
      case ';':
        end_pos = start_pos + 1;
        cursor  = end_pos;
        break;
      default:
        end_pos = data.find_first_of(" \t\r\n'\"(),;", start_pos);
        cursor  = end_pos;
    }

    if (cursor >= data.length()) {
      cursor = std::string::npos;
      return std::string_view(data).substr(start_pos, end_pos - start_pos);
    }
    cursor = data.find_first_not_of(" \t\r\n", cursor);

    while (std::string_view(data).substr(cursor, 2) == "--") {
      cursor = data.find('\n', cursor);
      cursor = data.find_first_not_of(" \t\r\n", cursor);
      if (cursor == std::string::npos) break;
    }

    return std::string_view(data).substr(start_pos, end_pos - start_pos);
  }

  std::string_view until(std::string_view stop_token) {
    if (cursor == std::string::npos) return "";
    size_t start_pos = cursor;
    size_t end_pos   = cursor;
    while (cursor != std::string::npos && next_token() != stop_token) end_pos = cursor;
    return std::string_view(data).substr(start_pos, end_pos - start_pos);
  }

  bool done() { return cursor == std::string::npos; }
};
}  // namespace asql
