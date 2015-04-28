#pragma once
#include <stdint.h>


// Reserves a memory block on the heap. The block is reserved and maintained in a stack-like fashion and used for further allocations within the block itself.
// The size of the block itself is at most 'uint32_t' bytes.
// A 'stackMarker' is maintained to point to the top of the 'stack'. Every allocation moves the stack marker up by the size of the individual allocation.
// Freeing a portion of the stack memory is done by moving the pointer down. This means that the previously written data can be overwritten by the next allocations.
class MemoryAllocator
{
public:
	
	// Constructs a stack allocator of the given size. 
	explicit MemoryAllocator(unsigned char stackSize_bytes);

	virtual ~MemoryAllocator();
	
	MemoryAllocator(const MemoryAllocator &memory);
	MemoryAllocator &operator=(const MemoryAllocator &memory);
	MemoryAllocator(MemoryAllocator &&memory);
	MemoryAllocator &operator=(MemoryAllocator &&memory);

	// Allocates a new block of the given size beginning at the top of the stack. 
	// The top of the stack is represented by stackMarker.
	virtual void allocateMemoryInBlock(unsigned char size_bytes);

	virtual void clearEntireMemoryBlock();
	
	// Frees the memory to the marker used before last using the allocateMemoryInBlock() function.
	virtual void freeMemoryToPreviousMarker();
	
private: 

	unsigned char *memoryBlock;

	// Represents the current top of the stack. Any further allocations on the MemoryAllocator using allocateMemoryInBlock() move the topMarker up.
	// Freeing up a memory moves the topMarker down the stack.
	unsigned char *topMarker;

	// The marker used prior to the last allocation done by allocateMemoryInBlock. 
	unsigned char *lastMarker;
	unsigned char *originalMarker;
	unsigned int size;

};