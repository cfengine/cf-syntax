#ifndef PARSE_LIB_H
#define PARSE_LIB_H

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

Parser* NewParser();
void DestroyParser(Parser *p);
PolicyFile *CloseParser(Parser *p);
PolicyFile *NewPolicyFile();
void DestroyPolicyFile(PolicyFile *policy_file);

#endif
