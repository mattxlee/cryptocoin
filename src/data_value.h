#ifndef __DATA_VALUE_H__
#define __DATA_VALUE_H__

#include <cassert>
#include <functional>
#include <string>
#include <vector>

namespace coin {
namespace data {

typedef std::vector<uint8_t>::iterator DataIterator;
typedef std::vector<uint8_t>::const_iterator ConstDataIterator;

typedef std::function<void(ConstDataIterator, ConstDataIterator)> WithFuncType;

namespace utils {

std::vector<uint8_t> ToData(const std::string &str);
std::vector<uint8_t> ToData(const uint16_t value);
std::vector<uint8_t> ToData(const uint32_t value);
std::vector<uint8_t> ToData(const uint64_t value);

size_t FromData(DataIterator begin, DataIterator end, std::string &out);
size_t FromData(DataIterator begin, DataIterator end, uint16_t &out);
size_t FromData(DataIterator begin, DataIterator end, uint32_t &out);
size_t FromData(DataIterator begin, DataIterator end, uint64_t &out);

}  // namespace utils

template <typename T>
class Value {
 public:
  Value() {}
  Value(const T &value) : value_(value) {}
  Value(T &&value) { value_ = std::move(value); }

  DataIterator set_data(DataIterator begin, DataIterator end) {
    size_t bytes_taken = utils::FromData(begin, end, value_);
    return begin + bytes_taken;
  }

  std::vector<uint8_t> get_data() const { return utils::ToData(value_); }

  void WithData(const WithFuncType &func) const {
    std::vector<uint8_t> data = utils::ToData(value_);
    func(data.cbegin(), data.cend());
  }

 private:
  T value_;
};

template <>
class Value<std::vector<uint8_t>> {
 public:
  Value() {}
  Value(const std::vector<uint8_t> &value) : value_(value) {}
  Value(std::vector<uint8_t> &&value) { value_ = std::move(value); }

  DataIterator set_data(DataIterator begin, DataIterator end) {
    value_.assign(begin, end);
    return end;
  }

  std::vector<uint8_t> get_data() const { return value_; }

  void WithData(const WithFuncType &func) const {
    func(value_.cbegin(), value_.cend());
  }

  const uint8_t *get_buffer() const { return value_.data(); }
  size_t get_size() const { return value_.size(); }

  void Resize(size_t new_size) { value_.resize(new_size); }
  void CopyFrom(const uint8_t *p, size_t size) {
    value_.resize(size);
    memcpy(value_.data(), p, size);
  }

 private:
  std::vector<uint8_t> value_;
};

typedef Value<std::vector<uint8_t>> Buffer;

}  // namespace data
}  // namespace coin

#endif
