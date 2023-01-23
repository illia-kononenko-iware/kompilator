%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "classes/variable.h"
#include "classes/code_generator.h"
#include "parser.tab.hpp"


int count = 0;
int yylex();
void yyerror(char *s);

int line = 1;
%}

%%

","   { return COMMA; }
"PROCEDURE" { return PROCEDURE; }
"PROGRAM"   { return PROGRAM; }
"IS"   { return IS; }
"VAR"   { return VAR; }
"BEGIN"   { return _BEGIN; }
"END"   { return END; }

"READ"  { return READ; }
"WRITE" { return WRITE; }

"IF"                { return IF; }
"THEN"              { return THEN; }
"ELSE"              { return ELSE; }
"ENDIF"             { return ENDIF; }

"WHILE"             { return WHILE; }
"DO"                { return DO; }
"ENDWHILE"          { return ENDWHILE; }

"REPEAT"            { return REPEAT; }
"UNTIL"             { return UNTIL; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return MUL; }
"/"                 { return DIV; }
"%"                 { return MODULO; }

":="                { return ASSIGN; }

"("                 { return LBRACKET; }
")"                 { return RBRACKET; }

"="                 { return EQ; }
"!="                { return NE; }
"<"                 { return LT; }
">"                 { return GT; }
"<="                { return LE; }
">="                { return GE; }

";"                 { return SEMICOLON; }

[a-zA-Z]+ { yylval.str = new std::string(yytext); return PIDENTIFIER; }

[0-9]+ { yylval.num = std::stoll(yytext); return INT; }

[.+ \r\t\n]           ;

"//".*  { /* ignore rest of the line */ }

"\n"  { line++; }

. { return INVALIDCHAR; }

%%

void yyerror(char *s)
{
    printf("Error: %s at line %d\n", s, line);
    exit(1);
}

int yywrap(){}