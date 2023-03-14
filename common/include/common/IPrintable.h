#ifndef COMMON_IPRINTABLE_H
#define COMMON_IPRINTABLE_H

#include <ostream>

namespace cb {

namespace common {

template <class Derived> class IPrintable {
public:
  virtual void print(std::ostream &Out) const = 0;
};

template <class Derived>
std::ostream &operator<<(std::ostream &Out, const IPrintable<Derived> &RHS) {
  RHS.print(Out);
  return Out;
}

} // namespace common

} // namespace cb

#endif
