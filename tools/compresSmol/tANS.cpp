#include "tANS.h"

std::vector<DecodeCol> createDecodingTable(std::vector<unsigned char> symbols, std::vector<int> frequencies)
{
    std::vector<DecodeCol> decodeTable;

    int totalFreq = 0;
    std::vector<int> yValueCounting = frequencies;

    for (int freq : frequencies)
        totalFreq += freq;

    int indexCounter = 0;
    //  Create a table with size equal to the total frequencies
    for (int i = 0; i < totalFreq; i++)
    {
        DecodeCol currCol;
        int currentSymbolIndex = indexCounter % symbols.size();

        //  Set the state to total frequencies + current index
        currCol.state = totalFreq + i;
        //  Set the symbol to the next valid symbol
        currCol.symbol = symbols[currentSymbolIndex];
        //  Set the y-value to the frequency of the symbol + number of times used
        currCol.y = yValueCounting[currentSymbolIndex];
        //  Calculate the k-value, where k is how many times y needs to be doubled
        //  to be larger or equal to the total frequency count
        int k = 0;
        int yDoubledk = currCol.y;
        while (yDoubledk < totalFreq)
        {
            k++;
            yDoubledk = currCol.y << k;
        }
        currCol.k = k;
        decodeTable.push_back(currCol);

        //  Update the y-value counter
        yValueCounting[currentSymbolIndex]++;
        //  Decrease the freq for the symbol of the current table entry
        frequencies[currentSymbolIndex]--;

        //  Find the next valid symbol to generate a table entry for
        indexCounter++;
        for (int j = 0; j < symbols.size(); j++)
        {
            if (frequencies[indexCounter % symbols.size()] != 0)
                break;
            else
                indexCounter++;
        }
    }
    return decodeTable;
}

std::vector<EncodeCol> createEncodingTable(std::vector<DecodeCol> decodeTable, std::vector<unsigned char> symbols)
{
    std::vector<EncodeCol> encodeTable(decodeTable.size());
    for (int i = 0; i < decodeTable.size(); i++)
    {
        encodeTable[i].state = decodeTable.size() + i;
    }
    std::vector<EncodeHelper> encodeHelpers;

    //  Build the encoding helper tables, one per symbol
    for (unsigned char symbol : symbols)
    {
        EncodeHelper newHelper;
        newHelper.symbol = symbol;
        encodeHelpers.push_back(newHelper);
    }

    for (DecodeCol dCol : decodeTable)
    {
        unsigned char currSymbol = dCol.symbol;
        int symbolIndex = 0;
        while (encodeHelpers[symbolIndex].symbol != currSymbol)
        {
            symbolIndex++;
        }
        encodeHelpers[symbolIndex].states.push_back(dCol.state);
        encodeHelpers[symbolIndex].yVals.push_back(dCol.y);
        encodeHelpers[symbolIndex].kVals.push_back(dCol.k);
        encodeHelpers[symbolIndex].yPrimVals.push_back(dCol.y << dCol.k);
    }
    //  Build the encoding table, one column per state in decoding table
    for (int helperNum = 0; helperNum < encodeHelpers.size(); helperNum++)
    {
        for (int i = 0; i < encodeHelpers[helperNum].states.size(); i++)
        {
            EncodeSymbolData currSymbol;
            currSymbol.symbol = encodeHelpers[helperNum].symbol;
            currSymbol.streamValue = 0;
            currSymbol.numBits = encodeHelpers[helperNum].kVals[i];
            currSymbol.nextState = encodeHelpers[helperNum].states[i];
            int currIndex = encodeHelpers[helperNum].yPrimVals[i]-encodeTable.size();
            encodeTable[currIndex].symbols.push_back(currSymbol);
            int limit = std::pow(2, currSymbol.numBits);
            for (int j = 1; j < limit; j++)
            {
                currSymbol.streamValue++;
                encodeTable[currIndex + j].symbols.push_back(currSymbol);
            }
        }
    }

    return encodeTable;
}

EncodedData encodeData(std::vector<unsigned char> input, std::vector<EncodeCol> encodingTable)
{
    EncodedData data;
    int state;
    //  Reverse the input, since ANS operates in FILO mode
    std::vector<unsigned char> reverseInput(input.size());
    for (int i = 0; i < input.size(); i++)
    {
        reverseInput[input.size() - 1 - i] = input[i];
    }
    //  Encode the string
    int currentOffsetState = 0;
    for (int i = 0; i < reverseInput.size(); i++)
    {
        unsigned char currChar = reverseInput[i];
        //  Find correct encoding instruction for the symbol
        for (EncodeSymbolData eSymbols : encodingTable[currentOffsetState].symbols)
        {
            if (eSymbols.symbol == currChar)
            {
                //  Encode the symbol
                state = eSymbols.nextState;
                if (i == 0)
                    break;
                int streamValue = eSymbols.streamValue;
                int compVal = 1;
                for (int numBit = eSymbols.numBits-1; numBit >= 0; numBit--)
                {
                    unsigned char currBit = (streamValue >> numBit) & compVal;
                    data.bitStream.push_back(currBit);
                }
                break;
            }
        }
        currentOffsetState = state - encodingTable.size();
    }
    data.initialState = state;

    return data;
}

int encodeSingleSymbol(EncodeCol currEncodeCol, unsigned char symbol, std::vector<unsigned int> *bitstream)
{
    int newState;
    for (EncodeSymbolData eSymbols : currEncodeCol.symbols)
    {
        if (eSymbols.symbol == symbol)
        {
            newState = eSymbols.nextState;
            int streamValue = eSymbols.streamValue;
            int compVal = 1;
            for (int numBit = eSymbols.numBits-1; numBit >= 0; numBit--)
            {
                unsigned char currBit = (streamValue >> numBit) & compVal;
                bitstream->push_back(currBit);
            }
            break;
        }
    }
    return newState;
}

std::vector<unsigned char> decodeData(EncodedData *data, std::vector<DecodeCol> decodeTable, int numChars)
{
    unsigned int tableSize = decodeTable.size();
    unsigned int state = data->initialState;
    std::vector<unsigned char> returnVec(numChars);
    returnVec[0] = decodeTable[state-tableSize].symbol;
    for (int i = 1; i < numChars; i++)
    {
        unsigned int currY = decodeTable[state-tableSize].y;
        unsigned int currK = decodeTable[state-tableSize].k;
        unsigned int streamValue = 0;
        //  Horrible way of reading a value from the bitstream
        for (int j = 0; j < currK; j++)
        {
            unsigned int tempVal;
            bool val = data->bitStream.back();
            data->bitStream.pop_back();
            if (val)
                tempVal = 1;
            else
                tempVal = 0;
            streamValue += (tempVal << j);
        }
        //  Calculate the next state and retrieve the symbol for that state
        state = (currY << currK) + streamValue;
        returnVec[i] = decodeTable[state-tableSize].symbol;
    }
    data->initialState = state;
    return returnVec;
}

std::vector<unsigned char> findSymbols(std::vector<unsigned char> input)
{
    std::vector<unsigned char> symbols = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15};
    return symbols;
}

std::vector<int> countSymbols(std::vector<unsigned char> input, std::vector<unsigned char> symbols)
{
    std::vector<int> symbolCounts(symbols.size());
    for (int i = 0; i < input.size(); i++)
    {
        unsigned char currChar = input[i];
        for (int j = 0; j < symbols.size(); j++)
        {
            if (currChar == symbols[j])
            {
                symbolCounts[j]++;
                break;
            }
        }
    }
    //  Setting found counts to be a minimum of 1
    for (int i = 0; i < symbols.size(); i++)
    {
        if (symbolCounts[i] == 0)
            symbolCounts[i] = 1;
    }
    return symbolCounts;
}

std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize)
{
    return normalizeCounts(counts, tableSize, false);
}
std::vector<int> normalizeCounts(std::vector<int> counts, int tableSize, bool fillZeros)
{
    int totalCount = 0;
    for (int count : counts)
    {
        totalCount += count;
    }
    if (totalCount < tableSize)
    {
        for (int i = 0; i < counts.size(); i++)
        {
            counts[i] *= (int)ceil((float)tableSize/(float)totalCount);
        }
    }
    if (fillZeros)
        for (int i = 0; i < counts.size(); i++)
            if (counts[i] == 0)
                counts[i] = 1;

    totalCount = 0;
    for (int count : counts)
    {
        totalCount += count;
    }

    bool shouldContinue = true;
    std::vector<int> normCounts(counts.size());
    while (shouldContinue)
    {
        int smallestCount;
        int smallestIndex;
        int firstIndex;
        int largestCount = 0;
        for (int i = 0; i < counts.size(); i++)
        {
            if (counts[i] != 0)
            {
                firstIndex = i;
                smallestIndex = i;
                smallestCount = counts[i];
                break;
            }
        }
        for (int i = firstIndex; i < counts.size(); i++)
        {
            if (counts[i] < smallestCount && counts[i] != 0)
            {
                smallestCount = counts[i];
                smallestIndex = i;
            }
            if (counts[i] > largestCount)
            {
                largestCount = counts[i];
            }
        }
        if (largestCount == 0)
        {
            shouldContinue = false;
        }
        else
        {
            float frac = float(smallestCount)/float(totalCount);
            int newCount = std::round(frac*float(tableSize));
            if (newCount == 0 and counts[smallestIndex] != 0)
            {
                newCount = 1;
            }
            normCounts[smallestIndex] = newCount;
            tableSize -= newCount;
            totalCount -= smallestCount;
            counts[smallestIndex] = 0;
        }
    }
    return normCounts;
}

void printEncodeTable(std::vector<EncodeCol> encodeTable, std::vector<unsigned char> symbols)
{
    printf("    x: ");
    for (int i = 0; i < encodeTable.size(); i++)
    {
        printf("%3zu ", i+encodeTable.size());
    }
    printf("\n");
    for (int i = 0; i < symbols.size(); i++)
    {
        printf("%3hhu s: ", symbols[i]);
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%3i ", encodeTable[k].symbols[i].nextState);
        }
        printf("\n");
        printf("%3hhu b: ", symbols[i]);
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%3i ", encodeTable[k].symbols[i].streamValue);
        }
        printf("\n");
        printf("%3hhu k: ", symbols[i]);
        for (int k = 0; k < encodeTable.size(); k++)
        {
            printf("%3i ", encodeTable[k].symbols[i].numBits);
        }
        printf("\n");
    }
}

void printDecodeTable(std::vector<DecodeCol> decodeTable)
{
    printf("State: ");
    for (DecodeCol currCol : decodeTable)
        printf("%3i ", currCol.state);
    printf("\n");
    printf("  Sym: ");
    for (DecodeCol currCol : decodeTable)
        printf("%3hhu ", currCol.symbol);
    printf("\n");
    printf("    Y: ");
    for (DecodeCol currCol : decodeTable)
        printf("%3i ", currCol.y);
    printf("\n");
    printf("    K: ");
    for (DecodeCol currCol : decodeTable)
        printf("%3i ", currCol.k);
    printf("\n");
}

std::vector<unsigned char> readFileAsNibbles(std::string filePath)
{
    std::vector<unsigned char> returnVec;
    std::ifstream iStream;
    iStream.open(filePath.c_str(), std::ios::binary);
    if (!iStream.is_open())
    {
        fprintf(stderr, "Error: Couldn't open %s for reading as nibbles\n", filePath.c_str());
        return returnVec;
    }
    iStream.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize size = iStream.gcount();
    iStream.clear();
    iStream.seekg( 0, std::ios_base::beg );
    std::vector<unsigned char> ucVec(size);
    iStream.read((char*)(&ucVec[0]), size);
    iStream.close();
    for (unsigned char byte : ucVec)
    {
        returnVec.push_back(byte >> 4);
        returnVec.push_back(byte & 0b00001111);
    }
    return returnVec;
}

int getFileSize(std::string filePath)
{
    std::ifstream iStream;
    iStream.open(filePath.c_str(), std::ios::binary);
    if (!iStream.is_open())
    {
        fprintf(stderr, "Error: Couldn't open %s for reading file size\n", filePath.c_str());
        return 0;
    }
    iStream.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize size = iStream.gcount();
    int returnSize = size;
    iStream.clear();
    iStream.seekg( 0, std::ios_base::beg );
    iStream.close();
    return returnSize;
}

void printCounts(std::vector<int> counts)
{
    int totalCount = 0;
    for (int count : counts)
    {
        printf("%4i ", count);
        totalCount += count;
    }
    printf("%4i\n", totalCount);
}

void printSymbols(std::vector<unsigned char> symbols)
{
    for (int symbol : symbols)
        printf("%4hhu ", symbol);
    printf("\n");
}

void sortCount(std::vector<int> *counts, std::vector<unsigned char> *symbols)
{
    for (int i = 0; i < counts->size(); i++)
    {
        int currCount = (*counts)[i];
        int currChar = (*symbols)[i];
        for (int j = i; j < counts->size(); j++)
        {
            if ((*counts)[j] > currCount)
            {
                (*counts)[i] = (*counts)[j];
                (*symbols)[i] = (*symbols)[j];
                (*counts)[j] = currCount;
                (*symbols)[j] = currChar;
                currCount = (*counts)[i];
                currChar = (*symbols)[i];
            }
        }
    }
}

bool areVectorsEqual(std::vector<unsigned char> input, std::vector<unsigned char> output)
{
    if (input.size() != output.size())
        return false;
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] != output[i])
            return false;
    }
    return true;
}

std::string getStringFromUCvec(std::vector<unsigned char> input)
{
    std::stringstream ss;
    for (unsigned char uc : input)
        ss << getCharFromUC(uc);
    return ss.str();
}
std::vector<unsigned char> getUCvecFromString(std::string input)
{
    std::vector<unsigned char> ucVec(input.size());
    for (int i = 0; i < input.size(); i++)
        ucVec[i] = getUCfromChar(input[i]);
    return ucVec;
}

unsigned char getUCfromChar(char input)
{
    switch (input)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a':
            return 10;
        case 'b':
            return 11;
        case 'c':
            return 12;
        case 'd':
            return 13;
        case 'e':
            return 14;
        case 'f':
            return 15;
        default:
            return 255;
    }
}

char getCharFromUC(unsigned char input)
{
    switch (input)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'a';
        case 11:
            return 'b';
        case 12:
            return 'c';
        case 13:
            return 'd';
        case 14:
            return 'e';
        case 15:
            return 'f';
        default:
            return 'X';
    }
}

