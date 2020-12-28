#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>

#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

class HCTree {
  public:
    HCNode* root;
    vector<HCNode*> leaves;
    /* TODO: Initializes a new empty HCTree.*/
    HCTree() { root = new HCNode(0, 0); }

    ~HCTree();

    void build(const vector<unsigned int>& freqs);

    void encode(byte symbol, BitOutputStream& out) const;

    void encode(byte symbol, ostream& out) const;

    byte decode(BitInputStream& in) const;

    byte decode(istream& in) const;
};

#endif  // HCTREE_HPP
