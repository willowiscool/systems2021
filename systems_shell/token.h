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
};

// null-terminated array
struct token* parseInput(char* input);
#endif // TOKEN_H_
