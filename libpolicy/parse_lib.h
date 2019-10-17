#ifndef PARSE_LIB_H
#define PARSE_LIB_H

#include <stdbool.h>  // bool
#include <stdio.h>    // FILE

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RST "\033[0m"

typedef struct PolicyFile {
    size_t bodies;
    char *name;
} PolicyFile;

typedef struct Parser {
    char *current_line;
    const char *current_token;
    size_t column_number;
    size_t line_number;
    size_t errors;
    PolicyFile *policy;
} Parser;

PolicyFile *ParseFileStream(FILE *input_file, const char *name);
bool LexFileStream(FILE *input_file, const char *name);
PolicyFile *ParseFile(const char *path);

Parser* NewParser(const char *filename);
void DestroyParser(Parser *p);
PolicyFile *CloseParser(Parser *p);
PolicyFile *NewPolicyFile(const char *filename);
void DestroyPolicyFile(PolicyFile *policy_file);

#endif
