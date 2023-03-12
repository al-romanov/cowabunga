#ifndef PARSER_IMATCHER_H
#define PARSER_IMATCHER_H

#include <vector>

namespace cb {

namespace common {

class Token;

} // namespace common

namespace parser {

using TokenIterator = std::vector<common::Token>::const_iterator;

class IMatcher {
public:
  virtual TokenIterator match(TokenIterator ItBegin, TokenIterator ItEnd) = 0;

  virtual ~IMatcher() = default;
};

} // namespace parser

} // namespace cb

#endif
