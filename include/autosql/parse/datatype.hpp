#pragma once

#include <string_view>

#include "autosql/parse/parser.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

enum class RawType {
  User,
  Bool,
  Bytes,
  Int32,
  Int64,
  Float,
  Double,
  Numeric,
  String,
  Date,
  Time,
  Datetime,
  Timestamp,
  Interval,
  Json,
};

class DatatypeParse {
  Identifier name_;

public:
  RawType id_;
  bool is_array;

  DatatypeParse() = default;

  DatatypeParse(Lexer& tokens);

  std::string_view str() const noexcept;
};
}  // namespace asql::parse
