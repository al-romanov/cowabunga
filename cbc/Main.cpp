#include "Driver.h"
#include "Tokenizers.h"
#include "lexer/Lexer.h"

#include <fstream>
#include <iostream>

using namespace cb;

int main(int argc, char **argv) {
  lexer::Lexer Lexer{};
  Lexer.addTokenizer(EofTokenizer())
      .addTokenizer(IntegralNumberTokenizer())
      .addTokenizer(IdentifierTokenizer())
      .addTokenizer(KeywordTokenizer(Assignment, "="))
      .addTokenizer(KeywordTokenizer(Plus, "+"))
      .addTokenizer(KeywordTokenizer(Minus, "-"))
      .addTokenizer(KeywordTokenizer(ShiftLeft, "<<"))
      .addTokenizer(KeywordTokenizer(ShiftRight, ">>"))
      .addTokenizer(KeywordTokenizer(ExpressionSeparator, ";"));

  std::ifstream Script;
  if (argc == 2) {
    Script.open(argv[1]);
  } else {
    std::cerr << "No input files" << std::endl;
    exit(1);
  }
  auto Tokens = Lexer.produceTokens(Script);
  if (!Tokens) {
    std::cerr << "Failed to parse input file" << std::endl;
  }
  for (auto &Token : *Tokens) {
    std::cout << Token << std::endl;
  }
  return 0;
}