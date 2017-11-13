#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>
#include <vector>

#include "big_num.h"
#include "data_value.h"
#include "hash_utils.h"
#include "key.h"
#include "merkle_tree.h"

namespace coin {

/// Basic transaction.
class TransactionBase {
 public:
  /// Create new Transaction.
  TransactionBase();

  /// New transaction type can inherite from.
  virtual ~TransactionBase() {}

  /// Get timestamp.
  time_t get_time() const { return time_; }

  /// Set timestamp.
  void set_time(time_t time) { time_ = time; }

  /// Get hash value.
  const bn::HashNum &get_hash() const { return hash_; }
  bn::HashNum &get_hash() { return hash_; }

  /// Set hash value.
  void set_hash(const bn::HashNum &hash) { hash_ = hash; }

  /// Transaction type value, starts from 0.
  virtual int get_type() const = 0;

  /// Calculate hash value.
  virtual const data::Buffer CalcHash() const = 0;

 private:
  time_t time_ = 0;
  bn::HashNum hash_;
};

/// Transaction incoming tx.
struct TxIn {
  bn::HashNum tx_hash;    // From transaction hash value.
  int out_index;           // txout index.
  data::Buffer signature;  // Signature of hash(tx_hash + out_index).

  data::Buffer CalcHash() const {
    Hash256Builder hash_builder;
    hash_builder << data::MakeValue(tx_hash) << data::MakeValue(out_index) << signature;
    return hash_builder.FinalValue();
  }

  template <typename Stream>
  void Serialize(Stream &s) const {
    data::MakeValue(tx_hash).WriteToStream(s);
    data::MakeValue(out_index).WriteToStream(s);
    signature.WriteToStream(s);
  }

  template <typename Stream>
  void Unserialize(Stream &s) {
    data::MakeValue(tx_hash).ReadFromStream(s);
    out_index = data::ReadValue<uint32_t>(s);
    signature.ReadFromStream(s);
  }
};

/// Transaction outcoming tx.
struct TxOut {
  std::string address;  // To address.
  uint64_t amount;      // Transfer amount.

  data::Buffer CalcHash() const {
    Hash256Builder hash_builder;
    hash_builder << data::MakeValue(address) << data::MakeValue(amount);
    return hash_builder.FinalValue();
  }

  template <typename Stream>
  void Serialize(Stream &s) const {
    data::MakeValue(address).WriteToStream(s);
    data::MakeValue(amount).WriteToStream(s);
  }

  template <typename Stream>
  void Unserialize(Stream &s) {
    address = data::ReadValue<std::string>(s);
    amount = data::ReadValue<uint64_t>(s);
  }
};

namespace tx {

/**
 * @brief Make a signature for tx.
 *
 * @param key Private key to make signature.
 * @param tx_hash Transaction hash value.
 * @param out_index Out index.
 *
 * @return Signature data.
 */
data::Buffer MakeTxSignature(const ecdsa::Key &key, const data::Buffer &tx_hash,
                             int out_index);

}  // namespace tx

/// Spend transaction.
class Transaction : public TransactionBase {
 public:
  enum { TypeValue = 0 };

 public:
  /// The type of current transaction.
  virtual int get_type() const override { return Transaction::TypeValue; }

  /// Set public key, verification of TxIn.
  void set_pub_key(const data::Buffer &pub_key);

  /// Add TxIn record.
  void add_tx_in(const TxIn &in);

  /// Add TxOut record.
  void add_tx_out(const TxOut &out);

  /// Serialize to stream.
  template <typename Stream>
  void Serialize(Stream &s) const {
    // Header values.
    data::MakeValue(get_type()).WriteToStream(s);  // type
    data::MakeValue(get_time()).WriteToStream(s);  // timestamp
    pub_key_.WriteToStream(s);                     // public key

    // Tx in/out merkle tree hash value.
    auto txin_root = mt::MakeMerkleTree(vec_txin);    // TxIn
    auto txout_root = mt::MakeMerkleTree(vec_txout);  // TxOut
    Hash256Builder hash_builder;
    if (txin_root) {
      hash_builder << txin_root->get_hash();
    }
    if (txout_root) {
      hash_builder << txout_root->get_hash();
    }
    hash_builder.FinalValue().WriteToStream(s);

    // TxIn list.
    data::MakeValue(static_cast<int>(vec_txin.size())).WriteToStream(s);
    for (const TxIn &in : vec_txin) {
      in.Serialize(s);
    }

    // TxOut list.
    data::MakeValue(static_cast<int>(vec_txout.size())).WriteToStream(s);
    for (const TxOut &out : vec_txout) {
      out.Serialize(s);
    }
  }

  /// Unserialize from stream.
  template <typename Stream>
  void Unserialize(Stream &s) {
    // Type.
    int type = data::ReadValue<int>(s);
    assert(type == Transaction::TypeValue);

    // Timestamp.
    set_time(data::ReadValue<time_t>(s));

    // Public key.
    pub_key_.ReadFromStream(s);

    // Merkle tree hash value.
    auto merkle_hash = data::ReadValue<std::vector<uint8_t>>(s);

    // TxIn list.
    auto txin_n = data::ReadValue<int>(s);
    for (int i = 0; i < txin_n; ++i) {
      TxIn txin;
      txin.Unserialize(s);
      vec_txin.push_back(txin);
    }

    // TxOut list.
    auto txout_n = data::ReadValue<int>(s);
    for (int i = 0; i < txout_n; ++i) {
      TxOut txout;
      txout.Unserialize(s);
      vec_txout.push_back(txout);
    }
  }

  virtual const data::Buffer CalcHash() const override {
    auto txin_root = mt::MakeMerkleTree(vec_txin);    // TxIn
    auto txout_root = mt::MakeMerkleTree(vec_txout);  // TxOut
    Hash256Builder hash_builder;
    if (txin_root) {
      hash_builder << txin_root->get_hash();
    }
    if (txout_root) {
      hash_builder << txout_root->get_hash();
    }
    data::Buffer hash_data = hash_builder.FinalValue();
    return hash_data;
  }

 private:
  data::Buffer pub_key_;
  std::vector<TxIn> vec_txin;
  std::vector<TxOut> vec_txout;
};

}  // namespace coin

#endif
