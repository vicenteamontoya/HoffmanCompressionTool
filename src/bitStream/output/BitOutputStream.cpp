#include "BitOutputStream.hpp"

/**
 * TODO: Write the part of the buffer that was written by the user to the output
 * stream, and then clear the buffer to allow further use. You may use fill() to
 * zero-fill the buffer.
 *
 * Note: don’t flush the ostream here, as it is an extremely slow operation that
 * may cause a timeout.
 */
void BitOutputStream::flush() {
    int bytesToWrite;

    if (nbits % 8 == 0) {
        bytesToWrite = nbits / 8;
    } else {
        bytesToWrite = (nbits / 8) + 1;
    }

    // Loop to sort through buf byte by byte
    for (int i = 0; i < bytesToWrite; i++) {
        byte b = buf[i];  // byte to write to stream
        out << b;
        buf[i] = 0;  // resetting byte to 0
    }
}

/**
 * TODO: Write the least significant bit of the given int to the bit buffer.
 * Flushes the buffer first if it is full (which means all the bits in the
 * buffer have already been set). You may assume the given int is either 0 or 1.
 */
void BitOutputStream::writeBit(unsigned int i) {
    if (nbits == bufSize * 8) {
        flush();
        nbits = 0;
    }

    unsigned int index = nbits / 8;
    byte b = ((unsigned char)i) << (7 - (nbits % 8));
    buf[index] = buf[index] | b;
    nbits++;
}

BitOutputStream::~BitOutputStream() { delete[] buf; }