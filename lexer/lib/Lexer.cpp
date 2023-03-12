#include "lexer/Lexer.h"

#include "common/Metadata.h"
#include "common/Token.h"

#include <istream>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cb {

namespace lexer {

Lexer::Lexer(const Lexer &RHS) {
  Tokenizers.reserve(RHS.Tokenizers.size());
  for (auto &Tokenizer : RHS.Tokenizers) {
    Tokenizers.push_back(Tokenizer->clone());
  }
}

Lexer &Lexer::operator=(const Lexer &RHS) {
  Lexer NewLexer(RHS);
  std::swap(*this, NewLexer);
  return *this;
}

std::optional<std::vector<common::Token>>
Lexer::produceTokens(std::istream &Input) {
  std::vector<common::Token> Tokens;
  std::string Word;
  std::string_view WordView;
  size_t Offset = 0;
  do {
    if (!WordView.empty()) {
      WordView.remove_prefix(Offset);
    }
    if (WordView.empty()) {
      Word.clear();
      Input >> Word;
      Offset = 0;
      WordView = std::string_view(Word.c_str());
    }
    if (WordView.empty()) {
      return Tokens;
    }
    auto Result = findBestToken(WordView);
    if (!Result.first) {
      return std::nullopt;
    }
    Tokens.push_back(std::move(Result.first));
    Offset = Result.second.length();
    Tokens.back().metadata().set(common::Stringified, Result.second);
  } while (Input);
  return Tokens;
}

std::pair<common::Token, std::string>
Lexer::findBestToken(std::string_view Word) const {
  std::pair<common::Token, std::string> BestResult;
  for (auto &Tokenizer : Tokenizers) {
    auto Result = Tokenizer->tokenize(Word);
    if (Result > BestResult) {
      BestResult = Result;
    }
  }
  return BestResult;
}

} // namespace lexer

} // namespace cb
