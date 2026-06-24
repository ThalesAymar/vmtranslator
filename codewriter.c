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
		fprintf(w->out, "@%d	//push %s %d\n", value, segment, value);
		fprintf(w->out,"D=A\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"M=D\n");
        fprintf(w->out,"@SP\n");
        fprintf(w->out,"M=M+1\n");
	}
	
	else if(strcmp(segment,"temp") == 0 || strcmp(segment,"pointer") == 0){
		fprintf(w->out,"@%d		//push %s %d\n", getBaseAddress(segment)+value, segment, value);
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
        fprintf(w->out,"@%d		//push %s %d\n", getBaseAddress(segment), segment, value);
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
        fprintf(w->out,"@SP		// pop %s %d\n", segment, value);
        fprintf(w->out,"M=M-1\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%d\n", getBaseAddress(segment) + value);
        fprintf(w->out,"M=D\n");
    }
    else if(strcmp(segment,"static") == 0){
    	fprintf(w->out,"@SP		// pop %s %d\n", segment, value);
        fprintf(w->out,"M=M-1\n");
        fprintf(w->out,"A=M\n");
        fprintf(w->out,"D=M\n");
        fprintf(w->out,"@%s.%d\n", w->fileName, value);
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
    else if (strcmp(segment,"not") == 0){ writeArithmeticNot(w);}
    else if (strcmp(segment,"neg") == 0){ writeArithmeticNeg(w);}
}





void writeArithmeticAdd(CodeWriter* w){
	fprintf(w->out,"@SP		// add\n");
    fprintf(w->out,"M=M-1\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=D+M\n");
}




void writeArithmeticSub(CodeWriter* w){
	fprintf(w->out,"@SP		// sub\n");
    fprintf(w->out,"M=M-1\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=M-D\n");
}




void writeArithmeticAnd(CodeWriter* w){
	fprintf(w->out,"@SP		// and\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=D&M\n");
}

void writeArithmeticOr(CodeWriter* w){
	fprintf(w->out,"@SP		// or\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=D|M\n");
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

void writeArithmeticNot(CodeWriter* w){
	fprintf(w->out,"@SP		// not\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=!M\n");
}


void writeArithmeticNeg(CodeWriter* w){
	fprintf(w->out,"@SP		// neg\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"A=A-1\n");
    fprintf(w->out,"M=-M\n");
}



void writeLabel(char* label, CodeWriter* w){
	fprintf(w->out,"(%s$%s)\n", w->fileName, label);
}


void writeGoto(char* label, CodeWriter* w){
	fprintf(w->out,"@%s$%s\n", w->fileName, label);
	fprintf(w->out,"0;JMP\n");
}


void writeIf(char* label, CodeWriter* w){
	fprintf(w->out,"@SP\n");
	fprintf(w->out,"AM=M-1\n");
	fprintf(w->out,"D=M\n");
	fprintf(w->out,"@%s$%s\n", w->fileName, label);
	fprintf(w->out,"D;JNE\n");
}



void  writeFunction(char* funcName , int nLocals, CodeWriter* w) {
    
    
    char loopLabel[64];
    sprintf(loopLabel, "%s_INIT_LOCALS_LOOP", funcName);

    char loopEndLabel[64];
    sprintf(loopEndLabel, "%s_INIT_LOCALS_END", funcName);
    
    
    
    fprintf(w->out,"(%s)	// initializa local variables\n", funcName);
    fprintf(w->out,"@%d\n", nLocals);
    fprintf(w->out,"D=A\n");
    fprintf(w->out,"@R13\n"); 
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"(%s)\n", loopLabel);
    
    fprintf(w->out,"@R13\n");
    fprintf(w->out,"D=M\n");

    fprintf(w->out,"@%s\n", loopEndLabel);
    fprintf(w->out,"D;JEQ\n");
    fprintf(w->out,"@0\n");
    fprintf(w->out,"D=A\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=M+1\n");
    fprintf(w->out,"@R13\n");
    fprintf(w->out,"MD=M-1\n");
    fprintf(w->out,"@%s\n", loopLabel);
    fprintf(w->out,"0;JMP\n");
    fprintf(w->out,"(%s)\n", loopEndLabel);
}




void writeCall(char* funcName , int nArgs, CodeWriter* w){
	
	char returnLabel[64];
	sprintf(returnLabel, "%s$ret.%d", funcName, w->labelCount++);
	
	
    fprintf(w->out,"@%s	//write call\n", returnLabel); 
    fprintf(w->out,"D=A\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=M+1\n");
    
    
    fprintf(w->out,"@LCL\n");
    fprintf(w->out,"@D=M\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@A=M\n");
    fprintf(w->out,"@M=D\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@M=M+1\n");
    
    fprintf(w->out,"@ARG\n");
    fprintf(w->out,"@D=M\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@A=M\n");
    fprintf(w->out,"@M=D\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@M=M+1\n");
    
    fprintf(w->out,"@THIS\n");
    fprintf(w->out,"@D=M\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@A=M\n");
    fprintf(w->out,"@M=D\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@M=M+1\n");
    
    fprintf(w->out,"@THAT\n");
    fprintf(w->out,"@D=M\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@A=M\n");
    fprintf(w->out,"@M=D\n");
    fprintf(w->out,"@@SP\n");
    fprintf(w->out,"@M=M+1\n");

    
    fprintf(w->out,"@%d\n", nArgs); 
    fprintf(w->out,"D=A\n");
    fprintf(w->out,"@5\n");
    fprintf(w->out,"D=D+A\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"D=M-D\n");
    fprintf(w->out,"@ARG\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@LCL\n");
    fprintf(w->out,"M=D\n");
    
    writeGoto(funcName, w);
    
    fprintf(w->out,"(%s)\n", returnLabel); 
}



void  writeReturn(CodeWriter* w) {
    
    fprintf(w->out,"@LCL  //return\n"); 
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@R13\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@5\n");
    fprintf(w->out,"A=D-A\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@R14\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@ARG\n");
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"D=A\n");
    fprintf(w->out,"@SP\n");
    fprintf(w->out,"M=D+1\n");
    fprintf(w->out,"@R13\n"); 
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@THAT\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@R13\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@THIS\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@R13\n"); 
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@ARG\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@R13\n");
    fprintf(w->out,"AM=M-1\n");
    fprintf(w->out,"D=M\n");
    fprintf(w->out,"@LCL\n");
    fprintf(w->out,"M=D\n");
    fprintf(w->out,"@R14\n"); 
    fprintf(w->out,"A=M\n");
    fprintf(w->out,"0;JMP\n");
    
}



void  writeInit(CodeWriter* w) {
    fprintf(w->out,"@256");
    fprintf(w->out,"D=A");
    fprintf(w->out,"@SP");
    fprintf(w->out,"M=D");
    writeCall("Sys.init", 0, w);
}





void makeOutputName(const char* input, char* output){
    strcpy(output, input);

    char* dot = strrchr(output, '.');

    if(dot != NULL){
        strcpy(dot, ".asm");
    }
}


void setFileName(CodeWriter* w, const char* path) {
    const char* slash1 = strrchr(path, '/');
    const char* slash2 = strrchr(path, '\\');

    const char* base = path;

    if (slash1 != NULL && slash1 > base) base = slash1 + 1;
    if (slash2 != NULL && slash2 > base) base = slash2 + 1;

    strncpy(w->fileName, base, sizeof(w->fileName) - 1);
    w->fileName[sizeof(w->fileName) - 1] = '\0';

    char* dot = strrchr(w->fileName, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
}
