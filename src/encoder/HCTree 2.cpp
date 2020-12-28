#include "HCTree.hpp"

/* TODO: Delete all objects on the heap to avoid memory leaks. */
HCTree::~HCTree() {}

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
    printf("%s \n", "Called tree building function!");
    vector<unsigned int>::const_iterator it =
        freqs.begin();  // iterator through freqs
    pq nodeList;
    int i = 0;  // i is the index of the iterator, and the ASCII value of
                // current char
    while (it != freqs.end()) {
        // *it is the count, i is the ASCII value
        if (*it > 0) {  // if this is a letter with no
            HCNode* newNode = new HCNode(*it, i);

            nodeList.push(newNode);
            leaves.push_back(newNode);
        } else {
            leaves.push_back(NULL);
        }
        i++;
        it++;
    }

    printf("%s \n", "successfully populated leaves and nodelist first run");
    printf("Node list size: %d", nodeList.size());
    printf("leaves size: %d", leaves.size());

    // now nodelist is a heap of nodes
    while (nodeList.size() > 2) {
        // printf("%s\n","node list size is greater than 2");
        HCNode* smaller = nodeList.top();
        nodeList.pop();
        HCNode* larger = nodeList.top();
        nodeList.pop();
        HCNode* newParent =
            new HCNode(smaller->count + larger->count, larger->symbol);
        nodeList.push(newParent);
        newParent->c0 = smaller;
        newParent->c1 = larger;
        larger->p = newParent;
        smaller->p = newParent;
    }
    if (nodeList.size() == 2) {
        // printf("%s\n","node list size is 2");
        HCNode* smaller = nodeList.top();
        // printf("%s\n","topped smaller element");
        // printf("Smaller element is: %c, %d \n", smaller->symbol,
        // smaller->count); printf("%s\n","Commencing pop()");
        nodeList.pop();
        HCNode* larger = nodeList.top();
        // printf("%s\n","topped larger element");
        // printf("larger element is: %c, %d \n", larger->symbol,
        // larger->count); printf("%s\n","Commencing pop()");
        root->c0 = smaller;
        root->c1 = larger;
        larger->p = root;
        smaller->p = root;
        root->symbol = larger->symbol;
        root->count = smaller->count + larger->count;
    } else if (nodeList.size() == 1) {
        // printf("%s\n","node list size is 1");
        HCNode* loner = nodeList.top();
        nodeList.pop();
        root->count = loner->count;
        root->symbol = loner->symbol;
    } else {
    }
    printf("%s\n", "tree building finished");
}

/**
 * TODO: Write the encoding bits of the given symbol to the ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must be called before
 * to create the HCTree.
 */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/**
 * TODO: Write the encoding bits of the given symbol to ostream. You should
 * write each encoding bit as ascii char either '0' or '1' to the ostream. You
 * must not perform a comprehensive search to find the encoding bits of the
 * given symbol, and you should use the leaves vector instead to achieve
 * efficient encoding. For this function to work, build() must have been called
 * beforehand to create the HCTree.
 */
void HCTree::encode(byte symbol, ostream& out) const {
    string encoding = "";

    printf("%s\n",
           "Successfully entered the encoding function. let's see what's "
           "happening.");

    HCNode* node = leaves[symbol];

    printf("%s\n", "Acquired node from leaves");

    printf("This node is: %c, %d \n", node->symbol, node->count);

    HCNode* parent = node->p;
    printf("%s\n", "Acquired parent from node's parent");

    while (parent != 0) {
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
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/**
 * TODO: Decode the sequence of bits (represented as char of either '0' or
 * '1') from istream to return the coded symbol. For this function to work,
 * build() must have been called beforehand to create the HCTree.
 */
byte HCTree::decode(istream& in) const {
    char code;
    HCNode* node = root;

    while ((node->c0 != NULL) && (node->c1 != NULL)) {
        code = in.get();
        if (code == 0) {
            node = node->c0;
        } else {
            node = node->c1;
        }
    }

    return node->symbol;
}
