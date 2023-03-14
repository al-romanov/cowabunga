#ifndef LEXER_TOKENIZER_WRAPPER_H
#define LEXER_TOKENIZER_WRAPPER_H

#include "common/IClonableMixin.h"
#include "common/Token.h"
#include "lexer/ITokenizer.h"

#include <memory>
#include <string_view>
#include <type_traits>

namespace cb {

namespace lexer {

class ITokenizerProxy : public ITokenizer {
public:
  virtual std::unique_ptr<ITokenizerProxy> clone() const = 0;
};

template <class Tokenizer,
          class = std::enable_if<std::is_base_of_v<ITokenizer, Tokenizer>>>
class TokenizerProxy final
    : public common::IClonableMixin<ITokenizerProxy,
                                    TokenizerProxy<Tokenizer>> {
public:
  TokenizerProxy(Tokenizer Impl) : TokenizerImpl(Impl) {}

  std::pair<common::Token, std::string>
  tokenize(std::string_view Word) override {
    return TokenizerImpl.tokenize(Word);
  }

private:
  Tokenizer TokenizerImpl;
};

} // namespace lexer

} // namespace cb

#endif
