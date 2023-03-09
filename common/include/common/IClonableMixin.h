#ifndef COMMON_ICLONABLEMIXIN_H
#define COMMON_ICLONABLEMIXIN_H

#include <memory>
namespace cb {

namespace common {

template <class Base, class Derived> class IClonableMixin : public Base {
public:
  std::unique_ptr<Base> clone() const override {
    return std::make_unique<Derived>(static_cast<const Derived &>(*this));
  }
};

} // namespace common

} // namespace cb

#endif
