#ifndef CODEWRITER_H
#define CODEWRITER_H

typedef struct {
    FILE* out;
    char fileName[256];
} CodeWriter;

void writePush(char* segment, int value, CodeWriter* w);

void writePop(char* segment, int value, CodeWriter* w );

void writeArithmetic(char* segment, CodeWriter* w);

int getBaseAddress(char* segment);


#endif
