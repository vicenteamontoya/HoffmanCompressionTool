/**
 * TODO: file header
 *
 * Author:
 */
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
using namespace std;

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
void pseudoCompression(const string& inFileName, const string& outFileName) {
    ifstream uncompressedFile("inFileName");  // stream of file to compress
    ofstream compressedFile("outFileName");   // stream to compressed file
    HCTree tree;                              // encoding tree
    vector<unsigned int> freqs(256);          // ascii frequency array
    vector<unsigned int>::iterator itr = freqs.begin();  // to iterate freqs
    vector<unsigned int>::iterator end = freqs.end();    // to iterate freqs
    char c;  // character to read/write stream

    cout << "File status: " << uncompressedFile.is_open() << endl;

    // Getting ascii frequency from file
    while (uncompressedFile.is_open()) {
        uncompressedFile.get(c);
        printf("%c\n", c);
        freqs[c]++;
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
    uncompressedFile.open("inFileName");
    byte b;

    // Read each character from initial file, encoding, and writing to new file
    while (uncompressedFile.is_open()) {
        uncompressedFile.get(c);
        b = c;
        tree.encode(b, compressedFile);
    }

    // Closing the file streams
    compressedFile.close();
    uncompressedFile.close();
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(const string& inFileName, const string& outFileName) {}

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