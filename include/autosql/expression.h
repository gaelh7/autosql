#pragma once

#include <string>

#include "autosql/sqlparse.h"
#include "autosql/token.h"

namespace asql {
class Expression {
public:
  std::string raw_;

  Expression() = default;

  Expression(Tokenizer& parser) {
    if (parser.next_token().type_ != TokenType::OPEN_PAR_T)
      return;  // TODO error

    size_t par_count = 1;

    while (true) {
      Token t = parser.next_token();
      switch (t.type_) {
        case TokenType::OPEN_PAR_T: ++par_count; break;
        case TokenType::CLOSE_PAR_T: --par_count; break;
        default: break;
      }
      if (par_count == 0) break;
      raw_ += t.raw_;
      raw_ += ' ';
    }
  }
};
}  // namespace asql
