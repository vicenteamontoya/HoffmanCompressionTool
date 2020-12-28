/**
 * File name: compress.cpp
 * Description: This file contains functions for uncompressing a Huffman encoded
 * file and a main method to drive it.
 * Guide: ./compress data/source_file.txt destination_compressed_file.txt
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

// struct pair_obj = struct pair(i,s.substr(i));

void printTree(HCNode* node) {
    cout << *node;
    if (node->c0 != NULL) {
        printTree(node->c0);
    }
    if (node->c1 != NULL) {
        printTree(node->c1);
    }
}

// Recursive algorithm to perform an in-order transversal of the tree, writing
// each node in the visit
void writeTransversal(HCNode* node, BitOutputStream& out) {
    // Writing c0 child
    if (node->c0 == NULL) {
        out.writeBit(0);
    } else {
        out.writeBit(1);
    }

    // Writing c1 child
    if (node->c1 == NULL) {
        out.writeBit(0);
    } else {
        out.writeBit(1);
    }

    // If it's a leaf child write symbol
    if ((node->c0 == NULL) && (node->c1 == NULL)) {
        for (int i = 0; i < 8; i++) {
            unsigned int bit = ((node->symbol >> (7 - i)) & 1);
            out.writeBit(bit);
        }
    }

    // Visiting c0 child
    if (node->c0 != NULL) {
        writeTransversal(node->c0, out);
    }

    // Visiting c1 child
    if (node->c1 != NULL) {
        writeTransversal(node->c1, out);
    }
    return;
}

void pseudoCompression(const string& inFileName, const string& outFileName) {
    ifstream uncompressedFile(inFileName);  // stream of file to compress
    ofstream compressedFile(outFileName);   // stream to compressed file
    HCTree tree;                            // encoding tree
    vector<unsigned int> freqs(256);        // ascii frequency array
    vector<unsigned int>::iterator itr = freqs.begin();  // to iterate freqs
    vector<unsigned int>::iterator end = freqs.end();    // to iterate freqs
    byte b;  // character to read/write stream

    // Getting ascii frequency from file
    while (uncompressedFile.peek() != EOF) {
        b = uncompressedFile.get();
        freqs[b]++;
    }

    // Building the tree
    tree.build(freqs);

    // Writing the freq to compressed file
    while (itr != end) {
        compressedFile << *itr << endl;
        itr++;
    }

    // Reopening the uncompressed file
    uncompressedFile.close();
    uncompressedFile.open(inFileName);

    // Read each character from initial file, encoding, and writing to new file
    while (uncompressedFile.peek() != EOF) {
        b = uncompressedFile.get();
        tree.encode(b, compressedFile);
    }

    // Closing the file streams
    compressedFile.close();
    uncompressedFile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {
    ifstream uncompressedFile(inFileName);  // stream of file to compress
    ofstream compressedFile(outFileName);   // stream to compressed file
    HCTree tree;                            // encoding tree
    vector<unsigned int> freqs(256);        // ascii frequency array
    vector<unsigned int>::iterator itr = freqs.begin();  // to iterate freqs
    vector<unsigned int>::iterator end = freqs.end();    // to iterate freqs
    byte b;                       // character to read/write stream
    unsigned long num_Chars = 0;  // number of characters in file

    // Getting ascii frequency from file
    while (uncompressedFile.peek() != EOF) {
        b = uncompressedFile.get();
        freqs[b]++;
        num_Chars++;
    }

    // Building the tree
    tree.build(freqs);

    BitOutputStream out(compressedFile, 4000);  // bitwise output stream

    // Writing the number of characters in file
    for (int i = 0; i < 64; i++) {
        unsigned int bit = ((num_Chars >> (63 - i)) & 1);
        out.writeBit(bit);
    }

    // Writing the tree encoding into the file
    HCNode* node = tree.root;
    writeTransversal(node, out);

    // Reopening the uncompressed file
    uncompressedFile.close();
    uncompressedFile.open(inFileName);

    // Read each character from initial file, encoding, and writing to new file
    while (uncompressedFile.peek() != EOF) {
        b = uncompressedFile.get();
        tree.encode(b, out);
    }

    // Flushing bit stream
    out.flush();

    // Closing the file streams
    compressedFile.close();
    uncompressedFile.close();
}

/* Main program that runs the compression */
int main(int argc, char* argv[]) {
    cxxopts::Options options(argv[0],
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        return 0;
    }

    // if original file is empty, output empty file
    if (FileUtils::isEmptyFile(inFileName)) {
        ofstream outFile;
        outFile.open(outFileName, ios::out);
        outFile.close();
        return 0;
    }

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}
