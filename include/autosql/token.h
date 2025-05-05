#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

namespace asql {

enum class TokenType {
  UNSPECIFIED_T,
  EOF_T,
  IDENTIFIER_T,
  OPEN_PAR_T,
  CLOSE_PAR_T,
  SEMICOLON_T,
  COMMA_T,
  STRING_LITERAL_T,
  FLOAT_LITERAL_T,
  INT_LITERAL_T,
  VARCHAR_T,
  INTEGER_T,
  TEXT_T,
  CREATE_T,
  TABLE_T,
  TYPE_T,
  GRANT_T,
  CONSTRAINT_T,
  CHECK_T,
  REFERENCES_T,
  DEFAULT_T,
  UNIQUE_T,
  NOT_T,
  NULL_T,
  AS_T,
  STORED_T,
  PRIMARY_T,
  FOREIGN_T,
  KEY_T,
};

inline const std::unordered_map<std::string_view, TokenType> keyword_map = {
    {"INTEGER",    TokenType::INTEGER_T   },
    {"VARCHAR",    TokenType::VARCHAR_T   },
    {"TEXT",       TokenType::TEXT_T      },
    {"CREATE",     TokenType::CREATE_T    },
    {"TABLE",      TokenType::TABLE_T     },
    {"TYPE",       TokenType::TYPE_T      },
    {"GRANT",      TokenType::GRANT_T     },
    {"CONSTRAINT", TokenType::CONSTRAINT_T},
    {"CHECK",      TokenType::CHECK_T     },
    {"REFERENCES", TokenType::REFERENCES_T},
    {"DEFAULT",    TokenType::DEFAULT_T   },
    {"UNIQUE",     TokenType::UNIQUE_T    },
    {"NOT",        TokenType::NOT_T       },
    {"NULL",       TokenType::NULL_T      },
    {"AS",         TokenType::AS_T        },
    {"STORED",     TokenType::STORED_T    },
    {"PRIMARY",    TokenType::PRIMARY_T   },
    {"FOREIGN",    TokenType::FOREIGN_T   },
    {"KEY",        TokenType::KEY_T       },
};

class Token {
  std::string data_;

public:
  TokenType type;

  Token() = default;

  Token(std::string_view in, TokenType type) noexcept : data_{in}, type{type} {}

  std::string_view str() const noexcept {
    switch (type) {
      case TokenType::IDENTIFIER_T:
      case TokenType::STRING_LITERAL_T:
      case TokenType::FLOAT_LITERAL_T:
      case TokenType::INT_LITERAL_T: return data_;
      case TokenType::UNSPECIFIED_T:
      case TokenType::EOF_T: return "";
      case TokenType::OPEN_PAR_T: return "(";
      case TokenType::CLOSE_PAR_T: return ")";
      case TokenType::SEMICOLON_T: return ";";
      case TokenType::COMMA_T: return ",";
      case TokenType::VARCHAR_T: return "VARCHAR";
      case TokenType::INTEGER_T: return "INTEGER";
      case TokenType::TEXT_T: return "TEXT";
      case TokenType::CREATE_T: return "CREATE";
      case TokenType::TABLE_T: return "TABLE";
      case TokenType::TYPE_T: return "TYPE";
      case TokenType::GRANT_T: return "GRANT";
      case TokenType::CONSTRAINT_T: return "CONSTRAINT";
      case TokenType::CHECK_T: return "CHECK";
      case TokenType::REFERENCES_T: return "REFERENCES";
      case TokenType::DEFAULT_T: return "DEFAULT";
      case TokenType::UNIQUE_T: return "UNIQUE";
      case TokenType::NOT_T: return "NOT";
      case TokenType::NULL_T: return "NULL";
      case TokenType::AS_T: return "AS";
      case TokenType::STORED_T: return "STORED";
      case TokenType::PRIMARY_T: return "PRIMARY";
      case TokenType::FOREIGN_T: return "FOREIGN";
      case TokenType::KEY_T: return "KEY";
    }
    return ""; // Unreachable
  }
};
}  // namespace asql
