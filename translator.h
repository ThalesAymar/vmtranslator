#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "codewriter.h"

int isVmFile(const char* filename);

void translateFile(const char* filePath, CodeWriter* w);

void translateDirectory(const char* dirPath, CodeWriter* w);

#endif
