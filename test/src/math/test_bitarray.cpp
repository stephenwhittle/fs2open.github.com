#include <gtest/gtest.h>
#include "util/FSTestFixture.h"
#include <math/bitarray.h>
#include <core/pstypes.h>

class BitArrayTest : public test::FSTestFixture {
  public:
	BitArrayTest() : test::FSTestFixture(INIT_NONE) {}
};

TEST_F(BitArrayTest, bitarray_get) {
	ubyte TestBitArray = 0xAA; // 0b1010101
	for (uint Index = 0; Index < 8; Index+= 2)
	{
		ASSERT_EQ(1, get_bit(&TestBitArray, Index));
	}

	for (uint Index = 1; Index < 8; Index+= 2)
	{
		ASSERT_EQ(0, get_bit(&TestBitArray, Index));
	}
}
