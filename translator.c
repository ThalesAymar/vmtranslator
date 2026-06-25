#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "translator.h"
#include "parser.h"
#include "codewriter.h"

int isVmFile(const char* filename) {
    const char* dot = strrchr(filename, '.');
    if (dot == NULL) return 0;
    return strcmp(dot, ".vm") == 0;
}

void translateFile(const char* filePath, CodeWriter* w) {
    FILE* arq = fopen(filePath, "r");
    if (!arq) {
        printf("Erro ao abrir arquivo %s\n", filePath);
        exit(1);
    }

    setFileName(w, filePath);

    Parser p;
    char* arg_1;
    int arg_2;

    while (fgets(p.currentLine, sizeof(p.currentLine), arq)) {

        // remove comentário
        char* comment = strstr(p.currentLine, "//");
        if (comment) *comment = '\0';

        p.currentToken = strtok(p.currentLine, " \t\n");
        if (!p.currentToken) continue;

        CommandType type = getCommandType(p.currentToken);

        switch (type) {
            case PUSH:
                arg_1 = arg1(&p);
                arg_2 = arg2(&p);
                writePush(arg_1, arg_2, w);
                break;

            case POP:
                arg_1 = arg1(&p);
                arg_2 = arg2(&p);
                writePop(arg_1, arg_2, w);
                break;

            case ARITHMETIC:
                arg_1 = arg1(&p);
                writeArithmetic(arg_1, w);
                break;

            case LABEL:
                arg_1 = arg1(&p);
                writeLabel(arg_1, w);
                break;

            case GOTO:
                arg_1 = arg1(&p);
                writeGoto(arg_1, w);
                break;

            case IF:
                arg_1 = arg1(&p);
                writeIf(arg_1, w);
                break;

            case FUNCTION:
                arg_1 = arg1(&p);
                arg_2 = arg2(&p);
                writeFunction(arg_1, arg_2, w);
                break;

            case CALL:
                arg_1 = arg1(&p);
                arg_2 = arg2(&p);
                writeCall(arg_1, arg_2, w);
                break;

            case RETURN:
                writeReturn(w);
                break;

            default:
                break;
        }
    }

    fclose(arq);
}

void translateDirectory(const char* dirPath, CodeWriter* w) {
    DIR* dir = opendir(dirPath);
    if (!dir) {
        printf("Erro ao abrir diretório %s\n", dirPath);
        exit(1);
    }

    struct dirent* entry;


    // Sys.init deve ser chamado apenas uma vez em modo directory
    writeInit(w);

    while ((entry = readdir(dir)) != NULL) {
        if (isVmFile(entry->d_name)) {

            char fullPath[512];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);

            translateFile(fullPath, w);
        }
    }

    closedir(dir);
}
