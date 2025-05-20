#pragma once

#include <string>
#include <string_view>

#include "autosql/parse/parser.h"

namespace asql {
namespace parse {

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
  std::string name_;

public:
  RawType id_;
  bool is_array;

  DatatypeParse() = default;

  DatatypeParse(Tokenizer& tokens);

  std::string_view str() const noexcept;
};
}  // namespace parse
}  // namespace asql
