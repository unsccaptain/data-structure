#pragma once

#include "Allocator.h"
#include "DenseHash.h"

namespace adt {

	template<typename BucketTy>
	struct UnorderedSetBucketTraits {
		using keyTy = BucketTy;
		static keyTy& getKey(BucketTy& bucket) { return bucket; }
	};

	template<typename Ty, typename AllocatorTy = Allocator>
	class UnorderedSet :public DenseHash<Ty, UnorderedSetBucketTraits<Ty>, AllocatorTy> {
	public:
		using iterator = HashIterator<Ty>;
		using const_iterator = const HashIterator<Ty>;

	public:
		UnorderedSet() { }

		template<typename Iterator>
		UnorderedSet(Iterator First,Iterator Last){
			for (Iterator it = First;it != Last;++it)
				this->insert(*it);
		}

		UnorderedSet(const UnorderedSet& Right) {
			for (iterator it = Right.begin();it != Right.end();++it)
				this->insert(*it);
		}

	private:

	};

}