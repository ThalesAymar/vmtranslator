#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codewriter.h"




int getBaseAddress(char* segment){
	
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
		fprintf(w->out,"@%d			//push static %d\n", getBaseAddress(segment)+value, value);
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
        fprintf(w->out,"@%d			//push %s %d\n", getBaseAddress(segment), segment, value);
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



void writeArithmetic(char* segment, CodeWriter* w){
    if (strcmp(segment,"add") == 0){      writeArithmeticAdd(w);}
    else if (strcmp(segment,"sub") == 0){ writeArithmeticSub(w);}
    else if (strcmp(segment,"and") == 0){ writeArithmeticAnd(w);}
    else if (strcmp(segment,"or") == 0){  writeArithmeticOr(w);}
	else if (strcmp(segment,"lt") == 0){  writeArithmeticLt(w);}
    else if (strcmp(segment,"gt") == 0){  writeArithmeticGt(w);}
    else if (strcmp(segment,"eq") == 0){  writeArithmeticEq(w);}
}




void writeArithmeticAdd(CodeWriter* w){
	fprintf(w->out,"@SP		// add");
    fprintf(w->out,"M=M-1");
    fprintf(w->out,"A=M");
    fprintf(w->out,"D=M");
    fprintf(w->out,"A=A-1");
    fprintf(w->out,"M=D+M");
}



void writeArithmeticSub(CodeWriter* w){
	fprintf(w->out,"@SP		// sub");
    fprintf(w->out,"M=M-1");
    fprintf(w->out,"A=M");
    fprintf(w->out,"D=M");
    fprintf(w->out,"A=A-1");
    fprintf(w->out,"M=M-D");
}


void writeArithmeticAnd(CodeWriter* w){
	fprintf(w->out,"@SP		// and");
    fprintf(w->out,"AM=M-1");
    fprintf(w->out,"D=M");
    fprintf(w->out,"A=A-1");
    fprintf(w->out,"M=D&M");
}


void writeArithmeticOr(CodeWriter* w){
	fprintf(w->out,"@SP		// or");
    fprintf(w->out,"AM=M-1");
    fprintf(w->out,"D=M");
    fprintf(w->out,"A=A-1");
    fprintf(w->out,"M=D|M");
}



void writeArithmeticLt(CodeWriter* w){

    char labelTrue[64];
    sprintf(labelTrue, "LT_TRUE_%s_%d", w->fileName, w->labelCount);

    char labelEnd[64];
    sprintf(labelEnd, "LT_END_%s_%d", w->fileName, w->labelCount);
    

    fprintf(w->out,"@SP		// lt\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M-D\n");
    fprintf(w->out,"@%s\n", labelTrue);
    fprintf(w->out,"D;JLT\n");
    fprintf(w->out,"D=0\n");
    fprintf(w->out,"@%s\n", labelEnd);
    fprintf(w->out,"0;JMP\n");
    fprintf(w->out,"(%s)\n", labelTrue);
    fprintf(w->out,"D=-1\n");
    fprintf(w->out,"(%s)\n", labelEnd);
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=M+1\n");

    w->labelCount++;
}

void writeArithmeticGt(CodeWriter* w){

    char labelTrue[64];
    sprintf(labelTrue, "GT_TRUE_%s_%d", w->fileName, w->labelCount);

    char labelEnd[64];
    sprintf(labelEnd, "GT_END_%s_%d", w->fileName, w->labelCount);

    fprintf(w->out,"@SP		// gt\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M-D\n");
    fprintf(w->out,"@%s\n", labelTrue);
    fprintf(w->out,"D;JGT\n");
    fprintf(w->out,"D=0\n");
    fprintf(w->out,"@%s\n", labelEnd);
    fprintf(w->out,"0;JMP\n");
    fprintf(w->out,"(%s)\n", labelTrue);
    fprintf(w->out,"D=-1\n");
    fprintf(w->out,"(%s)\n", labelEnd);
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=M+1\n");

    w->labelCount++;
}

void writeArithmeticEq(CodeWriter* w){

    char labelTrue[64];
    sprintf(labelTrue, "EQ_TRUE_%s_%d", w->fileName, w->labelCount);

    char labelEnd[64];
    sprintf(labelEnd, "EQ_END_%s_%d", w->fileName, w->labelCount);

    fprintf(w->out,"@SP		// eq\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");       
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M-D\n");
    fprintf(w->out,"@%s\n", labelTrue);
    fprintf(w->out,"D;JEQ\n");
    fprintf(w->out,"D=0\n");
    fprintf(w->out,"@%s\n", labelEnd);
    fprintf(w->out,"0;JMP\n");
    fprintf(w->out,"(%s)\n", labelTrue);
    fprintf(w->out,"D=-1\n");
    fprintf(w->out,"(%s)\n", labelEnd);
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=M+1\n");

    w->labelCount++;
}
