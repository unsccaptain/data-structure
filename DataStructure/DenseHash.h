// reference from LLVM
#pragma once

#include "Vector.h"
#include "HashTrait.h"
#include <assert.h>

namespace adt {

	template<typename BucketTy>
	struct HashEntry {
		BucketTy Value;
		bool IsDead : 1;
		bool IsEmpty : 1;

		HashEntry()
			:Value(std::move(BucketTy())), IsDead(false), IsEmpty(true) {
		}

		~HashEntry() {
			Value.~BucketTy();
			IsDead = false;
			IsEmpty = true;
		}
	};

	template<typename BucketTy>
	class HashIterator {
	public:
		using entry_iterator = typename HashEntry<BucketTy>*;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = BucketTy;
		using pointer = BucketTy*;
		using reference = BucketTy&;
		using const_reference = const BucketTy&;

	public:
		HashIterator() = delete;

		HashIterator(entry_iterator pos, entry_iterator begin, entry_iterator end)
			:position_(pos), begin_(begin), end_(end) {
		}

		HashIterator& operator++() {
			assert(position_ != end_);
			while (position_ != end_) {
				++position_;
				if (position_->IsDead || position_->IsEmpty) continue;
			}
			return *this;
		}

		const HashIterator operator++(int) const {
			HashIterator old = *this;
			++* this;
			return old;
		}

		HashIterator& operator--() {
			assert(position_ != begin_);
			while (position_ != begin_) {
				--position_;
				if (position_->IsDead || position_->IsEmpty) continue;
			}
			return *this;
		}

		const HashIterator operator--(int) const {
			HashIterator old = *this;
			--* this;
			return old;
		}

		bool operator!=(const HashIterator& another) const {
			return position_ != another.position_;
		}

		bool operator==(const HashIterator& another) const {
			return position_ == another.position_;
		}

		reference operator*() {
			return position_->Value;
		}

		const_reference operator*() const {
			return (const_reference)*position_.Value;
		}

		reference operator->() {
			return *position_.Value;
		}

		const_reference operator->() const {
			return (const_reference)*position_.Value;
		}

	private:
		entry_iterator position_;
		entry_iterator begin_;
		entry_iterator end_;
	};

	template<typename BucketTy, typename BucketTraits, typename AllocatorTy,
		typename HashTraits = Hashable<typename BucketTraits::keyTy>>
		class DenseHash {
		public:
			using keyTy = typename BucketTraits::keyTy;
			using iterator = HashIterator<BucketTy>;
			using const_iterator = const HashIterator<BucketTy>;

			using entry = HashEntry<BucketTy>;
			using entry_iteartor = typename HashEntry<BucketTy>*;

			using al_ = AllocatorTy;

		public:
			DenseHash() {
				buckets_ = AllocBucketBufferAndCotor(32);
				num_buckets_ = 32;
			}

		public:
			iterator insert(const BucketTy& Value) {
				TryGrow();
				entry* place = LookupHashEntry(BucketTraits::getKey(const_cast<BucketTy&>(Value)));
				::new (&place->Value) BucketTy(Value);
				++num_entries_;place->IsEmpty = false;
				if (place->IsDead) {
					place->IsDead = false;
					--num_tombstones_;
				}
				return iterator(place, BucketBegin(), BucketEnd());
			}

			iterator insert(BucketTy&& Value) {
				TryGrow();
				entry* place = LookupHashEntry(BucketTraits::getKey(Value));
				::new (&place->Value) BucketTy(std::move(Value));
				++num_entries_;place->IsEmpty = false;
				if (place->IsDead) {
					place->IsDead = false;
					--num_tombstones_;
				}
				return iterator(place, BucketBegin(), BucketEnd());
			}

			iterator begin() {
				assert(num_entries_ != 0);
				entry_iteartor it = BucketBegin();
				while (it++ != BucketEnd()) { if (!it->IsEmpty && !it->IsDead) break; }
				return iterator(it, BucketBegin(), BucketEnd());
			}

			const_iterator begin() const {
				assert(num_entries_ != 0);
				entry_iteartor it = BucketBegin();
				while (it++ != BucketEnd()) { if (!it->IsEmpty && !it->IsDead) break; }
				return (const_iterator)iterator(it, BucketBegin(), BucketEnd());
			}

			iterator end() {
				assert(num_entries_ != 0);
				return iterator(BucketEnd(), BucketBegin(), BucketEnd());
			}

			const_iterator end() const {
				assert(num_entries_ != 0);
				return (const_iterator)iterator(BucketEnd(), BucketBegin(), BucketEnd());
			}

			iterator find(const keyTy& Key) {
				HashEntry* place = LookupHashEntry(Key);
				if (place->IsDead || place->IsEmpty)
					return end();
				return iterator(place, BucketBegin(), BucketEnd());
			}

			size_t erase(const keyTy& Key) {
				entry* place = LookupHashEntry(Key);
				if (place->IsDead || place->IsEmpty)
					return 0;
				place->Value.~BucketTy();
				place->IsDead = true;
				--num_entries_;
				++num_tombstones_;
				return 1;
			}

			void clear() {
				unsigned NumBuckets = GetNumBuckets();
				std::destroy_n(buckets_, NumBuckets);
				num_entries_ = 0;
				num_tombstones_ = 0;
			}

			void rehash() {
				Grow(GetNumBuckets());
			}

			size_t bucket_count() const { return num_buckets_; }
			size_t bucket_size() const { return num_entries_; }
			bool empty() const { return num_entries_ == 0; }
			size_t count() const { return num_entries_; }
			size_t size() const { return count(); }

			float load_factor() { return 1.0 * 3 / 4; }

		protected:
			unsigned GetNumEntries() { return num_entries_; }
			unsigned GetNumBuckets() { return num_buckets_; }
			unsigned GetNumTombstones() { return num_tombstones_; }
			unsigned Hash(const keyTy& Key) { return HashTraits::hash(Key); }

			entry* AllocBucketBufferAndCotor(size_t Size) {
				entry* Buffer = (entry*)al_::Allocate(Size * sizeof(entry));
				std::uninitialized_fill_n(Buffer, Size, entry());
				return Buffer;
			}

			void Grow(size_t NewSize) {
				entry* OldBuckets = buckets_;
				size_t OldBucketCount = num_buckets_;
				buckets_ = AllocBucketBufferAndCotor(NewSize);
				num_buckets_ = NewSize;
				num_tombstones_ = 0;
				for (unsigned i = 0;i < OldBucketCount;++i) {
					if (!OldBuckets[i].IsDead && !OldBuckets[i].IsEmpty)
						insert(OldBuckets[i].Value);
				}
				std::destroy(OldBuckets, OldBuckets + OldBucketCount);
				al_::Deallocate(OldBuckets);
			}

			void TryGrow() {
				unsigned NewNumNntries = GetNumEntries() + 1;
				unsigned NumBuckets = GetNumBuckets();
				/// bucket¿©»›
				if (NewNumNntries * 4 >= NumBuckets * 3)
					Grow(NumBuckets * 2);
				/// «Â¿Ìtombstone
				else if (NumBuckets - (NewNumNntries + GetNumTombstones()) <= NumBuckets / 8)
					Grow(NumBuckets);
			}

			entry* LookupHashEntry(const keyTy& Key) {
				unsigned NumBuckets = num_buckets_;
				unsigned BucketNo = Hash(Key) & (NumBuckets - 1);
				unsigned ProbeAmt = 1;
				if (NumBuckets == 0)
					return nullptr;

				entry* FirstTombstone = nullptr;

				while (true) {
					entry* solt = buckets_ + BucketNo;
					if (BucketTraits::getKey(solt->Value) == Key) {
						return solt;
					}

					if (solt->IsEmpty) {
						return FirstTombstone == nullptr ? solt : FirstTombstone;
					}

					if (solt->IsDead && !FirstTombstone) {
						FirstTombstone = solt;
					}

					BucketNo += ProbeAmt++;
					BucketNo &= (NumBuckets - 1);
				}
			}

			entry_iteartor BucketBegin() const { return buckets_; }
			entry_iteartor BucketEnd() const { return buckets_ + num_buckets_; }

		protected:
			size_t num_entries_;
			size_t num_tombstones_;
			size_t num_buckets_;
			entry* buckets_;
	};

}