#include "pub_key.h"

#include <cstring>

namespace ecdsa {

/** This function is taken from the libsecp256k1 distribution and implements
 *  DER parsing for ECDSA signatures, while supporting an arbitrary subset of
 *  format violations.
 *
 *  Supported violations include negative integers, excessive padding, garbage
 *  at the end, and overly long length descriptors. This is safe to use in
 *  Bitcoin because since the activation of BIP66, signatures are verified to be
 *  strict DER before being passed to this module, and we know it supports all
 *  violations present in the blockchain before that point.
 */
static int ecdsa_signature_parse_der_lax(const secp256k1_context *ctx,
                                         secp256k1_ecdsa_signature *sig,
                                         const unsigned char *input,
                                         size_t inputlen) {
  size_t rpos, rlen, spos, slen;
  size_t pos = 0;
  size_t lenbyte;
  unsigned char tmpsig[64] = {0};
  int overflow = 0;

  /* Hack to initialize sig with a correctly-parsed but invalid signature. */
  secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);

  /* Sequence tag byte */
  if (pos == inputlen || input[pos] != 0x30) {
    return 0;
  }
  pos++;

  /* Sequence length bytes */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    pos += lenbyte;
  }

  /* Integer tag byte for R */
  if (pos == inputlen || input[pos] != 0x02) {
    return 0;
  }
  pos++;

  /* Integer length for R */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    while (lenbyte > 0 && input[pos] == 0) {
      pos++;
      lenbyte--;
    }
    if (lenbyte >= sizeof(size_t)) {
      return 0;
    }
    rlen = 0;
    while (lenbyte > 0) {
      rlen = (rlen << 8) + input[pos];
      pos++;
      lenbyte--;
    }
  } else {
    rlen = lenbyte;
  }
  if (rlen > inputlen - pos) {
    return 0;
  }
  rpos = pos;
  pos += rlen;

  /* Integer tag byte for S */
  if (pos == inputlen || input[pos] != 0x02) {
    return 0;
  }
  pos++;

  /* Integer length for S */
  if (pos == inputlen) {
    return 0;
  }
  lenbyte = input[pos++];
  if (lenbyte & 0x80) {
    lenbyte -= 0x80;
    if (pos + lenbyte > inputlen) {
      return 0;
    }
    while (lenbyte > 0 && input[pos] == 0) {
      pos++;
      lenbyte--;
    }
    if (lenbyte >= sizeof(size_t)) {
      return 0;
    }
    slen = 0;
    while (lenbyte > 0) {
      slen = (slen << 8) + input[pos];
      pos++;
      lenbyte--;
    }
  } else {
    slen = lenbyte;
  }
  if (slen > inputlen - pos) {
    return 0;
  }
  spos = pos;

  /* Ignore leading zeroes in R */
  while (rlen > 0 && input[rpos] == 0) {
    rlen--;
    rpos++;
  }
  /* Copy R value */
  if (rlen > 32) {
    overflow = 1;
  } else {
    std::memcpy(tmpsig + 32 - rlen, input + rpos, rlen);
  }

  /* Ignore leading zeroes in S */
  while (slen > 0 && input[spos] == 0) {
    slen--;
    spos++;
  }
  /* Copy S value */
  if (slen > 32) {
    overflow = 1;
  } else {
    std::memcpy(tmpsig + 64 - slen, input + spos, slen);
  }

  if (!overflow) {
    overflow = !secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
  }
  if (overflow) {
    /* Overwrite the result again with a correctly-parsed but invalid
       signature if parsing failed. */
    std::memset(tmpsig, 0, 64);
    secp256k1_ecdsa_signature_parse_compact(ctx, sig, tmpsig);
  }
  return 1;
}

PubKey::PubKey(const std::vector<uint8_t> &pub_key_data)
    : pub_key_data_(pub_key_data) {
  // Create secp256k1 context.
  ctx_ = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
}

PubKey::PubKey(PubKey &&rhs) {
  pub_key_data_.assign(std::begin(rhs.pub_key_data_),
                       std::end(rhs.pub_key_data_));
  ctx_ = rhs.ctx_;
  rhs.pub_key_data_.clear();
  rhs.ctx_ = nullptr;
}

PubKey &PubKey::operator=(PubKey &&rhs) {
  if (this != &rhs) {
    pub_key_data_.assign(std::begin(rhs.pub_key_data_),
                         std::end(rhs.pub_key_data_));
    ctx_ = rhs.ctx_;
    rhs.pub_key_data_.clear();
    rhs.ctx_ = nullptr;
  }
  return *this;
}

PubKey::~PubKey() {
  secp256k1_context_destroy(ctx_);
  ctx_ = nullptr;
}

bool PubKey::Verify(const std::vector<uint8_t> &hash,
                    const std::vector<uint8_t> &sig_in) const {
  // Parse public key.
  secp256k1_pubkey pubkey;
  if (!secp256k1_ec_pubkey_parse(ctx_, &pubkey, pub_key_data_.data(),
                                 pub_key_data_.size())) {
    return false;
  }

  // Parse signature.
  secp256k1_ecdsa_signature sig;
  if (!ecdsa_signature_parse_der_lax(ctx_, &sig, sig_in.data(),
                                     sig_in.size())) {
    return false;
  }

  /* libsecp256k1's ECDSA verification requires lower-S signatures, which have
   * not historically been enforced in Bitcoin, so normalize them first. */
  secp256k1_ecdsa_signature_normalize(ctx_, &sig, &sig);
  return secp256k1_ecdsa_verify(ctx_, &sig, hash.data(), &pubkey);
}

}  // namespace ecdsa