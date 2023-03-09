#ifndef DRIVER_H
#define DRIVER_H

namespace cb {

enum TokenId {
  Eof,
  Identifier,
  IntegralNumber,
  ExpressionSeparator,
  ArgumentSeparator,
  BinaryOperatorsRangeBegin,
  Assignment,
  Plus,
  Minus,
  ShiftLeft,
  ShiftRight,
  BinaryOperatorsRangeEnd,
  Definition,
  ExternalDeclaration,
  BodyBegin,
  BodyEnd
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
