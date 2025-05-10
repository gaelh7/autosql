
#include "autosql/column.h"

#include <stdexcept>

#include "autosql/token.h"

namespace asql {

Column::Column(Tokenizer& tokens) {
  name = tokens->str();

  switch ((++tokens)->type) {
    case TokenType::VARCHAR_T: type = "VARCHAR"; break;
    case TokenType::INTEGER_T: type = "INTEGER"; break;
    case TokenType::TEXT_T: type = "TEXT"; break;
    default: throw std::runtime_error("Error: Not a type");
  }

  parse_constraints(++tokens);
}

void Column::parse_constraints(Tokenizer& tokens) {
  while (!tokens.done()) {
    std::string con_name;
    if (tokens->type == TokenType::CONSTRAINT_T) {
      con_name = (++tokens)->str();
      ++tokens;
    }
    switch (tokens->type) {
      case TokenType::NOT_T:
        if ((++tokens)->type == TokenType::NULL_T) {
          not_null = true;
          ++tokens;
          continue;
        }
        throw std::runtime_error(
            "Error: Expected symbol 'NULL' following 'NOT'");
      case TokenType::UNIQUE_T:
        if (con_name.empty()) con_name = name + "_uq";
        unique = Unique{con_name};
        ++tokens;
        continue;
      case TokenType::DEFAULT_T: expr = Expression{++tokens}; continue;
      case TokenType::AS_T:
        expr      = Expression{++tokens};
        generated = true;
        continue;
      case TokenType::REFERENCES_T: {
        if (con_name.empty()) con_name = name + "_fk";
        reference = ForeignKey<Column>{con_name, ++tokens};
        continue;
      }
      case TokenType::CHECK_T:
        if (con_name.empty()) con_name = name + "_ck";
        check = Check{con_name, ++tokens};
        continue;
      case TokenType::CLOSE_PAR_T:
      case TokenType::COMMA_T: return;
      default: throw std::runtime_error("Error: Unknown constraint type");
    }
  }
}
}  // namespace asql
