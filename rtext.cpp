#include "qstring.h"
#include "parser.tab.hpp"
#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <new>
#include <unistd.h>

parse_state *parse_state::root = NULL;

parse_state::parse_state(parse_state *included_from,
                         char *filename,
                         FILE *file ):
included_from(included_from),
includes(NULL),
next(NULL),
filename(strdup(filename))
{
    if(!filename){
        printf("Ran out of memory.\n");
        exit(EXIT_FAILURE);
    }

    if(included_from)
    {
        // get the line number of the include
        prev_lineno = yyget_lineno(included_from->scaninfo);
        
        // attach this to the includes list of the included_from
        splice<parse_state>(&included_from->includes,this);
    }else{
        // attach this to the root list
        splice<parse_state>(&root,this);
    }
    
    // initialize the scanner parse_state
    if(yylex_init(&scaninfo)){
        if(errno==ENOMEM){
            printf("Ran out of memory.\n");
        }else if(errno==EINVAL){
            perror("yylex_init");
        }else{
            printf("yylex_init unknown error.\n");
        }
        exit(EXIT_FAILURE);
    }
    yyrestart(file, scaninfo);
}

void parse_state::print(int level){
    printf("%*.s",level*2,"");
    printf("\"%s\"\n",filename);
    if(includes) includes->print(level+1);
    if(next) next->print(level);
}

int parse_file(char *filename, parse_state *included_from)
{
    // open the file
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("fopen");
        if(included_from){
            fprintf(stderr, "Couldn't open include file:\"%s\"\n",
                    filename );
            int prev_line = yyget_lineno(included_from->scaninfo);
            fprintf(stderr, "Include on line %d of file \"%s\"\n",
                    prev_line, included_from->filename );
        }else{
            fprintf(stderr, "Couldn't open source file:\"%s\"\n",
                    filename );
        }
        return 0;
    }

    parse_state *ps = new parse_state(included_from, filename, file);
    
    int r = yyparse(ps);
    
    fclose(file);
    
    yylex_destroy(ps->scaninfo);
    
    if(r==0)
        return 1;
    else
        return 0;
}

void yyerror(YYLTYPE* yyllocp, parse_state *ps, const char* msg)
{
    fprintf(stderr, "\"%s\"[line %d:column %d]: %s\n",
            ps->filename,
            yyllocp->first_line, yyllocp->first_column, msg);
    parse_state* ps_inc = ps;
    while(ps_inc->included_from){
        fprintf(stderr, "Included from \"%s\" on line %d.\n",
                ps_inc->included_from->filename,
                ps_inc->prev_lineno);
        ps_inc=ps_inc->included_from;
    }
}

void no_memory(void)
{
    printf("Ran out of memory! Exiting...\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    std::set_new_handler(no_memory);

    bool print_includes = false;
    int getopt_out;
    while((getopt_out=getopt(argc, argv, "i"))!=-1){
        switch(getopt_out){
            case 'i':
                print_includes = true;
                break;
            case '?':
                exit(EXIT_FAILURE);
        }
    }
    
    if(optind==argc){
        printf("No input files.\n");
        exit(EXIT_FAILURE);
    }
    int index = optind;
    for(;index<argc;index++){
        if(!parse_file(argv[index], NULL)){
            printf("Error parsing file \"%s\"\n",argv[index]);
            exit(EXIT_FAILURE);
        }
    }
    VerifyText();
    
    if(print_includes){
        parse_state::root->print(0);
    }else{
        RenderText();
    }
    
    exit(EXIT_SUCCESS);
}
