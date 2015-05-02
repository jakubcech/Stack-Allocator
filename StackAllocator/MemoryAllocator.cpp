#include "stdafx.h"
#include "MemoryAllocator.h"


// Constructs a stack allocator of the given stackSizeBytes_, reserving a new memory block. 
MemoryAllocator::MemoryAllocator(size_t stackSizeBytes_)
{
	memoryBlock_ = new uint8_t[stackSizeBytes_];
	topMarker_ = reinterpret_cast<uintptr_t>(memoryBlock_);
	originalMarker_ = topMarker_;
}

void *MemoryAllocator::allocateUnalignedMemoryBlock(size_t sizeBytes_)
{
	lastMarker_ = topMarker_;
	uintptr_t memoryBlock = sizeBytes_;

	ptrdiff_t allocatedAddress = topMarker_ + memoryBlock;

	topMarker_ = allocatedAddress;

	uint8_t *pMemory = reinterpret_cast<uint8_t*>(topMarker_);

	return static_cast<void*>(pMemory);
}

void *MemoryAllocator::allocateAlignedMemoryBlock(size_t sizeBytes_, size_t memoryAlignment_)
{
	assert(memoryAlignment_ >= 2);
	assert(memoryAlignment_ <= 128);
	
	// Check if memoryAlignment_ is of power of 2.
	assert((memoryAlignment_ & (memoryAlignment_ - 1)) == 0);

	// Calculate the total amount of memory to allocate.
	size_t sizeWithAlignment = sizeBytes_ + memoryAlignment_;

	// Allocate an unaligned memory block and convert the address to an uintptr_t.
	uintptr_t rawMemoryAddress = reinterpret_cast<uintptr_t>(allocateUnalignedMemoryBlock(sizeWithAlignment));

	// Calculate the necessary adjustment by masking off the lower bits of the address. This determines how misaligned the address is.
	size_t mask = (memoryAlignment_ - 1);
	uintptr_t memoryMisalignment = (rawMemoryAddress & mask);
	ptrdiff_t adjustment = memoryAlignment_ - memoryMisalignment;

	// Calculate the final adjusted address.
	uintptr_t alignedAddress = rawMemoryAddress + adjustment;

	// Store the adjustment (in bytes) in the byte immediately preceding the adjusted address. 
	assert(adjustment < 256);
	uint8_t *pAlignedMemory = reinterpret_cast<uint8_t*>(alignedAddress);
	pAlignedMemory[-1] = static_cast<uint8_t>(adjustment);


	lastMarker_ = topMarker_;
	topMarker_ = alignedAddress;

	return static_cast<void*>(pAlignedMemory);
}


// TODO
void *MemoryAllocator::freeUnalignedMemory(void * pMemory)
{
	ptrdiff_t adjustment = topMarker_ - lastMarker_;
	topMarker_ - adjustment;

	void *pFreedMemory = reinterpret_cast<void*>(topMarker_);
	return (pFreedMemory);
}

void MemoryAllocator::freeAlignedMemory(void * pMemory)
{
	const uint8_t *pAlignedMemory = reinterpret_cast<const uint8_t*>(pMemory);

	uintptr_t alignedAddress = reinterpret_cast<uintptr_t>(pMemory);

	ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMemory[-1]);

	uintptr_t rawMemoryAddress = alignedAddress - adjustment;

	void *pRawMem = reinterpret_cast<void*>(rawMemoryAddress);

	freeUnalignedMemory(pMemory);
}

void MemoryAllocator::clearEntireMemoryBlock()
{
	MemoryAllocator::~MemoryAllocator();
}

MemoryAllocator::~MemoryAllocator() { delete[] memoryBlock_; }


// **TODO
//MemoryAllocator::MemoryAllocator(const MemoryAllocator &memory)
//{
//	memoryBlock_ = new unsigned char[sizeBytes_];
//	for (int i = 0; i != sizeBytes_; ++i)
//		memoryBlock_[i] = memory.memoryBlock_[i];
//
//}
//
//MemoryAllocator & MemoryAllocator::operator=(const MemoryAllocator &memory)
//{
//	unsigned char *p = new unsigned char[sizeBytes_];
//	for (int i = 0; i != sizeBytes_; ++i)
//		p[i] = memory.memoryBlock_[i];
//	delete[] memory.memoryBlock_;
//	memoryBlock_ = p;
//	return *this;
//
//}
//
//MemoryAllocator::MemoryAllocator(MemoryAllocator &&memory)
//{
//	memoryBlock_ = memory.memoryBlock_;
//	memory.memoryBlock_ = nullptr;
//}