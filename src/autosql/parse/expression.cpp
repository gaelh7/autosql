#include "autosql/parse/expression.h"

#include <stdexcept>
#include <string>

#include "autosql/parse/parser.h"
#include "autosql/parse/token.h"

namespace asql {
namespace parse {

ExpressionParse::ExpressionParse(Tokenizer& tokens) { pratt(tokens, 0); }

std::string ExpressionParse::str() const noexcept {
  std::string out;
  for (const Token& token : tokens_) {
    out += token.str();
    out += ' ';
  }
  return out;
}

void ExpressionParse::parse_func(Tokenizer& tokens) {
  if ((++tokens)->type == TokenType::ClosePar) return;
  while (true) {
    pratt(tokens, 0);
    switch (tokens->type) {
      case TokenType::ClosePar: return;
      case TokenType::Comma: ++tokens; continue;
      default:
        throw std::runtime_error("Error: Unexpected token in function call");
    }
  }
}

void ExpressionParse::pratt(Tokenizer& tokens, unsigned int precedence) {
  switch (tokens->type) {
    case TokenType::OpenPar:
      pratt(++tokens, 0);
      if (tokens->type != TokenType::ClosePar)
        throw std::runtime_error("Error: Unmatched parantheses in expression");
      ++tokens;
      break;
    case TokenType::Plus:
      pratt(++tokens, 4);
      tokens_.emplace_back("", TokenType::UnaryPlus);
      break;
    case TokenType::Minus:
      pratt(++tokens, 4);
      tokens_.emplace_back("", TokenType::UnaryMinus);
      break;
    case TokenType::Identifier: {
      Token id = *tokens;
      if ((++tokens)->type == TokenType::OpenPar) {
        parse_func(tokens);
        tokens_.emplace_back(id.str(), TokenType::Func);
        ++tokens;
      } else tokens_.push_back(id);
      break;
    }
    case TokenType::StringLiteral:
    case TokenType::FloatLiteral:
    case TokenType::IntLiteral:
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
      case TokenType::Equal:
        if (precedence >= 1) return;
        pratt(++tokens, 1);
        break;
      case TokenType::Plus:
      case TokenType::Minus:
        if (precedence >= 2) return;
        pratt(++tokens, 2);
        break;
      case TokenType::Times:
      case TokenType::Div:
        if (precedence >= 3) return;
        pratt(++tokens, 3);
        break;
      case TokenType::Exp:
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
