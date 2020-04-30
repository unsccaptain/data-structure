#pragma once

namespace adt {

class Allocator {
public:
  static char *Allocate(size_t Size) { return new char[Size]; }

  static void Deallocate(void *Buffer) { delete[] Buffer; }
};

} // namespace adt