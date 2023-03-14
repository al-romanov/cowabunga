#ifndef LEXER_ITOKENIZER_H
#define LEXER_ITOKENIZER_H

#include <string_view>
#include <type_traits>

namespace cb {

namespace common {

class Token;

} // namespace common

namespace lexer {

class ITokenizer {
public:
  virtual std::pair<common::Token, std::string>
  tokenize(std::string_view Word) = 0;

  virtual ~ITokenizer() = default;
};

} // namespace lexer

} // namespace cb

#endif
