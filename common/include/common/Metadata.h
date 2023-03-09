#ifndef COMMON_METADATA_H
#define COMMON_METADATA_H

#include <map>
#include <string>

namespace cb {

namespace common {

enum MetadataType { Stringified };

class Metadata final {
public:
  void set(MetadataType Type, std::string Value);

  std::string get(MetadataType Type) const;

private:
  std::map<MetadataType, std::string> DataStorage;
};

} // namespace common

} // namespace cb

#endif
