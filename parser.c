#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"



Command commands[] = {
	{"push", PUSH},
	{"pop", POP},
	{"label", LABEL},
	{"goto", GOTO},
	{"if-goto", IF},
	{"function", FUNCTION},
	{"return", RETURN},
	{"call", CALL},
	{"add", ARITHMETIC},
	{"sub", ARITHMETIC},
	{"neg", ARITHMETIC},
	{"eq",  ARITHMETIC},
	{"gt",  ARITHMETIC},
	{"lt",  ARITHMETIC},
	{"and", ARITHMETIC},
	{"or",  ARITHMETIC},
	{"not", ARITHMETIC}
	
};



CommandType getCommandType(char* token){
	for(size_t i = 0; i < sizeof(commands)/sizeof(commands[0]); i++){
		if(strcmp(token, commands[i].name) == 0){
			return commands[i].type;
		}
	}
	
	
	printf("Comando invalido\n");
	exit(1);
}



	
	
void advance(Parser* p){ 
    p->currentToken = strtok(NULL, " \t\n");
}




char* arg1(Parser* p){
	if (getCommandType(p->currentToken) == ARITHMETIC){
		return p->currentToken;
	}
	else{
		advance(p);
		return p->currentToken;
	}
}

int arg2(Parser* p){  //só é chamado se não for operação aritmetica (obrigatoramente tem 2 argumentos)
	advance(p);
	return atoi(p->currentToken);
}

