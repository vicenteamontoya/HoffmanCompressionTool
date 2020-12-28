/**
 * File name: uncompress.cpp
 * Description: This file contains functions for uncompressing a Huffman encoded
 * file and a main method to drive it.
 * Guide: ./uncompress data/compressed_file.txt destination_file.txt
 * Command line option: --ascii |To do ascii '0' and '1' pseudo-compression
 *
 * Authors: Haocheng Li, Vicente Montoya
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)\
 */

void printTree(HCNode* node) {
    cout << *node;
    if (node->c0 != NULL) {
        printTree(node->c0);
    }
    if (node->c1 != NULL) {
        printTree(node->c1);
    }
}

void pseudoDecompression(const string& inFileName, const string& outFileName) {
    ifstream compressedFile(inFileName);     // stream of compress file
    ofstream uncompressedFile(outFileName);  // stream to decompressed file
    HCTree tree;                             // encoding tree
    vector<unsigned int> freqs(256);         // ascii frequency array

    // Getting ascii frequency from file
    for (int i = 0; i < 256; i++) {
        compressedFile >> freqs[i];
    }

    // Building the tree
    tree.build(freqs);

    // Reading each character from initial file, decoding, and writing to
    // newfile

    while (compressedFile.peek() != EOF) {
        uncompressedFile << tree.decode(compressedFile);
    }

    // Closing file streams
    compressedFile.close();
    uncompressedFile.close();
}

// Recursive algorithm to perform an in-order transversal of the tree, reading
// each node in the visit
HCNode* readTransversal(HCNode* parent, BitInputStream& in) {
    unsigned int c0Bit = in.readBit();
    unsigned int c1Bit = in.readBit();
    byte value = 0;

    // Reading in ASCII value if leaf node
    if ((c0Bit == 0) && (c1Bit == 0)) {
        for (int i = 0; i < 8; i++) {
            unsigned int bit = in.readBit();
            value = value | (bit << (7 - i));
        }
    }

    HCNode* node = new HCNode(0, value);
    node->p = parent;

    // Reading c0 child
    if (c0Bit == 1) {
        node->c0 = readTransversal(node, in);
    }
    // Reading c1 child
    if (c1Bit == 1) {
        node->c1 = readTransversal(node, in);
    }

    return node;
}

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(const string& inFileName, const string& outFileName) {
    ifstream compressedFile(inFileName);     // stream of compress file
    ofstream uncompressedFile(outFileName);  // stream to decompressed file
    HCTree tree;                             // encoding tree

    unsigned long num_Chars = 0;  // number of characters in file

    BitInputStream in(compressedFile, 4000);  // bitwise output stream

    // Reading the number of characters in file
    for (int i = 0; i < 64; i++) {
        unsigned long bit = in.readBit();
        num_Chars = num_Chars | (bit << (63 - i));
    }

    // Deleting previous root to avoid memory leaks
    delete tree.root;

    // Building tree
    tree.root = readTransversal(NULL, in);

    // Reading each character from initial file, decoding, and writing to
    // newfile
    for (unsigned long i = 0; i < num_Chars; i++) {
        if (in.eof()) break;
        byte character = tree.decode(in);
        uncompressedFile << character;
    }

    // Closing file streams
    compressedFile.close();
    uncompressedFile.close();
}

/* Main program that runs the decompression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Uncompresses files using Huffman Encoding");
    options.positional_help(
        "./path_to_compressed_input_file ./path_to_output_file");

    bool isAscii = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Read input in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAscii))("input", "",
                                       cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit.");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    // if compressed file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        exit(0);
    }

    if (isAscii) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }

    return 0;
}
