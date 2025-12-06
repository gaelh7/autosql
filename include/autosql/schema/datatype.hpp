#pragma once

import asql.parse;
import asql.symbols;

namespace asql {

enum class TypeId {
  Struct,
  Enum,
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

class Datatype {
  Identifier name_;
  TypeId id_;
  bool is_array;

public:
  Datatype() = default;

  Datatype(const parse::DatatypeParse& type);
};

}  // namespace asql
