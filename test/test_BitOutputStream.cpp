#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(0);
    bos.writeBit(0);
    bos.writeBit(0);
    bos.writeBit(0);
    bos.writeBit(0);
    bos.writeBit(0);
    bos.writeBit(1);
    bos.flush();

    string bitsStr1 = "11000000";
    string bitsStr2 = "10000000";
    unsigned int asciiVal1 = stoi(bitsStr1, nullptr, 2);
    unsigned int asciiVal2 = stoi(bitsStr2, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal1);
    ASSERT_EQ(ss.get(), asciiVal2);
}