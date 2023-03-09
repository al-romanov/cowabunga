#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include "lexer/TokenizerProxy.h"

#include <memory>
#include <optional>
#include <vector>

namespace cb {

namespace common {

class Token;

} // namespace common

namespace lexer {

class Lexer final {
public:
  Lexer() = default;

  Lexer(const Lexer &RHS);

  Lexer(Lexer &&RHS) noexcept = default;

  Lexer &operator=(const Lexer &RHS);

  Lexer &operator=(Lexer &&RHS) noexcept = default;

  template <class Tokenizer> Lexer &addTokenizer(Tokenizer TokenizerImpl) {
    Tokenizers.push_back(
        std::make_unique<TokenizerProxy<Tokenizer>>(std::move(TokenizerImpl)));
    return *this;
  }

  std::optional<std::vector<common::Token>> produceTokens(std::istream &Input);

private:
  std::pair<common::Token, std::string>
  findBestToken(std::string_view Word) const;

  std::vector<std::unique_ptr<ITokenizerProxy>> Tokenizers;
};

} // namespace lexer

} // namespace cb

#endif
