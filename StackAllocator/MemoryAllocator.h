#pragma once
#include "stdafx.h"

// Reserves a memory block on the heap. The block is reserved and maintained in a single-sided stack-like fashion and used for further allocations within the block itself.
// A 'stackMarker' is maintained to point to the top of the 'stack'. Every allocation moves the stack marker up by the sizeBytes_ of the individual allocation.
// Freeing a portion of the stack memory is done by moving the pointer down. This means that the previously written data can be overwritten by the next allocations.
class MemoryAllocator
{
public:

	// Construct a stack allocator of the given stackSize_bytes. 
	explicit MemoryAllocator(size_t stackSize_bytes);

	// Allocate a new block of the given sizeBytes_ beginning at the top of the stack. 
	// The top of the stack is represented by stackMarker.
	virtual void* allocateUnalignedMemoryBlock(size_t sizeBytes_);


	// Allocate a new block of the given sizeBytes_ beginning at the top of the stack. 
	// Aligns the allocation according to memoryAlignment_.
	virtual void* allocateAlignedMemoryBlock(size_t sizeBytes_, size_t memoryAlignment_);

	virtual ~MemoryAllocator();

	MemoryAllocator(const MemoryAllocator &memory);
	MemoryAllocator &operator=(const MemoryAllocator &memory);
	MemoryAllocator(MemoryAllocator &&memory);
	MemoryAllocator &operator=(MemoryAllocator &&memory);
	
	// Free the memory to the marker used before last using the allocateMemoryInBlock() function.
	virtual void* freeUnalignedMemory(void * pMemory);

	// Free the memory to the marker used before last using the allocateMemoryInBlock() function.
	virtual void freeAlignedMemory(void * pMemory);
	
private: 
	
	// The whole block of memory allocated on the heap. Used only when creating an instance of MemoryAllocator.
	uint8_t *memoryBlock_;

	// Represents the current top of the stack. Any further allocations using allocateUnalignedMemoryBlock() and allocateAlignedMemoryBlock() move the topMarker_ up.
	// Freeing up a memory moves the topMarker_ down the stack.
	uintptr_t topMarker_;

	// The marker used prior to the last allocation. 
	uintptr_t lastMarker_;

	// The size of the whole stack-like memory allocated in the memory.
	size_t stackSizeBytes_;

	// The size of a single allocation in the stack-like memory.
	size_t sizeBytes_;

	// The alignment, in bytes, of a single allocation in the stack-like memory.
	size_t memoryAlignment_;

};