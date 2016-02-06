%{ /* C++ */
	#include <cerrno>
	#include <climits>
	#include <cstdlib>
	#include <string>
	#include "ast/bison.hh"
	#include "ast/lexproto.h"
	#include "ast/parseexception.h"

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

id    [a-zA-Z_\$][a-zA-Z0-9_\$]*
int   [0-9]+
blank [ \t]

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
"void"		return yy::BisonParser::make_VOID(loc);
"return"	return yy::BisonParser::make_RETURN(loc);

";"		return yy::BisonParser::make_SEMICOLON(loc);
","		return yy::BisonParser::make_COMMA(loc);
"+"		return yy::BisonParser::make_ADD(loc);
"-"		return yy::BisonParser::make_SUB(loc);
"*"		return yy::BisonParser::make_MUL(loc);
"/"		return yy::BisonParser::make_DIV(loc);
"="		return yy::BisonParser::make_ASSIGN(loc);

"{"		return yy::BisonParser::make_LBRACE(loc);
"}"		return yy::BisonParser::make_RBRACE(loc);
"("		return yy::BisonParser::make_LPAREN(loc);
")"		return yy::BisonParser::make_RPAREN(loc);


{int}      {
	errno = 0;
	long n = strtol(yytext, NULL, 10);
	if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
		throw ParseException(loc, "integer out of range");
	return yy::BisonParser::make_NUMBER(yytext, loc);
}

{id}       return yy::BisonParser::make_IDENTIFIER(yytext, loc);
.          throw ParseException(loc, "invalid character");
<<EOF>>    return yy::BisonParser::make_END(loc);

%%

namespace yaloe
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

} // namespace yaloe

