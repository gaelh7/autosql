#pragma once

#include <cstddef>
#include <string_view>

namespace asql {
inline size_t closing_par(std::string_view str, size_t pos) {
  size_t count = 1;

  while (count != 0) {
    ++pos;
    if (str[pos] == '(') ++count;
    else if (str[pos] == ')') --count;
  }
  return pos;
}
}  // namespace asql
