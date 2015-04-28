#include "stdafx.h"
#include <iostream>
#include "MemoryAllocator.h"

int _tmain(int argc, _TCHAR* argv[])
{

	unsigned char sizeB = 2;
	unsigned char newBlock = 1;

	auto memory = MemoryAllocator(sizeB);
	memory.allocateMemoryInBlock(newBlock); 

	memory.~MemoryAllocator();

	return 0;
}

