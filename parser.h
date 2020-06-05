#ifndef PARSER_H
#define PARSER_H

typedef void* yyscan_t;

#include <stdio.h>

class parse_state {
public:
    parse_state *included_from;
    parse_state *includes;
    parse_state *next;
    static parse_state *root;
    yyscan_t scaninfo;
    char *filename;
    int prev_lineno;
    parse_state(parse_state *included_from,
                char *filename,
                FILE *file );
    void print(int level);
};

#endif
