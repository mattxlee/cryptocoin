#include "data_value.h"

#include <arpa/inet.h>

namespace coin {
namespace data {

namespace utils {

uint64_t HostToNet(uint64_t value) {
  uint64_t h = (value & 0xffffffff00000000) >> 32;
  uint64_t l = value & 0x00000000ffffffff;
  h = htonl(h);
  l = ntohl(l);
  return (l << 32) + h;
}

uint64_t NetToHost(uint64_t value_n) { return HostToNet(value_n); }

}  // namespace utils

}  // namespace data
}  // namespace coin
