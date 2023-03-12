#ifndef CBC_PARSERS_H
#define CBC_PARSERS_H

#include "Driver.h"
#include "Tokenizers.h"
#include "common/Metadata.h"
#include "parser/IMatcher.h"
#include "parser/IParser.h"
#include "parser/Matchers.h"

#include <iostream>

namespace cb {

using namespace parser;

class VariableParser : public Terminal<Identifier> {};

class IntegralNumberParser : public Terminal<IntegralNumber> {};

class PrimaryExpressionParser
    : public Optional<IntegralNumberParser, VariableParser> {};

class BinaryRHSExpressionParser
    : public Sequence<
          TerminalInRange<BinaryOperatorsRangeBegin, BinaryOperatorsRangeEnd>,
          Optional<
              PrimaryExpressionParser,
              Sequence<PrimaryExpressionParser, BinaryRHSExpressionParser>>> {};

class BinaryExpressionParser
    : public Sequence<PrimaryExpressionParser, BinaryRHSExpressionParser> {};

class BasicExpressionParser
    : public Sequence<BinaryExpressionParser, Terminal<ExpressionSeparator>> {};

class ExpressionParser
    : public Optional<BasicExpressionParser,
                      Sequence<BasicExpressionParser, ExpressionParser>> {};

} // namespace cb

#endif
