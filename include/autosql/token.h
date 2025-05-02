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

inline std::unordered_map<std::string_view, TokenType> keyword_map = {
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

class Token {
public:
  std::string raw_;
  TokenType type_;

  Token() = default;

  Token(std::string_view in) : raw_{in} {
    switch (in.front()) {
      case '"':
      case '\'':
        if (in.front() == in.back()) {
          raw_  = in.substr(1, in.length() - 2);
          type_ = TokenType::STRING_LITERAL_T;
        }
        return;
      case '(':
        if (in.length() == 1) {
          raw_  = in;
          type_ = TokenType::OPEN_PAR_T;
        }
        return;
      case ')':
        if (in.length() == 1) {
          raw_  = in;
          type_ = TokenType::CLOSE_PAR_T;
        }
        return;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        size_t end = in.find_first_not_of("0123456789", 1);
        if (end == std::string_view::npos) type_ = TokenType::INT_LITERAL_T;
        else if (in[end] == '.') type_ = TokenType::FLOAT_LITERAL_T;
        raw_ = in;
        return;
      }
      case ';': type_ = TokenType::SEMICOLON_T; return;
    }
    auto it = keyword_map.find(in);
    if (it != keyword_map.end()) {
      type_ = it->second;
      return;
    }
    type_ = TokenType::IDENTIFIER_T;
    raw_  = in;
  }

  Token(std::string_view in, TokenType type) : raw_{in}, type_{type} {
    if (type == TokenType::UNSPECIFIED_T) {
      auto it = keyword_map.find(in);
      if (it != keyword_map.end()) {
        type_ = it->second;
        return;
      }
      type_ = TokenType::IDENTIFIER_T;
      raw_  = in;
    }
  }
};
}  // namespace asql
