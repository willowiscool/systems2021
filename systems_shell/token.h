#ifndef TOKEN_H_
#define TOKEN_H_
// subject to change
enum tokenType {
	COMMAND,
	SEMICOLON,
	PIPE
};

struct token {
	enum tokenType type;
	char** command;
	// array length determined by type
	struct token** children;
	// for freeing
	char* beginningOfStrPtr;
	char* redirectTo;
	char* redirectFrom;
	// bool
	int append;
};

struct arg {
	char* str;
	struct arg* nextArg;
};

struct charll {
	char c;
	struct charll* nextChar;
};

// null-terminated array
struct token* parseInput(char* input);

void printToken(struct token* token, int level);
void printTabs(int level, char* str);
#endif // TOKEN_H_
