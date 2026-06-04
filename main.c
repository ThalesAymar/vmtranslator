#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int main (int argc, char *argv[]) {
	FILE *arq = fopen("teste.txt", "r");
	
	if(arq == NULL){
        printf("Erro ao abrir arquivo\n");
        return 1;
    }
	
	Parser p;
	p.currentToken = malloc(32);
	
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
    			printf("codewriterPush(arg1,arg2);\n");
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
