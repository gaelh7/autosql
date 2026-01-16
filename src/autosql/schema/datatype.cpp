module;

#include <string_view>

module asql.schema;

import asql.parse;

namespace asql {

Datatype::Datatype(const parse::DatatypeParse& type)
  : name_{type.str()}, is_array{type.is_array} {
  switch (type.id_) {
    case parse::RawType::User: id_ = TypeId::Struct; break;
    case parse::RawType::Bool: id_ = TypeId::Bool; break;
    case parse::RawType::Bytes: id_ = TypeId::Bytes; break;
    case parse::RawType::Int32: id_ = TypeId::Int32; break;
    case parse::RawType::Int64: id_ = TypeId::Int64; break;
    case parse::RawType::Float: id_ = TypeId::Float; break;
    case parse::RawType::Double: id_ = TypeId::Double; break;
    case parse::RawType::Numeric: id_ = TypeId::Numeric; break;
    case parse::RawType::String: id_ = TypeId::String; break;
    case parse::RawType::Date: id_ = TypeId::Date; break;
    case parse::RawType::Time: id_ = TypeId::Time; break;
    case parse::RawType::Datetime: id_ = TypeId::Datetime; break;
    case parse::RawType::Timestamp: id_ = TypeId::Timestamp; break;
    case parse::RawType::Interval: id_ = TypeId::Interval; break;
    case parse::RawType::Json: id_ = TypeId::Json; break;
  }
}

std::string_view Datatype::name() const {
  switch (id_) {
    case TypeId::Struct:
    case TypeId::Enum: return name_;
    case TypeId::Bool: return "BOOL";
    case TypeId::Bytes: return "BYTES";
    case TypeId::Int32: return "INT32";
    case TypeId::Int64: return "INT64";
    case TypeId::Float: return "FLOAT";
    case TypeId::Double: return "DOUBLE";
    case TypeId::Numeric: return "NUMERIC";
    case TypeId::String: return "STRING";
    case TypeId::Date: return "DATE";
    case TypeId::Time: return "TIME";
    case TypeId::Datetime: return "DATETIME";
    case TypeId::Timestamp: return "TIMESTAMP";
    case TypeId::Interval: return "INTERVAL";
    case TypeId::Json: return "JSON";
  }
  return "";  // Unreachable
}

bool Datatype::operator==(const Datatype& other) const {
  if (is_array != other.is_array) return false;
  switch (id_) {
    case TypeId::Struct:
    case TypeId::Enum:
      return name_ == other.name_;
    case TypeId::Bool:
    case TypeId::Bytes:
    case TypeId::Int32:
    case TypeId::Int64:
    case TypeId::Float:
    case TypeId::Double:
    case TypeId::Numeric:
    case TypeId::String:
    case TypeId::Date:
    case TypeId::Time:
    case TypeId::Datetime:
    case TypeId::Timestamp:
    case TypeId::Interval:
    case TypeId::Json: return id_ == other.id_;
  }
}

}  // namespace asql
