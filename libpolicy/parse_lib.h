#include <stdbool.h>  // bool
#include <stdio.h>    // FILE

typedef struct PolicyFile {
    size_t bodies;
} PolicyFile;

typedef struct Parser {
    size_t errors;
    PolicyFile *policy;
} Parser;

PolicyFile *ParseFileStream(FILE *input_file);
bool LexFileStream(FILE *input_file);
PolicyFile *ParseFile(const char *path);

extern Parser *parser;
Parser* NewParser();
PolicyFile *CloseParser();
PolicyFile *NewPolicyFile();
void DestroyPolicyFile(PolicyFile *policy_file);
