#include "qstring.h"
#include "parser.tab.hpp"
#include "lexer.h"
#include <stdio.h>
#include <string>

class file_info {
public:
    std::string filename;
    int prev_line;
    file_info *prev;
    file_info(const char *filename, int prev_line);
    ~file_info(void);
    static void print_include_hierarchy();
    static file_info *head;
};

file_info *file_info::head = NULL;

file_info::file_info(const char *filename, int prev_line){
    prev = head;
    head = this;
    file_info::filename = filename;
    file_info::prev_line = prev_line;
}

file_info::~file_info(void) {
    head = prev;
}

void file_info::print_include_hierarchy(void)
{
    file_info *fi = head;
    while(fi->prev){
        fprintf(stderr, "included from \"%s\" at line number %d.\n",
                fi->prev->filename.c_str(), fi->prev_line );
        fi = fi->prev;
    }
}

int parse_file(const char *filename, yyscan_t prev_scanner)
{
    int prev_line=0;
    file_info *prev_fi;
    if(prev_scanner){
        prev_line = yyget_lineno(prev_scanner);
        prev_fi = (file_info*)yyget_extra(prev_scanner);
    }
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("fopen");
        if(prev_scanner){
            fprintf(stderr, "Couldn't open include file:\"%s\"\n",
                    filename );
            fprintf(stderr, "Include on line %d of file \"%s\"\n",
                    prev_line, prev_fi->filename.c_str() );
        }else{
            fprintf(stderr, "Couldn't open source file:\"%s\"\n",
                    filename );
        }
        return 0;
    }
    yyscan_t new_scanner;
    file_info *fi = new file_info(filename, prev_line);
    yylex_init_extra((void*)fi, &new_scanner);
    yyrestart(file, new_scanner);
    int parse_result = yyparse(new_scanner);
    yylex_destroy(new_scanner);
    delete fi;
    fclose(file);
    if(parse_result==0){
        return 1;
    }else{
        return 0;
    }
}

void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, const char* msg)
{
    file_info *fi = (file_info*)yyget_extra(scanner);
    fprintf(stderr, "\"%s\"[line %d:column %d]: %s\n",
            fi->filename.c_str(),
            yyllocp->first_line, yyllocp->first_column, msg);
    file_info::print_include_hierarchy();
}




int main(int argc, char **argv)
{
    if(argc!=2){
        printf("Incorrect number of arguments.\n");
        printf("Usage: %s <input file>\n",argv[0]);
        return 1;
    }
    
    if(parse_file(argv[1], NULL)){
        RenderText();
    }
    
    return 0;
}
