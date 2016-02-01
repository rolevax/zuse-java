%skeleton "lalr1.cc" /*  C++  */
%require "3.0.4"

%defines
%define parser_class_name {BisonParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
	#include <string>
	#include "ast/ast.h"
	#include "ast/rootast.h"
	#include "ast/listast.h"
	#include "ast/termlistast.h"
	#include "ast/classast.h"
	#include "ast/methodast.h"
	#include "ast/scalarast.h"
	#include "ast/parenast.h"
	#include "ast/bopast.h"
	class ParseException;
}

/* The parsing context. */
%param { const std::string &filename }
%param { RootAst *result }

%locations
%initial-action
{
	/* Initialize the initial location. */ 
	/* position.filename is a non-const pointer somehow */
	static std::string s_filename(filename);
	@$.begin.filename = @$.end.filename = &s_filename;
};
%define parse.trace
%define parse.error verbose

%code
{
	#include "ast/lexproto.h"
	#include "ast/parseexception.h"
}

%define api.token.prefix {TOK_}

/* ============ token defs ============ */

%token
	END	0		"EOF"

	ASSIGN		"="
	SUB			"-"
	ADD			"+"
	MUL			"*"
	DIV			"/"

	CLASS		"class"
	VOID		"void"

	SEMICOLON	";"
	COMMA		","
	LBRACE		"{"
	RBRACE		"}"
	LPAREN		"("
	RPAREN		")"
;

%token	<std::string>	IDENTIFIER	"identifier"
%token	<std::string>	NUMBER		"number"
%type	<ListAst*>		class_list
%type	<ListAst*>		method_list
%type	<ListAst*>		decl_list
%type	<ListAst*>		stmt_list
%type	<ListAst*>		arg_list
%type	<ListAst*>		arg_list_noemp
%type	<Ast*>			class
%type	<Ast*>			method
%type	<Ast*>			expr
%type	<Ast*>			name
%printer { yyoutput << $$; } <*>;

%% /* ============ rules ============ */
/* all list non-terminals exclude begin/end deliminators
 * and include internal separator dealings 
 * for those internal-only separators, do with xxx_noemp
 */

%start class_list;

class_list: %empty					{ /* already newwed as 'result' */ }
		  | class_list class		{ result->append($2); }
		  ;

class: "class" "identifier" "{" method_list "}"	
	 			{ $$ = new ClassAst($2, $4); }
	 ;

method_list: %empty					
		   		{ $$ = new ListAst(Ast::Type::METHOD_LIST); }
		   | method_list method		
				{ $1->append($2); $$ = $1; }
		   ;

method: "void" "identifier" "(" decl_list ")" "{" stmt_list "}"
 				{ $$ = new MethodAst($2, $4, $7); }
	  ;

decl_list: %empty
		 		{ $$ = new ListAst(Ast::Type::DECL_LIST); }
		 ;

stmt_list: %empty
		 		{ $$ = new ListAst(Ast::Type::STMT_LIST); }
		 | stmt_list expr ";"
				{ $1->append($2); $$ = $1; }
		 ;

%right	"=";
%left	"+" "-";
%left	"*" "/";
expr: expr "+" expr
				{ $$ = TermListAst::makeBop($1, $3, TermListAst::Op::ADD); }
	| expr "-" expr
				{ $$ = TermListAst::makeBop($1, $3, TermListAst::Op::SUB); }
	| expr "*" expr
				{ $$ = TermListAst::makeBop($1, $3, TermListAst::Op::MUL); }
	| expr "/" expr
				{ $$ = TermListAst::makeBop($1, $3, TermListAst::Op::DIV); }
	| expr "=" expr
				{ $$ = new BopAst(Ast::Type::ASSIGN, $1, $3); } 
	| name "(" arg_list ")"
				{ $$ = new BopAst(Ast::Type::CALL, $1, $3); } 
	| "(" expr ")"
				{ $$ = new ParenAst($2); } 
	| name
				{ $$ = $1; } 
	| "number"
				{ $$ = new ScalarAst(Ast::Type::NUMBER, $1); }
	;

name: "identifier"
				{ $$ = new ScalarAst(Ast::Type::IDENT, $1); }
	;

arg_list: %empty
		 		{ $$ = new ListAst(Ast::Type::ARG_LIST); }
		| arg_list_noemp
		 		{ $$ = $1; }
		;

arg_list_noemp: expr
		 		{ $$ = new ListAst(Ast::Type::ARG_LIST); $$->append($1); }
			  | arg_list_noemp "," expr
		 		{ $1->append($3); $$ = $1; }
			  ;



%%

void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

