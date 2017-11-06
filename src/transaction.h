#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>
#include <vector>

#include "hash_utils.h"

namespace coin {

class Transaction {
 public:
  virtual ~Transaction() {}

  virtual int get_type() const = 0;

  void set_index(int index) { index_ = index; }
  int get_index() const { return index_; }

 private:
  int index_ = -1;
};

struct TxIn {
  DataValue block_hash;
  int index;
};

struct TxOut {
  std::string address;
  uint64_t value;
};

class TransactionSpend : public Transaction {
 public:
  virtual int get_type() const override { return 0; }
  void set_pub_key(const std::string &pub_key);
  void set_signature(const DataValue &signature);

  void add_tx_in(TxIn in);
  void add_tx_out(TxOut out);

 private:
  std::string pub_key_;
  DataValue signature_;
  std::vector<TxIn> vec_txin;
  std::vector<TxOut> vec_txout;
};

}  // namespace coin

#endif
