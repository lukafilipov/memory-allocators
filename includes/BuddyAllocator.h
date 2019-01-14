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
	static const size_t k_noBlocksAvailable = ~0;
	static const size_t k_mantissaSize = 52;
	static const size_t k_exponentOffset = 1023;

    byte *m_data = nullptr;
	std::size_t m_minimumSize;
	std::size_t m_levels;
	std::size_t m_freeBitmap = 0;
	
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

    std::size_t intLog2(size_t n) const
	{
		double convert = static_cast<double>(n);
		return (reinterpret_cast<std::size_t&>(convert) >> k_mantissaSize) - k_exponentOffset;
	}
	bool isLog(std::size_t n) const
	{
		return !(n & (n - 1));
	}
	std::size_t firstBiggerLog(std::size_t size)
	{
		if(size <= m_minimumSize) 
		{
			return m_levels - 1;
		}
		if(isLog(size))
		{
			return m_levels - (intLog2(size) - intLog2(m_minimumSize)) - 1;
		}
		else
		{
			return m_levels - (intLog2(size) + 1 - intLog2(m_minimumSize)) - 1;
		}
	}
	std::size_t firstFreeLevel(std::size_t levelToAllocate)
	{
		std::size_t shift = m_levels - 1 - levelToAllocate;
		#ifdef BUILTIN_CTZ_EXISTS
    		return m_levels - ((std::size_t)__builtin_ctz(m_freeBitmap >> shift) + shift) - 1;
		#else
			return m_levels - (intLog2(m_freeBitmap >> shift) + shift) - 1;
		#endif
	}
	void error(std::string error)
	{
		std::cout << error << std::endl;
		exit(1);
	}	
	void initializeSizes();
	
	void initializePointers();
		
	std::size_t fragmentAndAllocate(std::size_t freeLevel, std::size_t levelToAllocate);
	
	std::size_t findFreeBuddy(size_t address);
	
	void merge(std::size_t &address, size_t buddyAddress);
	
	void eraseBlock(std::size_t address, std::size_t level);
	
	std::size_t getBlock(std::size_t freeLevel);
	
	void putBlock(size_t address, std::size_t freeLevel);
	
	void putBlockInFreeList(size_t address, std::size_t freeLevel);
	
	std::size_t getIndex(size_t address, std::size_t level);

	void increaseUsage(std::size_t level);

	void decreaseUsage(size_t address);

};

#endif /* BUDDYALLOCATOR_H */

