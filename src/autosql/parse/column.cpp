
#include "autosql/parse/column.h"

#include <stdexcept>

#include "autosql/parse/datatype.h"
#include "autosql/parse/token.h"

namespace asql {
namespace parse {

ColumnParse::ColumnParse(Tokenizer& tokens) {
  name = tokens->str();
  type = DatatypeParse{++tokens};

  parse_constraints(tokens);
}

void ColumnParse::parse_constraints(Tokenizer& tokens) {
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
        unique = UniqueParse{con_name};
        ++tokens;
        continue;
      case TokenType::As:
        generated = true;
        [[fallthrough]];
      case TokenType::Default:
        if ((++tokens)->type != TokenType::OpenPar)
          throw std::runtime_error("Error: DEFAULT expression must be within parentheses.");
        expr = ExpressionParse{++tokens};
        if (tokens->type != TokenType::ClosePar)
          throw std::runtime_error("Error: DEFAULT expression must be within parentheses.");
        ++tokens;
        continue;
      case TokenType::References: {
        if (con_name.empty()) con_name = name + "_fk";
        reference = ForeignKeyParse<ColumnParse>{con_name, ++tokens};
        continue;
      }
      case TokenType::Check:
        if (con_name.empty()) con_name = name + "_ck";
        check = CheckParse{con_name, ++tokens};
        continue;
      case TokenType::ClosePar:
      case TokenType::Comma: return;
      default: throw std::runtime_error("Error: Unknown constraint type");
    }
  }
}
}  // namespace parse
}  // namespace asql
