%{ /* C++ */
	#include <cerrno>
	#include <climits>
	#include <cstdlib>
	#include <string>
	#include "bison.hh"
	#include "lexproto.h"
	#include "parse_exception.h"

	// Work around an incompatibility in flex (at least versions
	// 2.5.31 through 2.5.33): it generates code that does
	// not conform to C89.  See Debian bug 333231
	// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
	# undef yywrap
	# define yywrap() 1

	// The location of the current token.
	static yy::location loc;
%}

%option noyywrap nounput batch debug noinput

id    	[a-zA-Z_\$][a-zA-Z0-9_\$]*
int   	[0-9]+
string	\"([^\\\"]|\\.)*\"
blank 	[ \t]

%{
	// Code run each time a pattern is matched.
	#define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
	// Code run each time yylex is called.
	loc.step();
%}

{blank}+   loc.step();
[\n]+      loc.lines(yyleng); loc.step();

"class"		return yy::BisonParser::make_CLASS(loc);
"interface"	return yy::BisonParser::make_INTERFACE(loc);
"extends"	return yy::BisonParser::make_EXTENDS(loc);
"implements" return yy::BisonParser::make_IMPLEMENTS(loc);
"while"		return yy::BisonParser::make_WHILE(loc);
"do"		return yy::BisonParser::make_DO(loc);
"for"		return yy::BisonParser::make_FOR(loc);
"if"		return yy::BisonParser::make_IF(loc);
"else"		return yy::BisonParser::make_ELSE(loc);
"return"	return yy::BisonParser::make_RETURN(loc);
"break"		return yy::BisonParser::make_BREAK(loc);
"continue"	return yy::BisonParser::make_CONTINUE(loc);
"throw"		return yy::BisonParser::make_THROW(loc);
"throws"	return yy::BisonParser::make_THROWS(loc);
"instanceof"return yy::BisonParser::make_INSTANCEOF(loc);
"this"		return yy::BisonParser::make_THIS(loc);
"super"		return yy::BisonParser::make_SUPER(loc);
"null"		return yy::BisonParser::make_JNULL(loc);
"new"		return yy::BisonParser::make_NEW(loc);
"try"		return yy::BisonParser::make_TRY(loc);
"catch"		return yy::BisonParser::make_CATCH(loc);
"finally"	return yy::BisonParser::make_FINALLY(loc);

"abstract"	return yy::BisonParser::make_ABSTRACT(loc);
"final"		return yy::BisonParser::make_FINAL(loc);
"public"	return yy::BisonParser::make_PUBLIC(loc);
"protected"	return yy::BisonParser::make_PROTECTED(loc);
"private"	return yy::BisonParser::make_PRIVATE(loc);
"static"	return yy::BisonParser::make_STATIC(loc);
"transient"	return yy::BisonParser::make_TRANSIENT(loc);
"volatile"	return yy::BisonParser::make_VOLATILE(loc);
"native"	return yy::BisonParser::make_NATIVE(loc);
"synchronized"	return yy::BisonParser::make_SYNCHRONIZED(loc);

"void"		return yy::BisonParser::make_VOID(yytext, loc);

"="		return yy::BisonParser::make_ASSIGN(loc);
"+="	return yy::BisonParser::make_ASS_ADD(loc);
"-="	return yy::BisonParser::make_ASS_SUB(loc);
"*="	return yy::BisonParser::make_ASS_MUL(loc);
"/="	return yy::BisonParser::make_ASS_DIV(loc);
"%="	return yy::BisonParser::make_ASS_MOD(loc);
"&="	return yy::BisonParser::make_ASS_AND(loc);
"^="	return yy::BisonParser::make_ASS_XOR(loc);
"|="	return yy::BisonParser::make_ASS_OR(loc);
"<<="	return yy::BisonParser::make_ASS_SHL(loc);
">>="	return yy::BisonParser::make_ASS_SHR(loc);
">>>="	return yy::BisonParser::make_ASS_SHRA(loc);
":"		return yy::BisonParser::make_COLON(loc);
"?"		return yy::BisonParser::make_QUESTION(loc);
";"		return yy::BisonParser::make_SEMICOLON(loc);
"."		return yy::BisonParser::make_DOT(loc);
","		return yy::BisonParser::make_COMMA(loc);
"+"		return yy::BisonParser::make_ADD(loc);
"-"		return yy::BisonParser::make_SUB(loc);
"*"		return yy::BisonParser::make_MUL(loc);
"/"		return yy::BisonParser::make_DIV(loc);
"|"		return yy::BisonParser::make_BIT_OR(loc);
"&"		return yy::BisonParser::make_BIT_AND(loc);
"^"		return yy::BisonParser::make_BIT_XOR(loc);
"~"		return yy::BisonParser::make_BIT_NOT(loc);

"{"		return yy::BisonParser::make_LBRACE(loc);
"}"		return yy::BisonParser::make_RBRACE(loc);
"["		return yy::BisonParser::make_LSQUARE(loc);
"]"		return yy::BisonParser::make_RSQUARE(loc);
"("		return yy::BisonParser::make_LPAREN(loc);
")"		return yy::BisonParser::make_RPAREN(loc);

"++"	return yy::BisonParser::make_INC(loc);
"--"	return yy::BisonParser::make_DEC(loc);
"||"	return yy::BisonParser::make_LOGIC_OR(loc);
"&&"	return yy::BisonParser::make_LOGIC_AND(loc);
"!"		return yy::BisonParser::make_LOGIC_NOT(loc);
"=="	return yy::BisonParser::make_EQ(loc);
"!="	return yy::BisonParser::make_NEQ(loc);
"<"		return yy::BisonParser::make_LT(loc);
">"		return yy::BisonParser::make_GT(loc);
"<="	return yy::BisonParser::make_LEQ(loc);
">="	return yy::BisonParser::make_GEQ(loc);
"<<"	return yy::BisonParser::make_SHL(loc);
">>"	return yy::BisonParser::make_SHR(loc);
">>>"	return yy::BisonParser::make_SHRA(loc);
"[]"	return yy::BisonParser::make_DIM(loc);


{int}      {
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
		throw ParseException(loc, "integer out of range");
	return yy::BisonParser::make_NUMBER(yytext, loc);
}

{id}       return yy::BisonParser::make_IDENTIFIER(yytext, loc);

{string}   {
	yytext[strlen(yytext) - 1] = '\0';
	yytext++;
	return yy::BisonParser::make_STRING(yytext, loc);
}

.          throw ParseException(loc, "invalid character");
<<EOF>>    return yy::BisonParser::make_END(loc);

%%

namespace zuse
{ 



void scan_begin(const std::string &filename, bool traceLex)
{
	yy_flex_debug = traceLex;
	if (filename.empty() || filename == "-") {
		yyin = stdin;
	} else if (!(yyin = fopen(filename.c_str(), "r"))) {
		throw std::runtime_error("cannot open " + filename + ": " 
									+ strerror(errno));
	}
} 

void scan_end()
{
	fclose(yyin);
}



} // namespace zuse

