#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codewriter.h"




int getBaseAdress(char* segment){
	
	if(strcmp(segment,"local") == 0){
		return 1;
	}
	if(strcmp(segment,"argument") == 0){
		return 2;
	}
	if(strcmp(segment,"this") == 0 || strcmp(segment,"pointer") == 0){
		return 3;
	}
	if(strcmp(segment,"that") == 0){
		return 4;
	}
	if(strcmp(segment,"temp") == 0){
		return 5;
	}
}




void writePush(char* segment, int value, CodeWriter* w ){
	if(strcmp(segment,"constant") == 0){
		fprintf(w->out, "@%d		//push %s %d\n", value, segment, value);
		fprintf(w->out,"D=A\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"M=D\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"M=M+1\n");
	}
	
	else if(strcmp(segment,"temp") == 0 || strcmp(segment,"pointer") == 0){
		fprintf(w->out,"@%d			//push static %d\n", getBaseAdress(segment)+value, value);
    	fprintf(w->out,"D=M\n");
    	fprintf(w->out,"@SP\n");
    	fprintf(w->out,"A=M\n");
    	fprintf(w->out,"M=D\n");
    	fprintf(w->out,"@SP\n");
    	fprintf(w->out,"M=M+1\n");
	}
	else if(strcmp(segment,"static") == 0){
		fprintf(w->out,"@%s.%d\n", w->fileName, value);
    	fprintf(w->out,"D=M\n");
    	fprintf(w->out,"@SP\n");
    	fprintf(w->out,"A=M\n");
    	fprintf(w->out,"M=D\n");
    	fprintf(w->out,"@SP\n");
    	fprintf(w->out,"M=M+1\n");
	}
	
	else{
        fprintf(w->out,"@%d			//push %s %d\n", getBaseAdress(segment), segment, value);
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%d\n", value);
        fprintf(w->out,"A=D+A\n");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"M=D\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"M=M+1\n");
    }

}



void writePop(char* segment, int value, CodeWriter* w ){
	if(strcmp(segment,"temp") == 0 || strcmp(segment,"pointer") == 0){
        fprintf(w->out,"@SP			// pop %s %d\n", segment, value);
        fprintf(w->out,"M=M-1\n");
        fprintf(w->out,"A=Mv");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%d\n", getBaseAddress(segment) + value);
        fprintf(w->out,"M=D\n");
    }
    else if(strcmp(segment,"static") == 0){
    	fprintf(w->out,"@SP			// pop %s %d\n", segment, value);
        fprintf(w->out,"M=M-1\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%d\n", getBaseAddress(segment) + value);
        fprintf(w->out,"M=D\n");
	}
	
	else{
		fprintf(w->out,"@%d		// pop %s %d\n", getBaseAddress(segment), segment, value);
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%d\n", value);
        fprintf(w->out,"D=D+A\n");
        fprintf(w->out,"@R13\n");
        fprintf(w->out,"M=D\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"M=M-1\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@R13\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"M=D\n");
	}
}



void writeArithmetic(char* segment, CodeWriter* w){}
