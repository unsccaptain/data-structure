#pragma once

#include "Allocator.h"
#include "DenseHash.h"

namespace adt {

template <typename KeyTy, typename ValTy> struct UnorderedMapBucketTy {
  using keyTy = KeyTy;
  using valTy = ValTy;
  keyTy First;
  valTy Second;
};

template <typename BucketTy> struct UnorderedMapBucketTraits {
  using keyTy = typename BucketTy::keyTy;
  static keyTy &getKey(BucketTy &bucket) { return bucket.First; }
};

template <typename KeyTy, typename ValTy,
          typename BucketTy = UnorderedMapBucketTy<KeyTy, ValTy>,
          typename AllocatorTy = Allocator>
class UnorderedMap
    : public DenseHash<BucketTy, UnorderedMapBucketTraits<BucketTy>,
                       AllocatorTy> {
public:
  using iterator = HashIterator<BucketTy>;
  using const_iterator = const HashIterator<BucketTy>;
  using hash_entry = HashEntry<BucketTy>;

  ValTy &operator[](const KeyTy &Key) {
    hash_entry *place = this->LookupHashEntry(Key);
    if (place->IsDead || place->IsEmpty) {
      place->Value.First = Key;
      ::new (&place->Value.Second) ValTy();
    }
    return place->Value.Second;
  }

  ValTy &operator[](KeyTy &&Key) {
    hash_entry *place = this->LookupHashEntry(Key);
    if (place->IsDead || place->IsEmpty) {
      place->Value.First = std::move(Key);
      ::new (&place->Value.Second) ValTy();
    }
    return place->Value.Second;
  }

private:
};

} // namespace adt