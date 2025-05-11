#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

namespace asql {

enum class TokenType {
  Unspecified,
  Eof,
  Identifier,
  OpenPar,
  ClosePar,
  Semicolon,
  Comma,
  StringLiteral,
  FloatLiteral,
  IntLiteral,
  Varchar,
  Integer,
  Text,
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
    {"INTEGER",    TokenType::Integer   },
    {"VARCHAR",    TokenType::Varchar   },
    {"TEXT",       TokenType::Text      },
    {"CREATE",     TokenType::Create    },
    {"TABLE",      TokenType::Table     },
    {"TYPE",       TokenType::Type      },
    {"GRANT",      TokenType::Grant     },
    {"CONSTRAINT", TokenType::Constraint},
    {"CHECK",      TokenType::Check     },
    {"REFERENCES", TokenType::References},
    {"DEFAULT",    TokenType::Default   },
    {"UNIQUE",     TokenType::Unique    },
    {"NOT",        TokenType::Not       },
    {"NULL",       TokenType::Null      },
    {"AS",         TokenType::As        },
    {"STORED",     TokenType::Stored    },
    {"PRIMARY",    TokenType::Primary   },
    {"FOREIGN",    TokenType::Foreign   },
    {"KEY",        TokenType::Key       },
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
      case TokenType::Semicolon: return ";";
      case TokenType::Comma: return ",";
      case TokenType::Varchar: return "VARCHAR";
      case TokenType::Integer: return "INTEGER";
      case TokenType::Text: return "TEXT";
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
    return ""; // Unreachable
  }
};
}  // namespace asql
