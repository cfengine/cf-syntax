#include <stdbool.h>  // bool
#include <stdio.h>    // FILE

typedef struct PolicyFile {
    size_t bodies;
} PolicyFile;

typedef struct ParserState {
    size_t errors;
    PolicyFile policy;
} ParserState;

PolicyFile *ParseFileStream(FILE *input_file);
PolicyFile *ParseFile(const char *path);

extern ParserState parser_state;

ParserState *NewParser();
PolicyFile *CloseParser();
