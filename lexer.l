
%x quote quote_ref quote_escape

%option noyywrap
%option yylineno
%option bison-bridge
%option bison-locations
%option stack
%option pointer

%{
#include "parser.h"
#include "parser.tab.hpp"
#include <stdlib.h>
#include <stdio.h>

#define YY_USER_ACTION                                             \
yylloc->first_line = yylloc->last_line;                          \
yylloc->first_column = yylloc->last_column;                      \
if (yylloc->last_line == yylineno)                               \
    yylloc->last_column += yyleng;                                 \
else {                                                           \
    yylloc->last_line = yylineno;                                  \
    yylloc->last_column = yytext + yyleng - strrchr(yytext, '\n'); \
}
%}

DIGIT       [0-9]
ALPHA       [a-zA-Z_\x80-\xFF]
ALNUM       [0-9a-zA-Z_\x80-\xFF]
REF_STR     {ALPHA}{ALNUM}*
QUOTE_STR   [^<>\|\\]+

%%

{DIGIT}+    {yylval->INT = atol(yytext); return INT; }
{REF_STR}   {yylval->STRING = yytext; return STRING; }
\:          {return ':';}
\;          {return ';';}
\<          {yy_push_state(quote); return '<';}
<quote>\>   {yy_pop_state(); return '>';}
<quote>{QUOTE_STR} {yylval->STRING = yytext; return STRING; }
<quote>\|   {yy_push_state(quote_ref); return '|'; }
<quote_ref>{REF_STR} {yylval->STRING = yytext; return STRING; }
<quote_ref>\|       {yy_pop_state(); return '|'; }
<quote>\\   {yy_push_state(quote_escape); }
<quote_escape>{
t       {yylval->STRING = '\t'; yy_pop_state(); return STRING; }
n       {yylval->STRING = '\n'; yy_pop_state(); return STRING; }
\|      {yylval->STRING = '|'; yy_pop_state(); return STRING; }
\\      {yylval->STRING = '\\'; yy_pop_state(); return STRING; }
\<      {yylval->STRING = '<'; yy_pop_state(); return STRING; }
\>      {yylval->STRING = '>'; yy_pop_state(); return STRING; }
.       {yylval->STRING = *yytext; yy_pop_state(); return STRING; }
\n      {yy_pop_state();}
}
.           {}
\n          {}

%%

void SetInputBuffer( FILE *file )
{
    yyrestart(file);
}
