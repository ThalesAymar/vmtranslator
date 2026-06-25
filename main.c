#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "translator.h"
#include "codewriter.h"

int isDirectory(const char* path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0){
		return 0;
	}
    return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Uso: vmtranslator <arquivo.vm | diretório>\n");
        return 1;
    }

    const char* inputPath = argv[1];

    char outName[256];
    

    CodeWriter w;
    w.labelCount = 0;
    w.currentFunction[0] = '\0';


    if (isDirectory(inputPath)) {
    	makeOutputNameDir(inputPath, outName);

    	FILE* out = fopen(outName, "w");
    	if (!out) {
        	printf("Erro ao criar arquivo de saida DIR %s\n", outName);
        	return 1;
    	}
    	
    	w.out = out;
        translateDirectory(inputPath, &w);
        fclose(out);
	}
		
	else{
        makeOutputName(inputPath, outName);

    	FILE* out = fopen(outName, "w");
    	if (!out) {
        	printf("Erro ao criar arquivo de saida SOLO %s\n", outName);
        	return 1;
    	}
    	
    	
    	w.out = out;
        translateFile(inputPath, &w);
        fclose(out);
    }


    printf("Traducao concluida: %s\n", outName);
    return 0;
}
