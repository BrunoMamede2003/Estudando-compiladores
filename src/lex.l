%{

//Deixar o código postável.

#include <token_type.h>
#include <macros.h>
#include <stdio.h>
#include <stdlib.h>

char const * const out_file_name = "out_lex.txt";

%}

word [A-Za-z]+
litinteger [0-9]+
litfloat [0-9]*\.[0-9]+
symbassignment "="
symbop [\+\-\*\/]
commentstart "/*"
commentend "*/"
whitespace [ \t\n]

%x STRING_STATE
%x STRING_CONCAT
%x COMMENT_STATE

%option noyywrap
%%

{whitespace} {}
{word} { return ID; }
{litinteger} { return LITERAL_INT; }
{litfloat} { return LITERAL_FLOAT; }
{symbassignment} { return ASSIGNMENT; }
{symbop} { return OPERATOR; }
"\"" { BEGIN(STRING_STATE); yymore(); }
<STRING_STATE>"\n" { return ERROR; yyterminate(); }
<STRING_STATE>"\"" { BEGIN(STRING_CONCAT); yymore(); }
<STRING_STATE>[^\"] { yymore(); }
<STRING_CONCAT>{whitespace}*\" { yymore(); BEGIN(STRING_STATE); }
<STRING_CONCAT>[^ \t\n\"] { yyless(yyleng - 1); BEGIN(INITIAL); return STATIC_STRING; }
";" { return SENTENCE_END; }
{commentstart} { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>"\n" { }
<COMMENT_STATE>{commentend} { BEGIN(INITIAL); }
<COMMENT_STATE>. { }
%%

#define RET(X) do {retval = X; goto CLEANUP;} while(0)
int main(int argc, char ** argv) {
  int retval = 0;
  FILE * out_file = null;
  yyin = null;
  int yylex_return;
  size_t read_bytes = 0;
  if(argc < 2)
    RET(-1);
  yyin = fopen(argv[1], "r");
  out_file = fopen(out_file_name, "w");
  if(not out_file or not yyin)
    RET(-1);
  while((yylex_return = yylex()) != 0) {
    fprintf(out_file, "[LOG] Read token: [%s] = [%s]\n", token_type_to_str(yylex_return), yytext);
  }

  CLEANUP:
  if(out_file)
    fclose(out_file);
  if(yyin)
    fclose(yyin);
  return 0;
}
#undef RET