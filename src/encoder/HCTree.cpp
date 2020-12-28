#include "HCTree.hpp"

void deleter(HCNode* node) {
    if (node == NULL) {
        return;
    }
    deleter(node->c0);
    deleter(node->c1);
    delete node;
}

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() { deleter(root); }

/**
 * TODO: Build the HCTree from the given frequency vector. You can assume the
 * vector must have size 256 and each value at index i represents the frequency
 * of char with ASCII value i. Only non-zero frequency symbols should be used to
 * build the tree. The leaves vector must be updated so that it can be used in
 * encode() to improve performance.
 *
 * When building the HCTree, you should use the following tie-breaking rules to
 * match the output from reference solution in checkpoint:
 *
 *    1. HCNode with lower count should have higher priority. If count is the
 * same, then HCNode with a larger ascii value symbol should have higher
 * priority. (This should be already defined properly in the comparator in
 * HCNode.hpp)
 *    2. When popping two highest priority nodes from PQ, the higher priority
 * node will be the ‘c0’ child of the new parent HCNode.
 *    3. The symbol of any parent node should be taken from its 'c0' child.
 */

typedef priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

void HCTree::build(const vector<unsigned int>& freqs) {
    vector<unsigned int>::const_iterator it =
        freqs.begin();  // iterator through freqs
    pq nodeList;        // priority queue to store nodes
    int i = 0;  // i is the index of the iterator, and the ASCII value of
                // current char
    while (it != freqs.end()) {
        // *it is the count, i is the ASCII value
        if (*it > 0) {  // if this is a letter with frequency
            HCNode* newNode = new HCNode(*it, i);  // creating new node pair
            nodeList.push(newNode);                // adding new node to queue
            leaves.push_back(newNode);             // adding node to leaves
        } else {
            leaves.push_back(NULL);  // adding null placeholders to leaves
        }
        i++;
        it++;
    }

    // now nodelist is a heap of nodes
    // iterate through node list to merge nodes
    while (nodeList.size() > 2) {
        // popping two new nodes
        HCNode* smaller = nodeList.top();
        nodeList.pop();
        HCNode* larger = nodeList.top();
        nodeList.pop();
        // now two original  nodes are removed from the priority queue

        // creating new parent node
        HCNode* newParent =
            new HCNode(smaller->count + larger->count, smaller->symbol);
        nodeList.push(newParent);

        // connecting the nodes together
        newParent->c0 = smaller;
        newParent->c1 = larger;
        larger->p = newParent;
        smaller->p = newParent;
    }

    // tying thnigs up at the end
    if (nodeList.size() == 2) {
        // connecting the root with the tree
        HCNode* smaller = nodeList.top();
        nodeList.pop();
        HCNode* larger = nodeList.top();
        root->c0 = smaller;
        root->c1 = larger;
        larger->p = root;
        smaller->p = root;
        root->symbol = larger->symbol;
        root->count = smaller->count + larger->count;
    } else if (nodeList.size() == 1) {
        // edge case for if there's only one node in the frequency list
        HCNode* loner = nodeList.top();
        nodeList.pop();
        root->c0 = loner;
        root->count = loner->count;
        root->symbol = loner->symbol;
        loner->p = root;
    }
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    // initialize encoding stirng
    string encoding = "";

    // retrieve the leaf node containing the info on the symbol
    HCNode* node = leaves[symbol];

    // if tree doesn't contain the symbol
    if (node == NULL) {
        return;
    }

    // initialize parent node look-ahead traverser
    HCNode* parent = node->p;

    // traverse up the tree until we hit the root
    while (parent != 0) {
        // append to encoding depending on the leg of the tree
        if (node == parent->c0) {
            encoding = '0' + encoding;
        } else {
            encoding = '1' + encoding;
        }
        node = parent;
        parent = node->p;
    }

    for (int i = 0; i < encoding.length(); i++) {
        if (encoding[i] == '0') {
            out.writeBit(0);
        } else {
            out.writeBit(1);
        }
    }
    return;
}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    // initialize encoding stirng
    string encoding = "";

    // retrieve the leaf node containing the info on the symbol
    HCNode* node = leaves[symbol];

    // if tree doesn't contain the symbol
    if (node == NULL) {
        out << encoding;
        return;
    }

    // initialize parent node look-ahead traverser
    HCNode* parent = node->p;

    // traverse up the tree until we hit the root
    while (parent != 0) {
        // append to encoding depending on the leg of the tree
        if (node == parent->c0) {
            encoding = '0' + encoding;
        } else {
            encoding = '1' + encoding;
        }
        node = parent;
        parent = node->p;
    }

    out << encoding;
    return;
}

/**
 * TODO: Decode the sequence of bits (represented as a char of either '0' or
 * '1') from the istream to return the coded symbol. For this function to
 * work, build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(BitInputStream& in) const {
    // initialize traverser node and character
    unsigned int code;
    HCNode* node = root;

    // continue going down the tree until we find a leaf node or
    while ((node->c0 != NULL) || (node->c1 != NULL)) {
        // get next 0 or 1 to go down the branch
        code = in.readBit();

        // traverse down accordingly
        if (code == 0) {
            node = node->c0;
        } else if (code == 1) {
            node = node->c1;
        }
    }

    return node->symbol;
}

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or
 * '1') from istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    // initialize traverser node and character
    char code;
    HCNode* node = root;

    // continue going down the tree until we find a leaf node or
    while ((node->c0 != NULL) || (node->c1 != NULL)) {
        // get next 0 or 1 to go down the branch
        in >> code;

        // traverse down accordingly
        if (code == '0') {
            node = node->c0;
        } else if (code == '1') {
            node = node->c1;
        }
    }

    return node->symbol;
}