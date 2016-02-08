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
	#include "ast/scalarast.h"
	#include "ast/fixsizeast.h"
	#include "ast/listast.h"
	#include "ast/termlistast.h"
	#include "ast/rootast.h"
	#include "ast/declbeanast.h"
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
	RETURN		"return"
	WHILE		"while"
	DO			"do"
	IF			"if"
	ELSE		"else"

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
%type	<ListAst*>		param_list
%type	<ListAst*>		param_list_noemp
%type	<ListAst*>		stmt_list
%type	<ListAst*>		if_list
%type	<ListAst*>		arg_list
%type	<ListAst*>		arg_list_noemp
%type	<ListAst*>		decl_bean_list
%type	<Ast*>			class
%type	<Ast*>			method
%type	<Ast*>			stmt
%type	<Ast*>			expr
%type	<Ast*>			decl_stmt
%type	<Ast*>			decl_bean
%type	<Ast*>			decl_param
%type	<Ast*>			return_stmt
%type	<Ast*>			while_stmt
%type	<Ast*>			do_while_stmt
%type	<Ast*>			name
%type	<Ast*>			new_name
%type	<Ast*>			type
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

class: "class" new_name"{" method_list "}"	
	 			{ $$ = new FixSizeAst<2>(Ast::Type::CLASS, $2, $4); }
	 ;

method_list: %empty					
		   		{ $$ = new ListAst(Ast::Type::METHOD_LIST); }
		   | method_list method		
				{ $1->append($2); $$ = $1; }
		   ;

method: "void" new_name "(" param_list ")" "{" stmt_list "}"
 				{ $$ = new FixSizeAst<3>(Ast::Type::METHOD, $2, $4, $7); }
	  ;

param_list: %empty
		 		{ $$ = new ListAst(Ast::Type::DECL_PARAM_LIST); } 
		  | param_list_noemp
		 		{ $$ = $1; }

param_list_noemp: decl_param
		 		{ $$ = new ListAst(Ast::Type::DECL_PARAM_LIST); 
				  $$->append($1); } 
		  | param_list_noemp "," decl_param
				{ $1->append($3); $$ = $1; }
		  ;

decl_param: type new_name
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_PARAM, $1, $2); } 
		  ;

stmt_list: %empty
		 		{ $$ = new ListAst(Ast::Type::STMT_LIST); }
		 | stmt_list stmt
				{ $1->append($2); $$ = $1; }
		 ;

stmt: expr ";"
				{ $$ = $1; }
	| decl_stmt
				{ $$ = $1; }
	| return_stmt
				{ $$ = $1; }
	| while_stmt
				{ $$ = $1; }
	| do_while_stmt
				{ $$ = $1; }
	| if_list
				{ $$ = $1; }
	| "{" stmt_list "}"
				{ $$ = $2; }
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
				{ $$ = new FixSizeAst<2>(Ast::Type::ASSIGN, $1, $3); } 
	| name "(" arg_list ")"
				{ $$ = new FixSizeAst<2>(Ast::Type::CALL, $1, $3); } 
	| "(" expr ")"
				{ $$ = new FixSizeAst<1>(Ast::Type::PAREN, $2); } 
	| name
				{ $$ = $1; } 
	| "number"
				{ $$ = new ScalarAst(Ast::Type::NUMBER, $1); }
	;

name: "identifier"
				{ $$ = new ScalarAst(Ast::Type::IDENT, $1); }
	;

new_name: "identifier"
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

decl_stmt: type decl_bean_list ";"
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL, $1, $2); }
	;

decl_bean_list: decl_bean
		 		{ $$ = new ListAst(Ast::Type::DECL_BEAN_LIST); 
				  $$->append($1); }
			  | decl_bean_list "," decl_bean
		 		{ $1->append($3); $$ = $1; }
			  ;

decl_bean: new_name
		 		{ $$ = new DeclBeanAst($1); }
		 | new_name "=" expr
		 		{ $$ = new DeclBeanAst($1, $3); }
		 ;

type: name
				{ $$ = $1; }
	;

return_stmt: "return" expr ";"
		 		{ $$ = new FixSizeAst<1>(Ast::Type::RETURN, $2); }

while_stmt: "while" "(" expr ")" stmt
		 		{ ListAst *body = $5->bodify();
				  $$ = new FixSizeAst<2>(Ast::Type::WHILE, $3, body); }

do_while_stmt: "do" stmt "while" "(" expr ")" ";"
		 		{ ListAst *body = $2->bodify();
				  $$ = new FixSizeAst<2>(Ast::Type::DO_WHILE, body, $5); }

%right	"then" "else";

if_list: "if" "(" expr ")" stmt %prec "then"
				{ Ast *cond = new FixSizeAst<2>(Ast::Type::IF_CONDBODY,
												$3, $5->bodify()); 
				  $$ = new ListAst(Ast::Type::IF_LIST);
				  $$->append(cond); }
	   | "if" "(" expr ")" stmt "else" stmt
				{ Ast *cond = new FixSizeAst<2>(Ast::Type::IF_CONDBODY,
												$3, $5->bodify()); 
				  $$ = new ListAst(Ast::Type::IF_LIST);
				  $$->append(cond); 
				  if ($7->getType() == Ast::Type::IF_LIST) {
					  ListAst *tail = &$7->asList();
					  for (size_t i = 0; i < tail->size(); i++)
						  $$->append(tail->remove(i).release());
				  } else {
					  Ast *elze = new FixSizeAst<1>(Ast::Type::IF_ELSEBODY,
													$7->bodify()); 
					  $$->append(elze);
				  } }
	   ;

%%

void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

