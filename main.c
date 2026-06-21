#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "codewriter.h"

int main (int argc, char *argv[]) {
	
	if(argc != 2){
        printf("Nao foram passados 2 argumentos");
        return 1;
    }

    FILE* arq = fopen(argv[1], "r");

    if(arq == NULL){
        printf("Erro ao abrir arquivo %s\n", argv[1]);
        return 1;
    }

    char outName[256];
    makeOutputName(argv[1], outName);

    FILE* out = fopen(outName, "w");

    if(out == NULL){
        printf("Erro ao criar %s\n", outName);
        fclose(arq);
        return 1;
    }
    

    Parser p;
    CodeWriter w;
    
    w.out = out;
    setFileName(&w, argv[1]);
	w.labelCount = 0;
	
	
	char* arg_1;
    int arg_2;
    
    
	while(fgets(p.currentLine, sizeof(p.currentLine), arq) != NULL){
		
		char* comment = strstr(p.currentLine, "//");

		if(comment != NULL){
    		*comment = '\0';
		}
		
		
		p.currentToken = strtok(p.currentLine, " \t\n");
		
		if(p.currentToken == NULL){
    		continue;
		}
		
		
		CommandType type = getCommandType(p.currentToken);
		
		
    	
    	switch(type){
    		case PUSH:
    			arg_1 = arg1(&p);
    			arg_2 = arg2(&p);
    			writePush(arg_1, arg_2, &w);
    			break;
    		case POP:
    			arg_1 = arg1(&p);
    			arg_2 = arg2(&p);
    			writePop(arg_1, arg_2, &w);
    			break;
    		case ARITHMETIC:
    			arg_1 = arg1(&p);
    			writeArithmetic(arg_1, &w);
			case LABEL:
    			arg_1 = arg1(&p);
    			writeLabel(arg_1, &w);
    			break;
    		case GOTO:
    			arg_1 = arg1(&p);
    			writeGoto(arg_1, &w);
    			break;
    		case IF:
    			arg_1 = arg1(&p);
    			writeIf(arg_1, &w);
    			break;
    		default:
    			break;
		}
	}
	

	fclose(arq);
	
	printf("Arquivo %s traduzido com sucesso!\n", argv[1]);
	return 0;
}
