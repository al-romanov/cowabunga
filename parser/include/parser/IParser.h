#ifndef PARSER_IPARSER_H
#define PARSER_IPARSER_H

#include <vector>
namespace cb {

namespace common {

class Token;

} // namespace common

namespace parser {

using TokenIterator = std::vector<common::Token>::const_iterator;

template <class TVisitor> class IParser {
public:
  virtual void parse(TokenIterator ItBegin, TokenIterator ItEnd,
                     TVisitor &Visitor) = 0;

  virtual ~IParser() = default;
};

} // namespace parser

} // namespace cb

#endif
