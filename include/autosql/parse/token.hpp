#pragma once

#include <string>
#include <string_view>

namespace asql::parse {

enum class TokenId {
  Unspecified,
  Eof,
  Identifier,
  Func,
  OpenPar,
  ClosePar,
  OpenBracket,
  CloseBracket,
  Semicolon,
  Comma,
  Equal,
  UnaryPlus,
  Plus,
  UnaryMinus,
  Minus,
  Times,
  Div,
  Exp,
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
  std::string data_;

public:
  TokenId type;

  constexpr Token() = default;

  constexpr Token(std::string_view in, TokenId type) noexcept
    : data_{in}, type{type} {}

  constexpr std::string_view str() const noexcept {
    switch (type) {
      case TokenId::Identifier:
      case TokenId::Func:
      case TokenId::StringLiteral:
      case TokenId::FloatLiteral:
      case TokenId::IntLiteral: return data_;
      case TokenId::Unspecified:
      case TokenId::Eof: return "";
      case TokenId::OpenPar: return "(";
      case TokenId::ClosePar: return ")";
      case TokenId::OpenBracket: return "[";
      case TokenId::CloseBracket: return "]";
      case TokenId::Semicolon: return ";";
      case TokenId::Comma: return ",";
      case TokenId::Equal: return "=";
      case TokenId::UnaryPlus:
      case TokenId::Plus: return "+";
      case TokenId::UnaryMinus:
      case TokenId::Minus: return "-";
      case TokenId::Times: return "*";
      case TokenId::Div: return "/";
      case TokenId::Exp: return "^";
      case TokenId::String: return "STRING";
      case TokenId::Int32: return "INT32";
      case TokenId::Int64: return "INT64";
      case TokenId::Float: return "FLOAT";
      case TokenId::Double: return "DOUBLE";
      case TokenId::Create: return "CREATE";
      case TokenId::Table: return "TABLE";
      case TokenId::Type: return "TYPE";
      case TokenId::Grant: return "GRANT";
      case TokenId::Constraint: return "CONSTRAINT";
      case TokenId::Check: return "CHECK";
      case TokenId::References: return "REFERENCES";
      case TokenId::Default: return "DEFAULT";
      case TokenId::Unique: return "UNIQUE";
      case TokenId::Not: return "NOT";
      case TokenId::Null: return "NULL";
      case TokenId::As: return "AS";
      case TokenId::Stored: return "STORED";
      case TokenId::Primary: return "PRIMARY";
      case TokenId::Foreign: return "FOREIGN";
      case TokenId::Key: return "KEY";
    }
    return "";  // Unreachable
  }
};
}  // namespace asql::parse
