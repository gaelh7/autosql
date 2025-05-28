
#include "autosql/parse/column.hpp"

#include <stdexcept>

#include "autosql/parse/datatype.hpp"
#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

ColumnParse::ColumnParse(Lexer& tokens) {
  name = tokens->str();
  type = DatatypeParse{++tokens};

  parse_constraints(tokens);
}

void ColumnParse::parse_constraints(Lexer& tokens) {
  while (true) {
    std::string con_name;
    if (tokens->type == TokenId::Constraint) {
      con_name = (++tokens)->str();
      ++tokens;
    }
    switch (tokens->type) {
      case TokenId::Not:
        (++tokens).expect(TokenId::Null);
        not_null = true;
        ++tokens;
        continue;
      case TokenId::Unique:
        if (con_name.empty()) con_name = name + "_uq";
        unique = UniqueParse{con_name};
        ++tokens;
        continue;
      case TokenId::As: generated = true; [[fallthrough]];
      case TokenId::Default:
        (++tokens).expect(TokenId::OpenPar);
        expr = ExpressionParse{++tokens};
        tokens.expect(TokenId::ClosePar);
        ++tokens;
        continue;
      case TokenId::References: {
        if (con_name.empty()) con_name = name + "_fk";
        reference = ForeignKeyParse<ColumnParse>{con_name, ++tokens};
        continue;
      }
      case TokenId::Check:
        if (con_name.empty()) con_name = name + "_ck";
        check = CheckParse{con_name, ++tokens};
        continue;
      case TokenId::ClosePar:
      case TokenId::Comma: return;
      default: throw std::runtime_error("Error: Unknown constraint type");
    }
  }
}
}  // namespace parse
}  // namespace asql
