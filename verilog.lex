/* scanner for PA5 grammar */
/* Distributed under GPL from   */
/* http://www.len.ro/work/verilog-2-vhdl/ */
%{
    /* need this for the call to atof() below */
#define YY_NO_UNPUT

#include <iostream>
#include <cstdio>
#include <string>
#include <cmath>
using namespace std;
#include "design.h"
#include "gates.h"
#include "net.h"
#include "verilog.tab.h"


    extern int yylineno;
    extern YYSTYPE yylval;


    void yy_char_ptr(char *string);

%}

%option noyywrap
DIGIT    [0-9]
IDENT       [a-z_][a-z0-9_]*
SPACE     [ \t\n]          /* eat up whitespace */
NUMBER   {DIGIT} {DIGIT}*
BINARY      ( {Number})?\'[bB][01xXzZ?][01xXzZ?_]*

%%

			/* open curly brace needs to be on same line as rule */
[ \t]+		{ continue; }
[\n]            { yylineno++; }
"//"[^\n]*\n 	{ /*printf("Comment found\n");*/ yylineno++; continue; }  /*single line comment */

{DIGIT}+    {
  //printf( "An integer: %s (%d)\n", yytext,atoi( yytext ) );
yylval.ival = atoi(yytext);
return NUMBER;
}


"("|")"|","|";"|"#"      {
  //printf("Found punctuation\n");
return (int)yytext[0];
}


module   {
  //printf("Module declaration\n");
return MODULE;
}

endmodule {
  //printf("endmodule declaration\n");
return ENDMODULE;

}

input  {
return INPUT;
}

output {
return OUTPUT;
}

wire {
return WIRE;
}
and {           
  //printf( "A keyword: %s\n", yytext );
return GAND;
}
or {           
  //printf( "A keyword: %s\n", yytext );
return GOR;
}
nand {           
  //printf( "A keyword: %s\n", yytext );
return GNAND;
}
nor {           
  //printf( "A keyword: %s\n", yytext );
return GNOR;
}
xor {           
  //printf( "A keyword: %s\n", yytext );
return GXOR;
}
not {           
  //printf( "A keyword: %s\n", yytext );
return GNOT;
}

{IDENT} {       
// printf( "An identifier: %s\n", yytext );
yylval.sval = yytext;
return ID; }

"+"|"-"|"*"|"/"   { 
printf( "An operator: %s\n", yytext ); 
return yytext[0]; 
}

"{"[\^{}}\n]*"}"     /* eat up one-line comments */


.           printf( "Unrecognized character: %s\n", yytext );

%%

#ifdef LEX_ONLY
main(int argc, char **argv )
{
if ( argc > 1 )
yyin = fopen( argv[1], "r" );
else
yyin = stdin;

yylex();
printf("Parsed %d lines\n",yylineno+1);
}
#endif
