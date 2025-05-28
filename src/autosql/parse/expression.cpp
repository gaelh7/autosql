#include "autosql/parse/expression.hpp"

#include <stdexcept>
#include <string>

#include "autosql/parse/parser.hpp"
#include "autosql/parse/token.hpp"

namespace asql {
namespace parse {

ExpressionParse::ExpressionParse(Lexer& tokens) { pratt(tokens, 0); }

std::string ExpressionParse::str() const noexcept {
  std::string out;
  for (const Token& token : tokens_) {
    out += token.str();
    out += ' ';
  }
  return out;
}

void ExpressionParse::parse_func(Lexer& tokens) {
  tokens_.emplace_back("", TokenId::Semicolon);
  if ((++tokens)->type == TokenId::ClosePar) return;
  while (true) {
    pratt(tokens, 0);
    switch (tokens->type) {
      case TokenId::ClosePar: return;
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
      tokens_.emplace_back("", TokenId::UnaryPlus);
      break;
    case TokenId::Minus:
      pratt(++tokens, 4);
      tokens_.emplace_back("", TokenId::UnaryMinus);
      break;
    case TokenId::Identifier: {
      Token id = *tokens;
      if ((++tokens)->type == TokenId::OpenPar) {
        parse_func(tokens);
        tokens_.emplace_back(id.str(), TokenId::Func);
        ++tokens;
      } else tokens_.push_back(id);
      break;
    }
    case TokenId::StringLiteral:
    case TokenId::FloatLiteral:
    case TokenId::IntLiteral:
      tokens_.push_back(*tokens);
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
    tokens_.push_back(curr);
  }
}
}  // namespace parse
}  // namespace asql
