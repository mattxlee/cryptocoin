#ifndef __DATA_VALUE_H__
#define __DATA_VALUE_H__

#include <arpa/inet.h>

#include <cassert>
#include <cstring>

#include <functional>
#include <string>
#include <vector>

namespace coin {
namespace data {

typedef std::vector<uint8_t>::iterator DataIterator;
typedef std::vector<uint8_t>::const_iterator ConstDataIterator;

typedef std::function<void(ConstDataIterator, ConstDataIterator)> WithFuncType;

namespace utils {

template <typename IntType>
IntType HostToNet(const IntType value) {
  size_t size = sizeof(value);
  IntType value_n = value;
  switch (size) {
    case 2:
      value_n = htons(value);
      break;
    case 4:
      value_n = htonl(value);
      break;
  }
  return value_n;
}

template <typename IntType>
IntType NetToHost(IntType value_n) {
  size_t size = sizeof(IntType);
  IntType value = value_n;
  switch (size) {
    case 2:
      value = ntohs(value);
      break;
    case 4:
      value = ntohl(value);
      break;
  }
  return value;
}

uint64_t HostToNet(uint64_t value);

uint64_t NetToHost(uint64_t value_n);

}  // namespace utils

template <typename T>
class Value {
 public:
  T value = 0;

 public:
  Value() {}
  Value(const T &another) : value(another) {}

  template <typename Stream>
  void WriteToStream(Stream &s) const {
    auto value_n = utils::HostToNet(value);
    s.write((const char *)&value_n, sizeof(value_n));
  }

  template <typename Stream>
  void ReadFromStream(Stream &s) {
    T value_n;
    s.read((char *)&value_n, sizeof(value_n));
    value = utils::NetToHost(value_n);
  }

  std::vector<uint8_t> MakeStreamData() const {
    std::vector<uint8_t> data(sizeof(T));
    T value_stream = utils::HostToNet(value);
    memcpy(data.data(), &value_stream, sizeof(T));
    return data;
  }
};

template <>
class Value<std::string> {
 public:
  std::string value;

 public:
  Value() {}
  Value(const std::string &another) : value(another) {}

  template <typename Stream>
  void WriteToStream(Stream &s) const {
    uint32_t size = value.size();
    uint32_t size_n = utils::HostToNet(size);
    s.write((const char *)&size_n, sizeof(size_n));
    s.write(value.c_str(), value.size());
  }

  template <typename Stream>
  void ReadFromStream(Stream &s) {
    uint32_t size_n;
    s.read((char *)&size_n, sizeof(size_n));
    uint32_t size = utils::NetToHost(size_n);
    char *buf = new char[size + 1];
    s.read(buf, size);
    buf[size] = '\0';
    value = buf;
    delete[] buf;
  }

  std::vector<uint8_t> MakeStreamData() const {
    std::vector<uint8_t> data(value.size() + sizeof(uint32_t));
    uint32_t size = value.size();
    memcpy(data.data(), &size, sizeof(size));
    memcpy(data.data() + sizeof(size), value.c_str(), value.size());
    return data;
  }
};

template <>
class Value<std::vector<uint8_t>> {
 public:
  std::vector<uint8_t> value;

 public:
  Value() {}
  Value(const std::vector<uint8_t> &another) : value(another) {}

  void CopyFrom(const uint8_t *p, size_t size) {
    value.resize(size);
    std::memcpy(value.data(), p, size);
  }

  template <typename Stream>
  void WriteToStream(Stream &s) const {
    uint32_t size_n = utils::HostToNet(value.size());
    s.write((const char *)&size_n, sizeof(size_n));
    s.write((const char *)value.data(), value.size());
  }

  template <typename Stream>
  void ReadFromStream(Stream &s) {
    uint32_t size;
    s.read(&size, sizeof(size));
    size = utils::NetToHost(size);
    value.resize(size);
    s.read(value.data(), size);
  }

  std::vector<uint8_t> MakeStreamData() const {
    std::vector<uint8_t> data(value.size() + sizeof(uint32_t));
    uint32_t size = value.size();
    memcpy(data.data(), &size, sizeof(size));
    memcpy(data.data() + sizeof(size), value.data(), value.size());
    return data;
  }
};

template <typename T>
Value<T> MakeValue(const T &value) {
  return Value<T>(value);
}

template <typename Stream, typename T>
T ReadValue(Stream &s) {
  Value<T> value;
  value.ReadFromStream(s);
  return value.value;
}

typedef Value<std::vector<uint8_t>> Buffer;

}  // namespace data
}  // namespace coin

#endif
