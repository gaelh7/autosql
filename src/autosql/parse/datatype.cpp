#include "autosql/parse/datatype.hpp"

#include <stdexcept>
#include <string_view>

#include "autosql/parse/parser.hpp"
#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

DatatypeParse::DatatypeParse(Lexer& tokens) : is_array{false} {
  switch (tokens->type) {
    case TokenId::Identifier:
      id_   = RawType::User;
      name_ = tokens->str();
      break;
    case TokenId::String: id_ = RawType::String; break;
    case TokenId::Int32: id_ = RawType::Int32; break;
    case TokenId::Int64: id_ = RawType::Int64; break;
    case TokenId::Float: id_ = RawType::Float; break;
    case TokenId::Double: id_ = RawType::Double; break;
    default: throw std::runtime_error("Error: Not a type");
  }
  if ((++tokens)->type == TokenId::OpenBracket) {
    if ((++tokens)->type != TokenId::CloseBracket)
      throw std::runtime_error("Error: Expected ']'");
    is_array = true;
    ++tokens;
  }
}

std::string_view DatatypeParse::str() const noexcept {
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
