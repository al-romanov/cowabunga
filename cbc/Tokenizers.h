#ifndef CBC_TOKENIZERS_H
#define CBC_TOKENIZERS_H

#include "Driver.h"
#include "common/Token.h"
#include "lexer/ITokenizer.h"

#include <string_view>

namespace cb {

class EofTokenizer final : public lexer::ITokenizer {
public:
  std::pair<common::Token, std::string>
  tokenize(std::string_view Word) override;
};

class IdentifierTokenizer : public lexer::ITokenizer {
public:
  std::pair<common::Token, std::string>
  tokenize(std::string_view Word) override;
};

class IntegralNumberTokenizer : public lexer::ITokenizer {
public:
  std::pair<common::Token, std::string>
  tokenize(std::string_view Word) override;
};

class KeywordTokenizer : public lexer::ITokenizer {
public:
  KeywordTokenizer(TokenId TokId, std::string Keyword);

  std::pair<common::Token, std::string>
  tokenize(std::string_view Word) override;

private:
  TokenId Id;
  std::string KeywordString;
};

} // namespace cb

#endif
