#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace asql {

enum class TokenId {
  Unspecified,
  Eof,
  Identifier,
  OpenPar,
  ClosePar,
  OpenBracket,
  CloseBracket,
  Semicolon,
  Comma,
  Equal,
  Plus,
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
      case TokenId::Plus: return "+";
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

enum class OpId {
  Identifier,
  Function,
  StringLiteral,
  FloatLiteral,
  IntLiteral,
  Equal,
  UnaryPlus,
  Plus,
  UnaryMinus,
  Minus,
  Times,
  Div,
  Exp,
};

class Operator {
  std::string data_;
  OpId id_;
  unsigned int args_;

public:
  Operator(const Token& token) {
    switch (token.type) {
      case TokenId::Identifier:
        data_ = token.str();
        id_   = OpId::Identifier;
        break;
      case TokenId::StringLiteral:
        data_ = token.str();
        id_   = OpId::StringLiteral;
        break;
      case TokenId::FloatLiteral:
        data_ = token.str();
        id_   = OpId::FloatLiteral;
        break;
      case TokenId::IntLiteral:
        data_ = token.str();
        id_   = OpId::IntLiteral;
        break;
      case TokenId::Equal: id_ = OpId::Equal; break;
      case TokenId::Plus: id_ = OpId::Plus; break;
      case TokenId::Minus: id_ = OpId::Minus; break;
      case TokenId::Times: id_ = OpId::Times; break;
      case TokenId::Div: id_ = OpId::Div; break;
      case TokenId::Exp: id_ = OpId::Exp; break;
      default: throw std::runtime_error("Error: Not an operator or operand");
    }
  }

  Operator(OpId id) : id_{id} {}

  Operator(std::string_view func_name, unsigned int num_args)
    : data_{func_name}, id_{OpId::Function}, args_{num_args} {}

  std::string_view str() const noexcept {
    switch (id_) {
      case OpId::Identifier:
      case OpId::Function:
      case OpId::StringLiteral:
      case OpId::FloatLiteral:
      case OpId::IntLiteral: return data_;
      case OpId::Equal: return "=";
      case OpId::UnaryPlus:
      case OpId::Plus: return "+";
      case OpId::UnaryMinus:
      case OpId::Minus: return "-";
      case OpId::Times: return "+";
      case OpId::Div: return "/";
      case OpId::Exp: return "^"; ;
    }
    return "";
  }

  unsigned int args() const noexcept { return args_; }
};

}  // namespace asql
