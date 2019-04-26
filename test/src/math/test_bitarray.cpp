#include "util/FSTestFixture.h"

#include <core/pstypes.h>
#include <gtest/gtest.h>
#include <math/bitarray.h>

class BitArrayTest : public test::FSTestFixture {
  public:
	BitArrayTest() : test::FSTestFixture(INIT_NONE) {}
};

TEST_F(BitArrayTest, bitarray_get)
{
	ubyte TestBitArray[]         = {0xAA, 0x55, 0x0f, 0xf0, 0xb, 0xa, 0xa, 0xd, 0xf, 0x0, 0x0, 0xd};
	ubyte ExpectedResults[12][8] = {{1, 0, 1, 0, 1, 0, 1, 0}, {0, 1, 0, 1, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 1, 1, 1},
	                                {1, 1, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 1, 1}, {0, 0, 0, 0, 1, 0, 1, 0},
	                                {0, 0, 0, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 1, 1, 0, 1}, {0, 0, 0, 0, 1, 1, 1, 1},
	                                {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 1}};
	for (uint Index = 0; Index < 96; Index++) {
		ASSERT_EQ(ExpectedResults[Index / 8][7 - Index % 8], get_bit(&TestBitArray, Index)) << Index;
	}
}
TEST_F(BitArrayTest, bitarray_set)
{
	ubyte TestBitArray[8] = {0};
	for (uint BytePos = 0; BytePos < 63; BytePos++) {
		ASSERT_EQ(0, get_bit(&TestBitArray, BytePos));
		set_bit(&TestBitArray, BytePos);
		ASSERT_EQ(1, get_bit(&TestBitArray, BytePos));
	}
}

TEST_F(BitArrayTest, bitarray_clear)
{
	ubyte TestBitArray[8] = {0};
	memset(TestBitArray, 0xFF, 8);
	for (uint BytePos = 0; BytePos < 63; BytePos++) {
		ASSERT_EQ(1, get_bit(&TestBitArray, BytePos));
		clear_bit(&TestBitArray, BytePos);
		ASSERT_EQ(0, get_bit(&TestBitArray, BytePos));
	}
}

TEST_F(BitArrayTest, bitarray_toggle)
{
	ubyte TestBitArray[]         = {0xAA, 0x55, 0x0f, 0xf0, 0xb, 0xa, 0xa, 0xd, 0xf, 0x0, 0x0, 0xd};
	ubyte ExpectedResults[12][8] = {{1, 0, 1, 0, 1, 0, 1, 0}, {0, 1, 0, 1, 0, 1, 0, 1}, {0, 0, 0, 0, 1, 1, 1, 1},
	                                {1, 1, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 0, 1, 1}, {0, 0, 0, 0, 1, 0, 1, 0},
	                                {0, 0, 0, 0, 1, 0, 1, 0}, {0, 0, 0, 0, 1, 1, 0, 1}, {0, 0, 0, 0, 1, 1, 1, 1},
	                                {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 1}};
	for (uint Index = 0; Index < 96; Index++) {
		toggle_bit(&TestBitArray, Index);
		ASSERT_EQ(!(ExpectedResults[Index / 8][7 - Index % 8]), get_bit(&TestBitArray, Index)) << Index;
	}
}