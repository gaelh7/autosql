module;

#include <format>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <filesystem>

module asql.parse;

import asql.symbols;

namespace asql::parse {

static const std::unordered_map<std::string_view, TokenId> keyword_map = {
    {"AS",         TokenId::As        },
    {"CHECK",      TokenId::Check     },
    {"CONSTRAINT", TokenId::Constraint},
    {"CREATE",     TokenId::Create    },
    {"DEFAULT",    TokenId::Default   },
    {"DOUBLE",     TokenId::Double    },
    {"FOREIGN",    TokenId::Foreign   },
    {"FLOAT",      TokenId::Float     },
    {"GRANT",      TokenId::Grant     },
    {"INT32",      TokenId::Int32     },
    {"INT64",      TokenId::Int64     },
    {"KEY",        TokenId::Key       },
    {"NOT",        TokenId::Not       },
    {"NULL",       TokenId::Null      },
    {"PRIMARY",    TokenId::Primary   },
    {"REFERENCES", TokenId::References},
    {"STORED",     TokenId::Stored    },
    {"STRING",     TokenId::String    },
    {"TABLE",      TokenId::Table     },
    {"TYPE",       TokenId::Type      },
    {"UNIQUE",     TokenId::Unique    },
};

Lexer::Lexer(std::filesystem::path filename)
  : file_{filename}, line_{}, column_{} {
  if (!file_.is_open()) throw std::runtime_error("Error: couldn't open file");
  if (std::getline(file_, data_)) read_token();
  else curr_ = Token{"", TokenId::Eof};
}

void Lexer::expect(TokenId id) {
  std::string s;
  switch (id) {
    case TokenId::Identifier: s = "Identifier"; break;
    case TokenId::StringLiteral: s = "String Literal"; break;
    case TokenId::FloatLiteral: s = "Float Literal"; break;
    case TokenId::IntLiteral: s = "Integer Literal"; break;
    default: s = Token{"", id}.str(); break;
  }
  if (curr_.type != id)
    throw std::runtime_error(std::format("Line {}: Expected Token {}",
                                         line_ + 1, s));
}

Lexer& Lexer::operator++() {
  if (curr_.type == TokenId::Eof)
    throw std::runtime_error("Error: Unexpected end-of-file");
  read_token();
  return *this;
}

bool Lexer::seek_next() {
  column_ = data_.find_first_not_of(" \t\v\r\f", column_);
  while (true) {
    if (column_ == std::string::npos ||
        std::string_view{data_}.substr(column_, 2) == "--") {
      if (!std::getline(file_, data_)) return false;
      ++line_;
      column_ = data_.find_first_not_of(" \t\v\r\f");
    } else if (std::string_view{data_}.substr(column_, 2) == "/*") {
      for (column_ = data_.find("*/", column_); column_ == std::string::npos;
           column_ = data_.find("*/")) {
        if (!std::getline(file_, data_))
          throw std::runtime_error("Error: Unterminated multiline comment");
        ++line_;
      }
      column_ = data_.find_first_not_of(" \t\v\r\f", column_ + 2);
    } else return true;
  }
}

void Lexer::parse_string() {
  size_t end_pos = data_.find(data_[column_], column_ + 1);
  if (end_pos == std::string::npos)
    throw std::runtime_error("Unterminated string literal");
  curr_ =
      Token{std::string_view{data_}.substr(column_ + 1, end_pos - column_ - 1),
            TokenId::StringLiteral};
  column_ = end_pos + 1;
}

void Lexer::parse_num() noexcept {
  size_t end = data_.find_first_not_of("0123456789", column_ + 1);
  if (end == std::string::npos) {
    curr_ = Token{std::string_view{data_}.substr(column_), TokenId::IntLiteral};
    column_ = std::string::npos;
    return;
  }
  switch (data_[end]) {
    case '.':
      end   = data_.find_first_not_of("0123456789", end + 1);
      curr_ = Token{std::string_view{data_}.substr(column_, end - column_),
                    TokenId::FloatLiteral};
      break;
    default:
      curr_ = Token{std::string_view{data_}.substr(column_, end - column_),
                    TokenId::IntLiteral};
      break;
  }
  column_ = end;
}

void Lexer::parse_float() noexcept {
  size_t end = data_.find_first_not_of("0123456789", column_ + 1);
  if (end == std::string::npos) {
    curr_ =
        Token{std::string_view{data_}.substr(column_), TokenId::FloatLiteral};
    column_ = std::string::npos;
    return;
  }
  curr_   = Token{std::string_view{data_}.substr(column_, end - column_),
                TokenId::FloatLiteral};
  column_ = end;
}

void Lexer::read_token() {
  if (!seek_next()) {
    curr_ = Token{"", TokenId::Eof};
    return;
  }
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
      curr_ = Token{"", TokenId::OpenPar};
      ++column_;
      break;
    case ')':
      curr_ = Token{"", TokenId::ClosePar};
      ++column_;
      break;
    case '[':
      curr_ = Token{"", TokenId::OpenBracket};
      ++column_;
      break;
    case ']':
      curr_ = Token{"", TokenId::CloseBracket};
      ++column_;
      break;
    case ',':
      curr_ = Token{"", TokenId::Comma};
      ++column_;
      break;
    case ';':
      curr_ = Token{"", TokenId::Semicolon};
      ++column_;
      break;
    case '=':
      curr_ = Token{"", TokenId::Equal};
      ++column_;
      break;
    case '+':
      curr_ = Token{"", TokenId::Plus};
      ++column_;
      break;
    case '-':
      curr_ = Token{"", TokenId::Minus};
      ++column_;
      break;
    case '*':
      curr_ = Token{"", TokenId::Times};
      ++column_;
      break;
    case '/':
      curr_ = Token{"", TokenId::Div};
      ++column_;
      break;
    case '^':
      curr_ = Token{"", TokenId::Exp};
      ++column_;
      break;
    default: {
      size_t end_pos =
          data_.find_first_of(" \t\v\r\f'\"+-*/^&%!.()[],;", column_);
      std::string_view raw =
          std::string_view{data_}.substr(column_, end_pos - column_);
      auto it = keyword_map.find(raw);
      if (it != keyword_map.end()) curr_ = Token{"", it->second};
      else curr_ = Token{raw, TokenId::Identifier};
      column_ = end_pos;
    }
  }
}

}  // namespace asql::parse
