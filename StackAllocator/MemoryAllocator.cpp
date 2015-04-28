#include "stdafx.h"
#include "MemoryAllocator.h"

// Constructs a stack allocator of the given size, reserving a new memory block on the heap. 
MemoryAllocator::MemoryAllocator(unsigned char stackSize_bytes)
{
	memoryBlock = new unsigned char[stackSize_bytes];
	size = (int) stackSize_bytes;
	topMarker = &memoryBlock[0];
	originalMarker = &memoryBlock[0];
}

MemoryAllocator::~MemoryAllocator() { delete[] memoryBlock; }


MemoryAllocator::MemoryAllocator(const MemoryAllocator &memory)
{
	memoryBlock = new unsigned char[size];
	for (int i = 0; i != size; ++i)
		memoryBlock[i] = memory.memoryBlock[i];
	
}

MemoryAllocator & MemoryAllocator::operator=(const MemoryAllocator &memory)
{
	unsigned char *p = new unsigned char[size];
	for (int i = 0; i != size; ++i)
		p[i] = memory.memoryBlock[i];
	delete[] memory.memoryBlock;
	memoryBlock = p;
	return *this;

}


MemoryAllocator::MemoryAllocator(MemoryAllocator &&memory)
{
	memoryBlock = memory.memoryBlock;
	memory.memoryBlock = nullptr;
}

void MemoryAllocator::allocateMemoryInBlock(unsigned char size_bytes)
{
	lastMarker = topMarker;
	topMarker = &memoryBlock[size_bytes];
}

void MemoryAllocator::freeMemoryToPreviousMarker()
{
	topMarker = lastMarker;
}


void MemoryAllocator::clearEntireMemoryBlock()
{
	topMarker = originalMarker;
}
