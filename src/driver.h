#ifndef KALEIDOC_SRC_DRIVER_H_
#define KALEIDOC_SRC_DRIVER_H_

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace kaleidoc {



enum MetadataType {
  kStringified,
};  // Not implemented yet



enum OperatorAssociativity { kLeftToRight, kRightToLeft };

class IMetadataHandler {
 public:
  virtual void AddMetadata(MetadataType type, std::string value) = 0;

  virtual const std::map<MetadataType, std::string> &metadata() const = 0;
};

template<class T> class IClonable {
 public:
  virtual std::unique_ptr<T> Clone() const = 0;
};

class IPrintable {
 public:
  virtual void Print(std::ostream &out) const = 0;
};

class Token;



class Lexer {
 public:
  virtual Lexer &AddTokenizer(std::unique_ptr<Tokenizer> analyzer) = 0;

  virtual std::vector<Token> ProduceTokens(std::istream &input) const = 0;

  virtual ~Lexer() = default;
};

class AstNode;

class AstPrinter {
 public:
  virtual void PrintAst(AstNode *node) = 0;
};

class AstNode : public IClonable<AstNode>, public IPrintable, IMetadataHandler {
 public:
  AstNode() = default;

  AstNode(std::map<MetadataType, std::string> metainfo);

  AstNode(const AstNode &node) = default;

  AstNode(AstNode &&node) noexcept = default;

  AstNode &operator=(const AstNode &node) = default;

  AstNode &operator=(AstNode &&node) noexcept = default;

  void AddMetadata(MetadataType type, std::string value) override;

  const std::map<MetadataType, std::string> &metadata() const override;

  void Print(std::ostream &out) const override;

  virtual void AcceptAstPrinter(AstPrinter *printer) const = 0;

  virtual ~AstNode() = default;

 protected:
  std::map<MetadataType, std::string> metadata_;
};

std::ostream &operator<<(std::ostream &out, const AstNode &node);

class AstBuilder : public IClonable<AstBuilder> {
 public:
  virtual std::pair<std::unique_ptr<AstNode>,
                    std::vector<Token>::const_iterator>
  Build(std::vector<Token>::const_iterator begin,
        std::vector<Token>::const_iterator end) const = 0;

  virtual ~AstBuilder() = default;
};

class Parser {
 public:
  virtual AstBuilder *RegisterAstBuilder(AstBuilder *builder) = 0;

  virtual std::unique_ptr<AstNode> Parse(
      std::vector<Token>::const_iterator begin,
      std::vector<Token>::const_iterator end) const = 0;
};

std::map<OperatorId, int> GetOperatorPrecedenceValues();

std::map<OperatorId, OperatorAssociativity> GetOperatorAssociativityValues();

class Driver {};

}  // namespace kaleidoc

#endif  // KALEIDOC_SRC_DRIVER_H_