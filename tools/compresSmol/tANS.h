#ifndef TANS_H
#define TANS_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

struct DecodeCol {
    int state;
    unsigned char symbol;
    int y;
    int k;
};

struct EncodeHelper {
    unsigned char symbol;
    std::vector<int> states;
    std::vector<int> yVals;
    std::vector<int> kVals;
    std::vector<int> yPrimVals;
    int currentRepeat = 0;
    int currentIndex = 0;
};

struct EncodeSymbolData {
    unsigned char symbol;
    int nextState;
    int streamValue;
    int numBits;
};
struct EncodeCol {
    int state;
    std::vector<EncodeSymbolData> symbols;
};

struct EncodedData {
    int initialState;
    std::vector<unsigned char> bitStream;
};

std::vector<DecodeCol> createDecodingTable(std::vector<unsigned char> symbols, std::vector<int> frequencies);

std::vector<EncodeCol> createEncodingTable(std::vector<DecodeCol> decodeTable, std::vector<unsigned char> symbols);

EncodedData encodeData(std::vector<unsigned char> input, std::vector<EncodeCol> encodingTable);
std::vector<unsigned char> decodeData(EncodedData *data, std::vector<DecodeCol> decodeTable, int numChars);

std::vector<unsigned char> findSymbols(std::vector<unsigned char> input);
std::vector<int> countSymbols(std::vector<unsigned char> input, std::vector<unsigned char> symbols);
std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize);
std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize, bool fillZeros);
void sortCount(std::vector<int> *counts, std::vector<unsigned char> *symbols);

void printEncodeTable(std::vector<EncodeCol> encodeTable, std::vector<unsigned char> symbols);
void printDecodeTable(std::vector<DecodeCol> decodeTable);
void printCounts(std::vector<int> count);
void printSymbols(std::vector<unsigned char> symbols);

std::vector<unsigned char> readFileAsNibbles(std::string filePath);
int getFileSize(std::string filePath);
bool areVectorsEqual(std::vector<unsigned char> input, std::vector<unsigned char> output);
int encodeSingleSymbol(EncodeCol currEncodeCol, unsigned char symbol, std::vector<unsigned int> *bitstream);

std::string getStringFromUCvec(std::vector<unsigned char> input);
std::vector<unsigned char> getUCvecFromString(std::string input);
unsigned char getUCfromChar(char input);
char getCharFromUC(unsigned char input);


#endif
