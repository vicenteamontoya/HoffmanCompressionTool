#include <gtest/gtest.h>
#include <stdio.h>

#include <iostream>
#include <string>

#include "BitInputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitInputStreamTests, SIMPLE_TEST) {
    string byteStr = "10000000";
    char byte = (char)stoi(byteStr, nullptr, 2);

    stringstream ss;
    ss.write(&byte, 1);
    BitInputStream bis(ss, 1);

    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
    bis.readBit();
    ASSERT_EQ(true, bis.eof());
}
/*

bool charToBit(string input1, string input2, unsigned int bufSize) {
    stringstream inStream(input1);
    //inFile.open(inFileName, ios::binary);
    //outFile.open(outFileName);

    BitInputStream in(inStream, bufSize);

    // use vector buffer
    vector<unsigned char> bitsBuf;
    while (1) {
        unsigned int nextBit = in.readBit();
        if (in.eof()) break;
        unsigned char bit = (nextBit == 0) ? '0' : '1';
        bitsBuf.push_back(bit);
    }

    // write out the buffer
    stringstream ss;
    ss.write((const char*)&bitsBuf[0], bitsBuf.size());
    string output = ss.str();
    return output == input2;
}


TEST(BitInputStreamTests, BITCONV_TEST) {
    string original = "a";
    string converted = "01100001";
    bool outcome = charToBit(converted, converted, 100);
    ASSERT_TRUE(outcome);
}
*/
