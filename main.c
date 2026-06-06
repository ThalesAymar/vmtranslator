#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "codewriter.h"

int main (int argc, char *argv[]) {
	
	FILE* arq = fopen("teste.vm", "r");
	FILE* out = fopen("test.asm", "w");
	
	if(arq == NULL){
        printf("Erro ao abrir arquivo\n");
        return 1;
    }
    
    if(out == NULL){
        printf("Erro ao criar arquivo\n");
        return 1;
    }
	
	Parser p;
	p.currentToken = malloc(32);
	
	CodeWriter w;
	w.out = out;
	strcpy(w.fileName, "teste");
	
	
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
    			//printf("codewriterPush(arg1,arg2);\n");
    			break;
    		case POP:
    			arg_1 = arg1(&p);
    			arg_2 = arg2(&p);
    			printf("codewriterPop(arg1,arg2);\n");
    			break;
    		default:
    			printf("Deu ruim\n");
		}
	}
	

	fclose(arq);
	
	printf("Deu bom\n");
	return 0;
}
