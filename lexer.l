
%x quote quote_ref quote_escape path

%option noyywrap
%option yylineno
%option bison-bridge
%option bison-locations
%option stack
%option pointer
%option reentrant
%option noinput nounput 8bit nodefault

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
PATH_STR    [^\|]+

%%

{DIGIT}+    {yylval->INT = atol(yytext); return INT; }
{REF_STR}   {yylval->STRING = yytext; return STRING; }
\:          {return ':';}
\;          {return ';';}
\<          {yy_push_state(quote,yyscanner); return '<';}
<quote>\>   {yy_pop_state(yyscanner); return '>';}
<quote>\<   {return YYUNDEF;}
<quote>{QUOTE_STR} {yylval->STRING = yytext; return STRING; }
<quote>\|   {yy_push_state(quote_ref,yyscanner); return '|'; }
<quote_ref>{REF_STR} {yylval->STRING = yytext; return STRING; }
<quote_ref>\|       {yy_pop_state(yyscanner); return '|'; }
<quote>\\   {yy_push_state(quote_escape,yyscanner); }
<quote_escape>{
t       {yylval->STRING = '\t'; yy_pop_state(yyscanner); return STRING; }
n       {yylval->STRING = '\n'; yy_pop_state(yyscanner); return STRING; }
\|      {yylval->STRING = '|'; yy_pop_state(yyscanner); return STRING; }
\\      {yylval->STRING = '\\'; yy_pop_state(yyscanner); return STRING; }
\<      {yylval->STRING = '<'; yy_pop_state(yyscanner); return STRING; }
\>      {yylval->STRING = '>'; yy_pop_state(yyscanner); return STRING; }
[\x80-\xFF] {return YYUNDEF;}
.       {yylval->STRING = *yytext; yy_pop_state(yyscanner); return STRING; }
\n      {yy_pop_state(yyscanner);}
}
\|          {yy_push_state(path,yyscanner); return '|';}
<path>{PATH_STR}    {yylval->STRING = yytext; return STRING;}
<path>\|            {yy_pop_state(yyscanner); return '|'; }
.           {}
\n          {}

%%
