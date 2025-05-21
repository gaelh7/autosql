#pragma once

#include "autosql/parse/expression.h"

namespace asql {

class Expression {
public:
  Expression(const parse::ExpressionParse expr);
};

}  // namespace asql
