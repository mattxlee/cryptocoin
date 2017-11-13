#include "address.h"

#include <stdlib.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "base58.h"
#include "hash_utils.h"

namespace coin {

Address Address::FromPublicKey(const std::vector<uint8_t> &pub_key) {
  // 1. SHA256
  auto result = Hash256Builder::CalculateHash(pub_key.data(), pub_key.size());

  // 2. RIPEMD160
  result = Hash256Builder::CalculateHash(result.data(), result.size());

  // 3. Add 0x00 on front
  std::vector<uint8_t> temp;
  temp.resize(result.size() + 1);
  temp[0] = 0x00;
  std::memcpy(temp.data() + 1, result.data(), result.size());
  result = temp;

  // 4. SHA256 twice
  result = Hash256Builder::CalculateHash(result.data(), result.size());
  result = Hash256Builder::CalculateHash(result.data(), result.size());

  // 5. Take first 4 bytes and add to temp
  std::vector<uint8_t> long_result;
  long_result.resize(temp.size() + 4);
  memcpy(long_result.data(), temp.data(), temp.size());
  memcpy(long_result.data() + temp.size(), result.data(), 4);

  // 6. Base58
  Address addr;
  addr.addr_str_ = base58::EncodeBase58(long_result);

  // Returns address object
  return addr;
}

}  // namespace coin
