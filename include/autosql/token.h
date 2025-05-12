#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

namespace asql {
namespace parse {

enum class TokenType {
  Unspecified,
  Eof,
  Identifier,
  OpenPar,
  ClosePar,
  OpenBracket,
  CloseBracket,
  Semicolon,
  Comma,
  StringLiteral,
  FloatLiteral,
  IntLiteral,
  String,
  Int32,
  Int64,
  Float,
  Double,
  Create,
  Table,
  Type,
  Grant,
  Constraint,
  Check,
  References,
  Default,
  Unique,
  Not,
  Null,
  As,
  Stored,
  Primary,
  Foreign,
  Key,
};

inline const std::unordered_map<std::string_view, TokenType> keyword_map = {
    {"AS",         TokenType::As        },
    {"CHECK",      TokenType::Check     },
    {"CONSTRAINT", TokenType::Constraint},
    {"CREATE",     TokenType::Create    },
    {"DEFAULT",    TokenType::Default   },
    {"DOUBLE",     TokenType::Double    },
    {"FOREIGN",    TokenType::Foreign   },
    {"FLOAT",      TokenType::Float     },
    {"GRANT",      TokenType::Grant     },
    {"INT32",      TokenType::Int32     },
    {"INT64",      TokenType::Int64     },
    {"KEY",        TokenType::Key       },
    {"NOT",        TokenType::Not       },
    {"NULL",       TokenType::Null      },
    {"PRIMARY",    TokenType::Primary   },
    {"REFERENCES", TokenType::References},
    {"STORED",     TokenType::Stored    },
    {"STRING",     TokenType::String    },
    {"TABLE",      TokenType::Table     },
    {"TYPE",       TokenType::Type      },
    {"UNIQUE",     TokenType::Unique    },
};

class Token {
  std::string data_;

public:
  TokenType type;

  Token() = default;

  Token(std::string_view in, TokenType type) noexcept : data_{in}, type{type} {}

  std::string_view str() const noexcept {
    switch (type) {
      case TokenType::Identifier:
      case TokenType::StringLiteral:
      case TokenType::FloatLiteral:
      case TokenType::IntLiteral: return data_;
      case TokenType::Unspecified:
      case TokenType::Eof: return "";
      case TokenType::OpenPar: return "(";
      case TokenType::ClosePar: return ")";
      case TokenType::OpenBracket: return "[";
      case TokenType::CloseBracket: return "]";
      case TokenType::Semicolon: return ";";
      case TokenType::Comma: return ",";
      case TokenType::String: return "STRING";
      case TokenType::Int32: return "INT32";
      case TokenType::Int64: return "INT64";
      case TokenType::Float: return "FLOAT";
      case TokenType::Double: return "DOUBLE";
      case TokenType::Create: return "CREATE";
      case TokenType::Table: return "TABLE";
      case TokenType::Type: return "TYPE";
      case TokenType::Grant: return "GRANT";
      case TokenType::Constraint: return "CONSTRAINT";
      case TokenType::Check: return "CHECK";
      case TokenType::References: return "REFERENCES";
      case TokenType::Default: return "DEFAULT";
      case TokenType::Unique: return "UNIQUE";
      case TokenType::Not: return "NOT";
      case TokenType::Null: return "NULL";
      case TokenType::As: return "AS";
      case TokenType::Stored: return "STORED";
      case TokenType::Primary: return "PRIMARY";
      case TokenType::Foreign: return "FOREIGN";
      case TokenType::Key: return "KEY";
    }
    return "";  // Unreachable
  }
};
}  // namespace parse
}  // namespace asql
