#pragma once

#include <stdint.h>

namespace adt {

template <typename T> struct Hashable {};

template <> struct Hashable<bool> {
  static unsigned hash(const bool &Value) { return Value * 37U; }
};

template <> struct Hashable<unsigned char> {
  static unsigned hash(const unsigned char &Value) { return Value * 37U; }
};

template <> struct Hashable<unsigned short> {
  static unsigned hash(const unsigned short &Value) { return Value * 37U; }
};

template <> struct Hashable<unsigned int> {
  static unsigned hash(const unsigned int &Value) { return Value * 37U; }
};

template <> struct Hashable<unsigned long> {
  static unsigned hash(const unsigned long &Value) { return Value * 37U; }
};

template <> struct Hashable<unsigned long long> {
  static unsigned hash(const unsigned long long &Value) { return Value * 37U; }
};

template <> struct Hashable<char> {
  static unsigned hash(const char &Value) { return Value * 37U; }
};

template <> struct Hashable<short> {
  static unsigned hash(const short &Value) { return Value * 37U; }
};

template <> struct Hashable<int> {
  static unsigned hash(const int &Value) { return Value * 37U; }
};

template <> struct Hashable<long> {
  static unsigned hash(const long &Value) { return Value * 37U; }
};

template <> struct Hashable<long long> {
  static unsigned hash(const long long &Value) { return Value * 37U; }
};

template <> struct Hashable<void *> {
  static unsigned hash(const void *&Value) { return (intptr_t)Value * 37U; }
};

} // namespace adt