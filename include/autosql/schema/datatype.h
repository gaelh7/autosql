#pragma once

#include <string>

#include "autosql/parse/datatype.h"

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
  std::string name_;
  TypeId id_;
  bool is_array;

public:
  Datatype(const parse::DatatypeParse& type);
};

}  // namespace asql
