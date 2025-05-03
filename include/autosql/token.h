#pragma once

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
    {"KEY",        TokenType::KEY_T       },
};

struct Token {
  std::string data;
  TokenType type;

  Token() = default;

  Token(std::string_view in, TokenType type) : data{in}, type{type} {}
};
}  // namespace asql
