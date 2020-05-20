#include "parser.tab.hpp"
#include "qstring.h"
#include "time.h"
#include <stdio.h>

void SetInputBuffer( FILE *file );

int main(int argc, char **argv)
{
    FILE *file;
    if(argc == 2){
        file = fopen( argv[1], "r" );
        if(!file){
            perror("fopen");
            return 1;
        }
        SetInputBuffer(file);
    }

    if(yyparse()==0)
        RenderText();
    
    return 0;
}
