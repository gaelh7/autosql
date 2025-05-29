#include "autosql/parse/expression.hpp"

#include <stdexcept>
#include <string>

#include "autosql/parse/parser.hpp"
#include "autosql/symbols.hpp"

namespace asql::parse {

ExpressionParse::ExpressionParse(Lexer& tokens) { pratt(tokens, 0); }

std::string ExpressionParse::str() const noexcept {
  std::string out;
  for (const Operator& token : tokens_) {
    out += token.str();
    out += ' ';
  }
  return out;
}

unsigned int ExpressionParse::parse_func(Lexer& tokens) {
  if ((++tokens)->type == TokenId::ClosePar) return 0;
  unsigned int num_args = 0;
  while (true) {
    ++num_args;
    pratt(tokens, 0);
    switch (tokens->type) {
      case TokenId::ClosePar: return num_args;
      case TokenId::Comma: ++tokens; continue;
      default:
        throw std::runtime_error("Error: Unexpected token in function call");
    }
  }
}

void ExpressionParse::pratt(Lexer& tokens, unsigned int precedence) {
  switch (tokens->type) {
    case TokenId::OpenPar:
      pratt(++tokens, 0);
      tokens.expect(TokenId::ClosePar);
      ++tokens;
      break;
    case TokenId::Plus:
      pratt(++tokens, 4);
      tokens_.emplace_back(OpId::UnaryPlus);
      break;
    case TokenId::Minus:
      pratt(++tokens, 4);
      tokens_.emplace_back(OpId::UnaryMinus);
      break;
    case TokenId::Identifier: {
      Token id = *tokens;
      if ((++tokens)->type == TokenId::OpenPar) {
        unsigned int num_args = parse_func(tokens);
        tokens_.emplace_back(id.str(), num_args);
        ++tokens;
      } else tokens_.emplace_back(id);
      break;
    }
    case TokenId::StringLiteral:
    case TokenId::FloatLiteral:
    case TokenId::IntLiteral:
      tokens_.emplace_back(*tokens);
      ++tokens;
      break;
    default:
      throw std::runtime_error("Error: Expected operand, but found token '" +
                               std::string{tokens->str()} + "' in expression");
  }
  while (true) {
    Token curr = *tokens;
    switch (curr.type) {
      case TokenId::Equal:
        if (precedence >= 1) return;
        pratt(++tokens, 1);
        break;
      case TokenId::Plus:
      case TokenId::Minus:
        if (precedence >= 2) return;
        pratt(++tokens, 2);
        break;
      case TokenId::Times:
      case TokenId::Div:
        if (precedence >= 3) return;
        pratt(++tokens, 3);
        break;
      case TokenId::Exp:
        if (precedence > 5) return;
        pratt(++tokens, 5);
        break;
      default: return;
    }
    tokens_.emplace_back(curr);
  }
}
}  // namespace asql::parse
