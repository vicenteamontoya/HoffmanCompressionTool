#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    // two trees
    HCTree tree2;
    HCTree tree3;

  public:
    SimpleHCTreeFixture() {
        // setting up the first tree
        vector<unsigned int> freqs(256);
        freqs['a'] = 1;
        freqs['b'] = 2;
        freqs['c'] = 3;
        freqs['d'] = 4;
        freqs['e'] = 5;
        tree2.build(freqs);

        // setting up the second tree
        vector<unsigned int> freqs3(256);
        freqs3['z'] = 1;
        tree3.build(freqs3);
        cout << *(tree2.root);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    // checking five different encodings
    ostringstream os;
    tree2.encode('a', os);
    ASSERT_EQ(os.str(), "010");
    os.str("");
    tree2.encode('b', os);
    ASSERT_EQ(os.str(), "011");
    os.str("");
    tree2.encode('c', os);
    ASSERT_EQ(os.str(), "00");
    os.str("");
    tree2.encode('d', os);
    ASSERT_EQ(os.str(), "10");
    os.str("");
    tree2.encode('e', os);
    ASSERT_EQ(os.str(), "11");
    os.str("");
}

TEST_F(SimpleHCTreeFixture, TEST_BYTE_DECODE) {
    string byteStr = "01100000";
    char byte = (char)stoi(byteStr, nullptr, 2);

    stringstream strem;
    strem.write(&byte, 1);
    BitInputStream ibj(strem, 1);

    byteStr = "11000000";
    byte = (char)stoi(byteStr, nullptr, 2);

    stringstream strem2;
    strem2.write(&byte, 1);
    BitInputStream ibj2(strem2, 1);
    ASSERT_EQ('b', tree2.decode(ibj));
    ASSERT_EQ('e', tree2.decode(ibj2));
}

TEST_F(SimpleHCTreeFixture, TEST_BYTE_ENCODE) {
    ostringstream os;
    BitOutputStream obj(os, 2);
    tree2.encode('a', obj);
    obj.flush();
    printf("\n%s\n", os.str());
    ASSERT_EQ(os.str(), "@");
    os.str("");

    BitOutputStream obj2(os, 2);
    tree2.encode('b', obj2);
    obj2.flush();
    printf("\n%s\n", os.str());
    ASSERT_EQ(os.str(), "`");
    os.str("");

    BitOutputStream obj3(os, 2);
    tree2.encode('d', obj3);
    obj3.flush();
    string skrrt = os.str();
    unsigned char brrt = skrrt.at(0);
    ASSERT_EQ((int)brrt, 128);
    os.str("");

    BitOutputStream obj4(os, 2);
    tree2.encode('e', obj4);
    obj4.flush();
    string skrrt2 = os.str();
    unsigned char brrt2 = skrrt2.at(0);
    ASSERT_EQ((int)brrt2, 192);
    os.str("");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    // checking one leaf node
    istringstream is("011");
    ASSERT_EQ(tree2.decode(is), 'b');
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE3) {
    // testing edge case with only one character in the tree
    ostringstream os;
    tree3.encode('z', os);
    ASSERT_EQ(os.str(), "0");
}
