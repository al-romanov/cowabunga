#include "common/Metadata.h"

namespace cb {

namespace common {

void Metadata::set(MetadataType Type, std::string Value) {
  DataStorage[Type] = std::move(Value);
}

std::string Metadata::get(MetadataType Type) const {
  auto It = DataStorage.find(Type);
  if (It == DataStorage.cend()) {
    return "Unknown";
  }
  return It->second;
}

} // namespace common

} // namespace cb
