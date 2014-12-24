#ifndef LOCAL_HEAP_H
#define LOCAL_HEAP_H

#include <array>
#include "BaseHeap.h"

namespace hoard {


class LocalHeap : public BaseHeap {

constexpr static size_t kBinCount = 7; //blocks_allocated / size = 0,  <=1/8, <=1/4, <=2/4, <=3/4, <1, 1
constexpr static size_t kAlmostFullBlocksBinNum = 6; // 3/4<  blocks_allocated / size <1
constexpr static size_t kEmptySuperblocksBinNum = 0; // blocks_allocated / size = 0

public:
	LocalHeap(GlobalHeap &parent_heap)
			: parent_heap_(parent_heap),
				block_size_(parent_heap.block_size()),
				blocks_allocated_(0),
				size_(0) {
	}

	void * Alloc() {
		lock_guard guard(lock);
		if (blocks_allocated_ == size_) {
			GetSuperblock();
		}

		for(int i_bin = kAlmostFullBlocksBinNum; i_bin >= 0; --i_bin) {
			SuperblockStack & bin = bins_[i_bin];
			if (!bin.IsEmpty()) {
				SuperblockHeader & header = bin.Top()->header();
				assert(header.owner() == this);
				void * allocated =  header.Alloc();
				const int new_bin_num = GetBinNum(header.blocks_allocated(), header.block_size());
				if (new_bin_num != i_bin) {
					Superblock *superblock = bin.Pop();
					bins_[new_bin_num].Push(superblock);
				}
				++blocks_allocated_;
				return allocated;
			}
		}
		assert(false && "no superblock choosed, shouldn't happen");
	}

	virtual void OnFreeSuperblock(Superblock *superblock) override {
		SuperblockHeader & header = superblock->header();
		assert(header.owner() == this);
    assert(blocks_allocated_ > 0);
		--blocks_allocated_;
		const size_t old_bin_num = GetBinNum(header.blocks_allocated() + 1, header.block_size());
		bins_[old_bin_num].Remove(superblock);
		const size_t new_bin_num = GetBinNum(header.block_size(), header.block_size());
		bins_[new_bin_num].Push(superblock);
		if(BellowEmptynessThreshold()) {
			TransferSuperblock();
		}
	}



private:
	GlobalHeap& parent_heap_;
	const size_t block_size_;
	std::array<SuperblockStack, kBinCount> bins_;
	size_t blocks_allocated_;
	size_t size_;
	size_t superblock_count_;

	bool BellowEmptynessThreshold() {
		return superblock_count_ > kSuperblocsInLocalHeapLowBound  && BellowEmptynessThreshold(blocks_allocated_, size_);
	}
	bool BellowEmptynessThreshold(size_t blocks_allocated, size_t size) {
		return (blocks_allocated * kEmptynessFactor) / size > 0;
	}


	void GetSuperblock() {
		Superblock *superblock;
		{
			lock_guard guard(parent_heap_.lock);
			superblock = parent_heap_.GetSuperblock();
			SuperblockHeader& header = superblock->header();
			assert(header.owner() == &parent_heap_);
			header.set_owner(this);
		}
		SuperblockHeader& header = superblock->header();
		assert(header.block_size() == block_size_);
		assert(BellowEmptynessThreshold(header.blocks_allocated(), header.block_size()));
		blocks_allocated_ += header.blocks_allocated();
		size_ += header.size();
		++superblock_count_;
	}

	void TransferSuperblock() {
		for(size_t i_bin = 0; i_bin < kBinCount; ++i_bin) {
			SuperblockStack & bin = bins_[i_bin];
			if (!bin.IsEmpty()) {
				Superblock *superblock = bin.Pop();
				SuperblockHeader &header = superblock->header();
				assert(BellowEmptynessThreshold(header.blocks_allocated(), header.size())
						&& "Transfered superblock, shoul be bellow emptyness threshold");
				blocks_allocated_ -= header.blocks_allocated();
				size_ -= header.size();
				--superblock_count_;
				{
					lock_guard guard(parent_heap_.lock);
					parent_heap_.AddSuperblock(superblock);
				}
			}
		}
		assert(false && "no nonempty bins");

	}


	size_t GetBinNum(size_t blocs_allocated, size_t blocks_size) {
		//TODO tests. Test, below emptyness threshhold, and above is in different bins
		switch (RoundUp(blocs_allocated * 8, blocks_size) / blocks_size) {
			case 0: return 0; // e == 0
			case 1: return 1; // x <= 1/8
			case 2: return 2; // <= 1/4
			case 3:
			case 4: return 4; // <= 2/4
			case 5:
			case 6: return 5; // <= 3/4
			case 7:
			case 8: return blocs_allocated == blocks_size ? 7 : 6;
		}
		assert(false && "invalid bin num");
	}
};

}
#endif // LOCAL_HEAP_H
