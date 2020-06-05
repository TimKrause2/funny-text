#include "qstring.h"
#include "parser.tab.hpp"
#include "parser.h"
#include "lexer.h"
#include <stdio.h>
#include <string>

parse_state *parse_state::root = NULL;

parse_state::parse_state(parse_state *included_from,
                         char *filename,
                         FILE *file ):
included_from(included_from),
includes(NULL),
next(NULL),
filename(strdup(filename))
{
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
    yylex_init(&scaninfo);
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
}

int main(int argc, char **argv)
{
    if(argc!=2){
        printf("Incorrect number of arguments.\n");
        printf("Usage: %s <input file>\n",argv[0]);
        return 1;
    }
    
    if(parse_file(argv[1], NULL)){
        VerifyText();
        //parse_state::root->print(0);
        RenderText();
    }
    
    return 0;
}
