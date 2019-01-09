#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H

#include "Allocator.h"
#include <cmath>
#include <string>
#include <iostream>

typedef unsigned char byte;

class BuddyAllocator : public Allocator 
{
protected:
	static const size_t SIZE_T_MAX = ~0;

    void *m_data = nullptr;
	std::size_t m_minimumSize;
	byte m_levels;
	
	std::size_t *m_freeLists;
	byte *m_blockIndex;
	byte *m_blockLevels;
	
	std::size_t m_sizeFreeLists;
	std::size_t m_sizeIndex;
	std::size_t m_sizeBlockLevels;
	std::size_t m_sizeMetadata;

public:
	BuddyAllocator(const std::size_t totalSize, const std::size_t minimumSize = 16);

	virtual ~BuddyAllocator();

	virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
	
	virtual void Free(void* ptr) override;

	virtual void Init() override;

	virtual void Reset();
private:
	BuddyAllocator(BuddyAllocator &buddyAllocator);

    byte intLog2(double n)
	{
		return (reinterpret_cast<std::size_t&>(n) >> 52) - 1023;
	}
	bool isLog(std::size_t n)
	{
		return !(n & (n - 1));
	}
	byte firstBiggerLog(std::size_t size)
	{
		if(size <= m_minimumSize) return m_levels - 1;
		if(isLog(size))
			return m_levels - (intLog2(size) - intLog2(m_minimumSize)) - 1;
		else
			return m_levels - (intLog2(size) + 1 - intLog2(m_minimumSize)) - 1;
	}
	void error(std::string error)
	{
		std::cout << error << std::endl;
		exit(1);
	}	
	void initializeSizes();
	
	void initializePointers();
	
	byte firstFreeLevel(byte levelToAllocate);
	
	std::size_t fragmentAndAllocate(byte freeLevel, byte levelToAllocate);
	
	std::size_t findFreeBuddy(size_t address);
	
	void merge(std::size_t &address, size_t buddyAddress);
	
	void eraseBlock(std::size_t address, byte level);
	
	std::size_t getBlock(byte freeLevel);
	
	void putBlock(size_t address, byte freeLevel);
	
	void putBlockInFreeList(size_t address, byte freeLevel);
	
	std::size_t getIndex(size_t address, byte level);

	void increaseUsage(byte level);

	void decreaseUsage(size_t address);

};

#endif /* BUDDYALLOCATOR_H */
