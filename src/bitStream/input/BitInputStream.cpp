#include "BitInputStream.hpp"

/**
 * TODO: Fill the buffer of size bufSize from the input stream.
 */
void BitInputStream::fill() { in.read(buf, bufSize); }

/**
 * TODO: Return true when the istream tried to read more bytes than the
 * remaining bytes in the file and the buffer was also fully read. Otherwise,
 * return false.
 *
 * Note that eof() is different from this function. eof() in most other istreams
 * is only true when we are trying to read past the end of the file. Whereas
 * this function should be only true when we are at the end of the file, but
 * have not read past it yet. For example:
 *
 * // some code to read all the data in the file
 * atEndOfFile(); // atEndOfFile() = true. But eof() is still false for now.
 * readBit();     // trying to read past the end of the file
 * eof();         // eof is true now
 */
bool BitInputStream::atEndOfFile() {
    int ingcount = in.gcount();
    bool bufferFullyRead =
        ingcount == 0 || ((ingcount != 0) && (nbits == ingcount * 8));
    if (bufferFullyRead && !in) {
        return true;
    }

    return false;
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/**
 * TODO: Read the next bit from the bit buffer. Fills the buffer from the input
 * stream if all the bits have already been read. It should return 0 if the bit
 * read is 0, and return 1 if the bit read is 1.
 *
 * Before trying to read a bit from the buffer, if atEndOFfile() is true, set
 * the eofBit and directly return 0 (You can return any number here, since after
 * setting eofBit to true, eof() will return true and the program won’t be using
 * the returned value from readBit).
 */
unsigned int BitInputStream::readBit() {
    eofBit = 0;
    if (nbits == bufSize * 8 || nbits == 0) {
        fill();
        nbits = 0;
    }

    if (atEndOfFile()) {
        eofBit = 1;
        return 0;
    }

    // Extracting byte from buf
    unsigned char c = buf[nbits / 8];

    // Getting index position and extracting bit from bytes
    unsigned char index = nbits % 8;
    c = (c >> (7 - index)) & 1;

    nbits++;

    return c;
}

BitInputStream::~BitInputStream() { delete[] buf; }
