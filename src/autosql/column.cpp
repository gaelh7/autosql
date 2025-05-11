
#include "autosql/column.h"

#include <stdexcept>

#include "autosql/token.h"

namespace asql {

Column::Column(Tokenizer& tokens) {
  name = tokens->str();

  switch ((++tokens)->type) {
    case TokenType::Varchar: type = "VARCHAR"; break;
    case TokenType::Integer: type = "INTEGER"; break;
    case TokenType::Text: type = "TEXT"; break;
    default: throw std::runtime_error("Error: Not a type");
  }

  parse_constraints(++tokens);
}

void Column::parse_constraints(Tokenizer& tokens) {
  while (!tokens.done()) {
    std::string con_name;
    if (tokens->type == TokenType::Constraint) {
      con_name = (++tokens)->str();
      ++tokens;
    }
    switch (tokens->type) {
      case TokenType::Not:
        if ((++tokens)->type == TokenType::Null) {
          not_null = true;
          ++tokens;
          continue;
        }
        throw std::runtime_error(
            "Error: Expected symbol 'NULL' following 'NOT'");
      case TokenType::Unique:
        if (con_name.empty()) con_name = name + "_uq";
        unique = Unique{con_name};
        ++tokens;
        continue;
      case TokenType::Default: expr = Expression{++tokens}; continue;
      case TokenType::As:
        expr      = Expression{++tokens};
        generated = true;
        continue;
      case TokenType::References: {
        if (con_name.empty()) con_name = name + "_fk";
        reference = ForeignKey<Column>{con_name, ++tokens};
        continue;
      }
      case TokenType::Check:
        if (con_name.empty()) con_name = name + "_ck";
        check = Check{con_name, ++tokens};
        continue;
      case TokenType::ClosePar:
      case TokenType::Comma: return;
      default: throw std::runtime_error("Error: Unknown constraint type");
    }
  }
}
}  // namespace asql
