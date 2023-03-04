#include "parser.h"

#include <cstdlib>
#include <sstream>
#include <utility>

#include "driver.h"
#include "lexer.h"

namespace kaleidoc {

// --- EXCEPTIONS ---

UnknownAstBuilderRuleException::UnknownAstBuilderRuleException(std::string msg)
    : msg_{std::move(msg)} {}

const char *UnknownAstBuilderRuleException::what() const noexcept {
  return msg_.c_str();
}

UnexpectedTokenException::UnexpectedTokenException(std::string msg)
    : msg_{std::move(msg)} {}

const char *UnexpectedTokenException::what() const noexcept {
  return msg_.c_str();
}

// --- AST BUILDERS ---

// INTEGRAL NUMBER AST BUILDER

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
IntegralNumberAstBuilder::Build(std::vector<Token>::const_iterator begin,
                                std::vector<Token>::const_iterator end) const {
  if (begin == end || begin->id() != TokenId::kIntegralNumber) {
    return std::make_pair(nullptr, ++begin);
  }

  return std::make_pair(std::make_unique<IntegralNumberAstNode>(*begin),
                        begin + 1);
}

std::unique_ptr<AstBuilder> IntegralNumberAstBuilder::Clone() const {
  return std::make_unique<IntegralNumberAstBuilder>(*this);
}

// VARIABLE AST BUILDER

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
VariableAstBuilder::Build(std::vector<Token>::const_iterator begin,
                          std::vector<Token>::const_iterator end) const {
  if (begin == end || begin->id() != TokenId::kIdentifier) {
    return std::make_pair(nullptr, begin);
  }
  return std::make_pair(std::make_unique<VariableAstNode>(*begin), begin + 1);
}

std::unique_ptr<AstBuilder> VariableAstBuilder::Clone() const {
  return std::make_unique<VariableAstBuilder>(*this);
}

// PRIMARY EXPRESSION AST BUILDER

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
PrimaryExpressionAstBuilder::Build(
    std::vector<Token>::const_iterator begin,
    std::vector<Token>::const_iterator end) const {
  for (const auto &builder : ast_builders_) {
    auto result = builder->Build(begin, end);
    if (result.first) { return result; }
  }
  return std::make_pair(nullptr, begin);
}

void PrimaryExpressionAstBuilder::AddBuilder(AstBuilder *builder) {
  ast_builders_.push_back(builder);
}

std::unique_ptr<AstBuilder> PrimaryExpressionAstBuilder::Clone() const {
  return std::make_unique<PrimaryExpressionAstBuilder>(*this);
}

// BINARY EXPRESSION AST BUILDER

BinaryExpressionAstBuilder::BinaryExpressionAstBuilder()
    : primary_builder_{nullptr} {
  operator_precedence_[OperatorId::kMinusOp] = kMaxPrecedence / 2;
  operator_precedence_[OperatorId::kPlusOp] = kMaxPrecedence / 2;
  operator_precedence_[OperatorId::kShiftLeftOp] = kMaxPrecedence / 4;
  operator_precedence_[OperatorId::kShiftRightOp] = kMaxPrecedence / 4;
  operator_precedence_[OperatorId::kAssignmentOp] = 1;
}

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
BinaryExpressionAstBuilder::Build(
    std::vector<Token>::const_iterator begin,
    std::vector<Token>::const_iterator end) const {
  auto lhs_result = primary_builder_->Build(begin, end);
  if (!lhs_result.first) { return lhs_result; }
  auto binop_result =
      BuildBinopRhs(lhs_result.second, end, std::move(lhs_result.first));
  if (!binop_result.first) { return std::make_pair(nullptr, begin); }
  return binop_result;
}

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
BinaryExpressionAstBuilder::BuildBinopRhs(
    std::vector<Token>::const_iterator begin,
    std::vector<Token>::const_iterator end,
    std::unique_ptr<AstNode> lhs) const {
  if (begin == end || !TokenIsBinaryOperator(*begin)) {
    return std::make_pair(nullptr, begin);
  }
  return BuildBinopRhsChain(begin, end, std::move(lhs));
}

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
BinaryExpressionAstBuilder::BuildBinopRhsChain(
    std::vector<Token>::const_iterator begin,
    std::vector<Token>::const_iterator end,
    std::unique_ptr<AstNode> lhs) const {
  auto op_token = begin;
  int binop_precedence =
      operator_precedence_.at(static_cast<OperatorId>(op_token->id()));
  auto rhs_result = BuildRhsPrimary(begin + 1, end);
  begin = rhs_result.second;
  if (begin == end || !TokenIsBinaryOperator(*begin)) {
    return std::make_pair(
        std::make_unique<BinaryExpressionAstNode>(*op_token, std::move(lhs),
                                                  std::move(rhs_result.first)),
        begin);
  }
  auto next_operator_id = static_cast<OperatorId>(begin->id());
  int next_binop_precedence = operator_precedence_.at(next_operator_id);
  if (binop_precedence > next_binop_precedence) {
    auto lhs_ast = std::make_unique<BinaryExpressionAstNode>(
        *op_token, std::move(lhs), std::move(rhs_result.first));
    return BuildBinopRhsChain(begin, end, std::move(lhs_ast));
  }
  auto binop_rhs_result =
      BuildBinopRhsChain(begin, end, std::move(rhs_result.first));
  return std::make_pair(
      std::make_unique<BinaryExpressionAstNode>(
          *op_token, std::move(lhs), std::move(binop_rhs_result.first)),
      binop_rhs_result.second);
}

std::pair<std::unique_ptr<AstNode>, std::vector<Token>::const_iterator>
BinaryExpressionAstBuilder::BuildRhsPrimary(
    std::vector<Token>::const_iterator begin,
    std::vector<Token>::const_iterator end) const {
  auto result = primary_builder_->Build(begin, end);
  if (!result.first) {
    std::stringstream str;
    if (result.second == end) {
      str << "Unexpected end of line!";
    } else if (begin->metadata().count(MetadataType::kStringified)) {
      str << "Unexpected token: "
          << begin->metadata().at(MetadataType::kStringified);
    }
    throw UnknownAstBuilderRuleException{str.str()};
  }
  return result;
}

bool BinaryExpressionAstBuilder::TokenIsBinaryOperator(
    const Token &token) noexcept {
  return token.id() > TokenId::kBinaryOperatorsRangeBegin &&
         token.id() < TokenId::kBinaryOperatorsRangeEnd;
}

void BinaryExpressionAstBuilder::SetPrimaryBuilder(AstBuilder *builder) {
  primary_builder_ = builder;
}

std::unique_ptr<AstBuilder> BinaryExpressionAstBuilder::Clone() const {
  return std::make_unique<BinaryExpressionAstBuilder>(*this);
}

// --- AST NODES ---

// INTEGRAL NUMBER AST NODE

IntegralNumberAstNode::IntegralNumberAstNode(const Token &token)
    : AstNode{token.metadata()} {
  if (token.id() != TokenId::kIntegralNumber) {
    std::stringstream str;
    str << "Expected Token {" << TokenId::kIntegralNumber << "}, got " << token;
    throw UnexpectedTokenException{str.str()};
  }
}

void IntegralNumberAstNode::Print(std::ostream &out) const {
  out << "Integer '" << metadata_.at(MetadataType::kStringified) << "'";
}

std::unique_ptr<AstNode> IntegralNumberAstNode::Clone() const {
  return std::make_unique<IntegralNumberAstNode>(*this);
}

// VARIABLE AST NODE

VariableAstNode::VariableAstNode(const Token &token)
    : AstNode{token.metadata()} {
  if (token.id() != TokenId::kIdentifier) {
    std::stringstream str;
    str << "Expected Token {" << TokenId::kIdentifier << "}, got " << token;
    throw UnexpectedTokenException{str.str()};
  }
}

void VariableAstNode::Print(std::ostream &out) const {
  out << "Variable '" << metadata_.at(MetadataType::kStringified) << "'";
}

std::unique_ptr<AstNode> VariableAstNode::Clone() const {
  return std::make_unique<VariableAstNode>(*this);
};

// BINARY EXPRESSION AST NODE

BinaryExpressionAstNode::BinaryExpressionAstNode(const Token &token,
                                                 std::unique_ptr<AstNode> lhs,
                                                 std::unique_ptr<AstNode> rhs)
    : AstNode{token.metadata()}, lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {
  if (token.id() < TokenId::kBinaryOperatorsRangeBegin ||
      token.id() > TokenId::kBinaryOperatorsRangeEnd) {
    std::stringstream str;
    str << "Expected Token { binary_expression }, got " << token;
    throw UnexpectedTokenException{str.str()};
  }
  op_ = static_cast<OperatorId>(token.id());
}

BinaryExpressionAstNode::BinaryExpressionAstNode(
    const BinaryExpressionAstNode &rhs)
    : AstNode{rhs.metadata_},
      op_{rhs.op_},
      lhs_{rhs.lhs_->Clone()},
      rhs_{rhs.rhs_->Clone()} {}

BinaryExpressionAstNode &BinaryExpressionAstNode::operator=(
    const BinaryExpressionAstNode &rhs) {
  BinaryExpressionAstNode new_node{rhs};
  std::swap(*this, new_node);
  return *this;
}

OperatorId BinaryExpressionAstNode::operator_id() const { return op_; }

void BinaryExpressionAstNode::Print(std::ostream &out) const {
  out << "BinaryOperator '" << metadata_.at(MetadataType::kStringified) << "'"
      << "\n/" << *lhs_ << "\n\\" << *rhs_;
}

std::unique_ptr<AstNode> BinaryExpressionAstNode::Clone() const {
  return std::make_unique<BinaryExpressionAstNode>(*this);
}

}  // namespace kaleidoc