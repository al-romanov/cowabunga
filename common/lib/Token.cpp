#include "common/Token.h"

#include "common/Metadata.h"

#include <limits>
#include <ostream>

namespace cb {

namespace common {

Token::Token()
    : Priority(std::numeric_limits<int>::min()), Initialized(false) {}

Token::Token(int TokenId, int TokenPriority)
    : Id(TokenId), Priority(TokenPriority), Initialized(true) {}

bool Token::equals(const Token &RHS) const noexcept {
  return Initialized == RHS.Initialized && Id == RHS.Id &&
         Priority == RHS.Priority;
}

bool Token::less(const Token &RHS) const noexcept {
  return !Initialized && RHS.Initialized || Priority < RHS.Priority;
}

void Token::print(std::ostream &Out) const {
  Out << "Token {" << MetadataStorage.get(common::Stringified) << ", " << Id
      << "}";
}

common::Metadata &Token::metadata() noexcept { return MetadataStorage; }

std::string Token::metadata(MetadataType Type) const {
  return MetadataStorage.get(Type);
}

int Token::id() const noexcept { return Id; }

int Token::priority() const noexcept { return Priority; }

bool Token::initialized() const noexcept { return Initialized; }

Token::operator bool() const noexcept { return initialized(); }

} // namespace common

} // namespace cb
