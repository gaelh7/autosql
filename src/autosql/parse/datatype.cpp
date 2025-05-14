#include "autosql/parse/datatype.h"

#include <stdexcept>
#include <string_view>

#include "autosql/parse/parser.h"
#include "autosql/parse/token.h"

namespace asql {
namespace parse {

DatetypeParse::DatetypeParse(Tokenizer& tokens) : is_array{false} {
  switch (tokens->type) {
    case TokenType::Identifier:
      id_   = RawType::User;
      name_ = tokens->str();
      break;
    case TokenType::String: id_ = RawType::String; break;
    case TokenType::Int32: id_ = RawType::Int32; break;
    case TokenType::Int64: id_ = RawType::Int64; break;
    case TokenType::Float: id_ = RawType::Float; break;
    case TokenType::Double: id_ = RawType::Double; break;
    default: throw std::runtime_error("Error: Not a type");
  }
  if ((++tokens)->type == TokenType::OpenBracket) {
    if ((++tokens)->type != TokenType::CloseBracket)
      throw std::runtime_error("Error: Expected ']'");
    is_array = true;
    ++tokens;
  }
}

std::string_view DatetypeParse::str() const noexcept {
  switch (id_) {
    case RawType::User: return name_;
    case RawType::Bool: return "BOOL";
    case RawType::Bytes: return "BYTES";
    case RawType::Int32: return "INT32";
    case RawType::Int64: return "INT64";
    case RawType::Float: return "FLOAT";
    case RawType::Double: return "DOUBLE";
    case RawType::Numeric: return "NUMERIC";
    case RawType::String: return "STRING";
    case RawType::Date: return "DATE";
    case RawType::Time: return "TIME";
    case RawType::Datetime: return "DATETIME";
    case RawType::Timestamp: return "TIMESTAMP";
    case RawType::Interval: return "INTERVAL";
    case RawType::Json: return "JSON";
  }
  return ""; // Unreachable
}
}  // namespace parse

}  // namespace asql
