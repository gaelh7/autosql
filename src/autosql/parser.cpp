#include "autosql/parser.h"

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include "autosql/token.h"

namespace asql {
namespace parse {

static const std::unordered_map<std::string_view, TokenType> keyword_map = {
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

Tokenizer& Tokenizer::operator++() {
  if (curr_.type == TokenType::Eof)
    throw std::runtime_error("Error: Unexpected end-of-file");
  read_token();
  return *this;
}

void Tokenizer::skip_whitespace() {
  column_ = data_.find_first_not_of(" \t\v\r\f", column_);
  while (true) {
    if (column_ == std::string::npos ||
        std::string_view{data_}.substr(column_, 2) == "--") {
      if (file_.eof()) {
        curr_ = Token{"", TokenType::Eof};
        break;
      } else {
        std::getline(file_, data_);
        ++line_;
        column_ = data_.find_first_not_of(" \t\v\r\f");
      }
    } else if (std::string_view{data_}.substr(column_, 2) == "/*") {
      for (column_ = data_.find("*/", column_); column_ == std::string::npos;
           column_ = data_.find("*/")) {
        if (file_.eof())
          throw std::runtime_error("Error: Unterminated multiline comment");
        std::getline(file_, data_);
        ++line_;
      }
      column_ = data_.find_first_not_of(" \t\v\r\f", column_ + 2);
    } else break;
  }
}

void Tokenizer::parse_string() {
  size_t end_pos = data_.find(data_[column_], column_ + 1);
  if (end_pos == std::string::npos)
    throw std::runtime_error("Unterminated string literal");
  curr_ =
      Token{std::string_view{data_}.substr(column_ + 1, end_pos - column_ - 1),
            TokenType::StringLiteral};
  column_ = end_pos + 1;
}

void Tokenizer::parse_num() noexcept {
  size_t end = data_.find_first_not_of("0123456789", column_ + 1);
  if (end == std::string::npos) {
    curr_ =
        Token{std::string_view{data_}.substr(column_), TokenType::IntLiteral};
    column_ = std::string::npos;
    return;
  }
  switch (data_[end]) {
    case '.':
      end   = data_.find_first_not_of("0123456789", end + 1);
      curr_ = Token{std::string_view{data_}.substr(column_, end - column_),
                    TokenType::FloatLiteral};
      break;
    default:
      curr_ = Token{std::string_view{data_}.substr(column_, end - column_),
                    TokenType::IntLiteral};
      break;
  }
  column_ = end;
}

void Tokenizer::parse_float() noexcept {
  size_t end = data_.find_first_not_of("0123456789", column_ + 1);
  if (end == std::string::npos) {
    curr_ =
        Token{std::string_view{data_}.substr(column_), TokenType::FloatLiteral};
    column_ = std::string::npos;
    return;
  }
  curr_   = Token{std::string_view{data_}.substr(column_, end - column_),
                TokenType::FloatLiteral};
  column_ = end;
}

void Tokenizer::read_token() {
  skip_whitespace();
  if (curr_.type == TokenType::Eof) return;
  switch (data_[column_]) {
    case '"':
    case '\'': parse_string(); break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': parse_num(); break;
    case '.': parse_float(); break;
    case '(':
      curr_ = Token{"", TokenType::OpenPar};
      ++column_;
      break;
    case ')':
      curr_ = Token{"", TokenType::ClosePar};
      ++column_;
      break;
    case '[':
      curr_ = Token{"", TokenType::OpenBracket};
      ++column_;
      break;
    case ']':
      curr_ = Token{"", TokenType::CloseBracket};
      ++column_;
      break;
    case ',':
      curr_ = Token{"", TokenType::Comma};
      ++column_;
      break;
    case ';':
      curr_ = Token{"", TokenType::Semicolon};
      ++column_;
      break;
    default: {
      size_t end_pos = data_.find_first_of(" \t\v\r\f'\".()[],;", column_);
      std::string_view raw =
          std::string_view{data_}.substr(column_, end_pos - column_);
      auto it = keyword_map.find(raw);
      if (it != keyword_map.end()) curr_ = Token{"", it->second};
      else curr_ = Token{raw, TokenType::Identifier};
      column_ = end_pos;
    }
  }
}

}  // namespace parse
}  // namespace asql
