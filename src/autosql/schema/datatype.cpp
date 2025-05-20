#include "autosql/schema/datatype.h"

#include "autosql/parse/datatype.h"

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

}  // namespace asql
