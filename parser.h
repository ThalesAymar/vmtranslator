typedef struct{
	int numLine;
	char currentLine[256];
	char* currentToken;
} Parser;

typedef enum{
	ARITHMETIC,
    PUSH,
    POP,
    LABEL,
    GOTO,
    IF,
    FUNCTION,
    RETURN,
    CALL
} CommandType;

typedef struct{
	char* name;
	CommandType type;
} Command;


CommandType getCommandType(char* token);

void advance(Parser* p);

char* arg1(Parser* p);

int arg2(Parser* p);
