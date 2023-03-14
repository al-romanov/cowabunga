#ifndef DRIVER_H
#define DRIVER_H

namespace cb {

enum TokenId {
  EndOfFile,
  Identifier,
  IntegralNumber,
  ExpressionSeparator,
  ArgumentSeparator,
  BinaryOperatorsRangeBegin,
  Assignment,
  Plus,
  Minus,
  Multiplication,
  ShiftLeft,
  ShiftRight,
  BinaryOperatorsRangeEnd,
  Definition,
  ExternalDeclaration,
  BodyBegin,
  BodyEnd,
  OpenParantheses,
  CloseParantheses
};

enum TokenPriority { Unmatched = 0, Normal = 1, High = 2 };

enum OperatorId {
  AssignmentOp = TokenId::Assignment,
  PlusOp = TokenId::Plus,
  MinusOp = TokenId::Minus,
  ShiftLeftOp = TokenId::ShiftLeft,
  ShiftRightOp = TokenId::ShiftRight
};

} // namespace cb

#endif
