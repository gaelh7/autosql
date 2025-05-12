#pragma once

#include <string_view>

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

class Token {
  std::string_view data_;

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
