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
	#include "ast.h"
	#include "ast_scalar.h"
	#include "ast_fix_size.h"
	#include "ast_list.h"
	#include "ast_list_bop.h"
	#include "ast_root.h"
	class ParseException;
}

/* The parsing context. */
%param { const std::string &filename }
%param { AstRoot *result }

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
	#include "lexproto.h"
	#include "parse_exception.h"
}

%define api.token.prefix {TOK_}

/* ============ token defs ============ */

%token
	END	0		"EOF"

	CLASS		"class"
	INTERFACE	"interface"
	EXTENDS		"extends"
	IMPLEMENTS	"implements"
	WHILE		"while"
	DO			"do"
	FOR			"for"
	IF			"if"
	ELSE		"else"
	RETURN		"return"
	BREAK		"break"
	CONTINUE	"continue"
	THROW		"throw"
	THROWS		"throws"
	INSTANCEOF	"instanceof"
	JNULL		"null"
	THIS		"this"
	SUPER		"super"
	NEW			"new"
	TRY			"try"
	CATCH		"catch"
	FINALLY		"finally"

	ABSTRACT	"abstract"
	FINAL		"final"
	PUBLIC		"public"
	PROTECTED	"protected"
	PRIVATE		"private"
	STATIC		"static"
	TRANSIENT	"transient"
	VOLATILE	"volatile"
	NATIVE		"native"
	SYNCHRONIZED"synchronized" 

	ASSIGN		"="
	ASS_SUB		"-="
	ASS_ADD		"+="
	ASS_MUL		"*="
	ASS_DIV		"/=" 
	ASS_MOD		"%="
	ASS_AND		"&="
	ASS_XOR		"^="
	ASS_OR		"|="
	ASS_SHL		"<<="
	ASS_SHR		">>="
	ASS_SHRA	">>>="
	SUB			"-"
	ADD			"+"
	MUL			"*"
	DIV			"/" 
	MOD			"%"
	COLON		":"
	QUESTION	"?"
	SEMICOLON	";"
	DOT			"."
	COMMA		","
	LBRACE		"{"
	RBRACE		"}"
	LSQUARE		"["
	RSQUARE		"]"
	LPAREN		"("
	RPAREN		")"
	LOGIC_OR	"||"
	LOGIC_AND	"&&"
	LOGIC_NOT	"!"
	BIT_OR		"|"
	BIT_AND		"&"
	BIT_XOR		"^"
	BIT_NOT		"~"
	EQ			"=="
	NEQ			"!="
	LT			"<"
	GT			">"
	LEQ			"<="
	GEQ			">="
	SHL			"<<"
	SHR			">>"
	SHRA		">>>"
	INC			"++"
	DEC			"--"
	DIM			"[]"
;

%token	<std::string>	IDENTIFIER	"identifier"
%token	<std::string>	NUMBER		"number"
%token	<std::string>	STRING		"string"
%token	<std::string>	VOID		"void" 

%type	<int>			dims

%type	<AstList*>		class_list
%type	<AstList*>		member_list
%type	<AstList*>		param_list
%type	<AstList*>		param_list_noemp
%type	<AstList*>		stmt_list
%type	<AstList*>		if_list
%type	<AstList*>		arg_list
%type	<AstList*>		arg_list_noemp
%type	<AstList*>		dector_list
%type	<AstList*>		try_list
%type	<AstList*>		catch_list
%type	<AstList*>		name_list

%type	<Ast*>			class
%type	<Ast*>			decl_method
%type	<Ast*>			stmt
%type	<Ast*>			decl_var
%type	<Ast*>			decl_param
%type	<Ast*>			dector
%type	<Ast*>			dector_name
%type	<Ast*>			jump_stmt
%type	<Ast*>			while_stmt
%type	<Ast*>			do_while_stmt
%type	<Ast*>			for_stmt
%type	<Ast*>			for_init
%type	<Ast*>			catch
%type	<Ast*>			ident
%type	<Ast*>			type_spec
%type	<Ast*>			type_name
%type	<Ast*>			ptype

%type	<Ast*>			expr
%type	<Ast*>			expr_lv0
%type	<Ast*>			expr_lv1
%type	<Ast*>			expr_lv2
%type	<Ast*>			expr_lv3
%type	<Ast*>			expr_lv4
%type	<Ast*>			expr_lv5
%type	<Ast*>			expr_lv6
%type	<Ast*>			expr_lv7
%type	<Ast*>			expr_lv8
%type	<Ast*>			expr_lv9
%type	<Ast*>			expr_lv10
%type	<Ast*>			expr_lv11
%type	<Ast*>			expr_lv12
%type	<Ast*>			expr_ptype
%type	<Ast*>			expr_unary
%type	<Ast*>			expr_unary_logic
%type	<Ast*>			expr_post
%type	<Ast*>			expr_pp
%type	<Ast*>			expr_prime
%type	<Ast*>			expr_prime_noname
%type	<Ast*>			expr_prime_cx
%type	<Ast*>			expr_prime_cx_nude
%type	<Ast*>			expr_call
%type	<Ast*>			expr_field
%type	<Ast*>			expr_new
%type	<Ast*>			expr_new_plain
%type	<Ast*>			callee
%type	<Ast*>			name
%type	<Ast*>			special_name
%type	<Ast*>			dim_exprs

%type	<Modifiers>		modifiers
%type	<Modifiers>		modifier

%printer { yyoutput << $$; } <*>;

%% /* ============ rules ============ */
/* all list non-terminals exclude begin/end deliminators
 * and include internal separator dealings 
 * for those internal-only separators, do with xxx_noemp
 */

%start class_list;

type_spec: type_name
	 			{ $$ = $1; }
		 | type_name dims
				{ $$ = new AstListBop($1, $2); }
		 ;

type_name: ptype
	 			{ $$ = $1; }
		 | name
	 			{ $$ = $1; }
		 ;

ptype: "void"
	 			{ $$ = new AstScalar(Ast::Type::IDENT, $1); }
	 ;

modifiers: modifier
		 		{ $$ = $1; }
		 | modifiers modifier
				{ $1 |= $2; $$ = $1; }
		 ;

modifier: "abstract"
				{ $$ = Modifiers(); $$.abstract = true; }
		| "final"
				{ $$ = Modifiers(); $$.final = true; }
		| "public"
				{ $$ = Modifiers(); $$.access = Modifiers::PUBLIC; }
		| "protected"
				{ $$ = Modifiers(); $$.access = Modifiers::PROTECTED; }
		| "private"
				{ $$ = Modifiers(); $$.access = Modifiers::PRIVATE; }
		| "static"
				{ $$ = Modifiers(); $$.statik = true; }
		| "transient"
				{ $$ = Modifiers(); $$.transient = true; }
		| "volatile"
				{ $$ = Modifiers(); $$.voladile = true; }
		| "native"
				{ $$ = Modifiers(); $$.native = true; }
		| "synchronized"
				{ $$ = Modifiers(); $$.synchronized = true; }
		;

class_list: %empty					{ /* already newwed as 'result' */ }
		  | class_list class		{ result->append($2); }
		  ;

class: "class" ident "{" member_list "}"	
	 			{ Ast *h1 = new AstScalar(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
										 $2, h1, h2, $4); }
	 | "class" ident "extends" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
										 $2, $4, h, $6); }
	 | "class" ident "implements" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
										 $2, h, $4, $6); }
	 | "class" ident "extends" name_list "implements" name_list 
				"{" member_list "}"
	 			{ $$ = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
										 $2, $4, $6, $8); }
	 | modifiers "class" ident "{" member_list "}"	
	 			{ Ast *h1 = new AstScalar(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
											 $3, h1, h2, $5);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "class" ident "extends" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
											 $3, $5, h, $7);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "class" ident "implements" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
											 $3, h, $5, $7);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "class" ident "extends" name_list "implements" name_list 
				"{" member_list "}"
	 			{ auto a = new AstFixSize<4>(Ast::Type::DECL_CLASS, 
											 $3, $5, $7, $9);
				  a->setModifiers($1); $$ = a; }
     | "interface" ident "{" member_list "}"	
	 			{ Ast *h1 = new AstScalar(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
										 $2, h1, h2, $4); }
	 | "interface" ident "extends" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
										 $2, $4, h, $6); }
	 | "interface" ident "implements" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
										 $2, h, $4, $6); }
	 | "interface" ident "extends" name_list "implements" name_list 
				"{" member_list "}"
	 			{ $$ = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
										 $2, $4, $6, $8); }
	 | modifiers "interface" ident "{" member_list "}"	
	 			{ Ast *h1 = new AstScalar(Ast::Type::HIDDEN, "");
	 			  Ast *h2 = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
											 $3, h1, h2, $5);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "interface" ident "extends" name_list "{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
											 $3, $5, h, $7);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "interface" ident "implements" name_list 
				"{" member_list "}"
	 			{ Ast *h = new AstScalar(Ast::Type::HIDDEN, "");
				  auto a = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
											 $3, h, $5, $7);
				  a->setModifiers($1); $$ = a; }
	 | modifiers "interface" ident "extends" name_list 
				"implements" name_list "{" member_list "}"
	 			{ auto a = new AstFixSize<4>(Ast::Type::DECL_INTERFACE, 
											 $3, $5, $7, $9);
				  a->setModifiers($1); $$ = a; }
	 ;

member_list: %empty					
		   		{ $$ = new AstList(Ast::Type::MEMBER_LIST); }
		   | member_list decl_method
				{ $1->append($2); $$ = $1; }
		   | member_list decl_var
				{ $1->append($2); $$ = $1; }
		   ;

decl_method: type_spec dector_name "(" param_list ")" "{" stmt_list "}"
 				{ Ast* hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<5>(Ast::Type::DECL_METHOD, 
										 $1, $2, $4, hidden, $7); }
           | modifiers type_spec dector_name "(" param_list ")" 
				"{" stmt_list "}"
 				{ Ast* hidden = new AstScalar(Ast::Type::HIDDEN, "");
 				  $$ = new AstFixSize<5>(Ast::Type::DECL_METHOD, 
										 $2, $3, $5, hidden, $8); 
				  $$->asFixSize<5>().setModifiers($1); }
		   | type_spec dector_name "(" param_list ")" 
				"throws" name_list "{" stmt_list "}"
 				{ $$ = new AstFixSize<5>(Ast::Type::DECL_METHOD, 
										 $1, $2, $4, $7, $9); }
           | modifiers type_spec dector_name "(" param_list ")" 
				"throws" name_list "{" stmt_list "}"
 				{ $$ = new AstFixSize<5>(Ast::Type::DECL_METHOD, 
										 $2, $3, $5, $8, $10); 
				  $$->asFixSize<5>().setModifiers($1); }
		   ;

param_list: %empty
		 		{ $$ = new AstList(Ast::Type::DECL_PARAM_LIST); } 
		  | param_list_noemp
		 		{ $$ = $1; }

param_list_noemp: decl_param
		 		{ $$ = new AstList(Ast::Type::DECL_PARAM_LIST); 
				  $$->append($1); } 
		  | param_list_noemp "," decl_param
				{ $1->append($3); $$ = $1; }
		  ;

decl_param: type_spec ident
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_PARAM, $1, $2); } 
		  | "final" type_spec ident
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_PARAM, $2, $3); 
				  $$->asFixSize<2>().getModifiers().final = true; } 
		  ;

stmt_list: %empty
		 		{ $$ = new AstList(Ast::Type::STMT_LIST); }
		 | stmt_list stmt
				{ $1->append($2); $$ = $1; }
		 ;

stmt: expr ";"
				{ $$ = $1; }
	| decl_var
				{ $$ = $1; }
	| jump_stmt
				{ $$ = $1; }
	| while_stmt
				{ $$ = $1; }
	| do_while_stmt
				{ $$ = $1; }
	| for_stmt
				{ $$ = $1; }
	| if_list
				{ $$ = $1; }
	| try_list
				{ $$ = $1; }
	| "{" stmt_list "}"
				{ $$ = $2; }
	;

ident: "identifier"
				{ $$ = new AstScalar(Ast::Type::IDENT, $1); }
		;

arg_list: %empty
		 		{ $$ = new AstList(Ast::Type::ARG_LIST); }
		| arg_list_noemp
		 		{ $$ = $1; }
		;

arg_list_noemp: expr
		 		{ $$ = new AstList(Ast::Type::ARG_LIST); $$->append($1); }
			  | arg_list_noemp "," expr
		 		{ $1->append($3); $$ = $1; }
			  ;

decl_var: type_spec dector ";"
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_VAR, $1, $2); }
		| type_spec dector_list ";" 
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_VAR, $1, $2); }
		| modifiers type_spec dector ";" 
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_VAR, $2, $3); 
				  $$->asFixSize<2>().setModifiers($1); }
		| modifiers type_spec dector_list ";" 
		 		{ $$ = new AstFixSize<2>(Ast::Type::DECL_VAR, $2, $3); 
				  $$->asFixSize<2>().setModifiers($1); }
		;

dector_list: dector "," dector
		 		{ $$ = new AstList(Ast::Type::DECTOR_LIST); 
				  $$->append($1); $$->append($3); }
		   | dector_list "," dector
		 		{ $1->append($3); $$ = $1; }
		   ;

dector: dector_name
				{ $$ = $1; }
	  | dector_name "=" expr // TODO: array init
				{ $$ = new AstFixSize<2>(Ast::Type::ASSIGN, $1, $3); } 
	  ;

dector_name: ident
				{ $$ = $1; }
		   | dector_name dims
				{ $$ = new AstListBop($1, $2); }
		   ;

jump_stmt: "return" expr ";"
		 		{ $$ = new AstFixSize<1>(Ast::Type::RETURN, $2); }
		 | "return" ";"
		 		{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<1>(Ast::Type::RETURN, hidden); }
		 | "break" ident ";"
		 		{ $$ = new AstFixSize<1>(Ast::Type::BREAK, $2); }
		 | "break" ";"
		 		{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<1>(Ast::Type::BREAK, hidden); }
		 | "continue" ident ";"
		 		{ $$ = new AstFixSize<1>(Ast::Type::CONTINUE, $2); }
		 | "continue" ";"
		 		{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<1>(Ast::Type::CONTINUE, hidden); }
		 | "throw" ident ";"
		 		{ $$ = new AstFixSize<1>(Ast::Type::THROW, $2); }
		 ;

while_stmt: "while" "(" expr ")" stmt
		 		{ AstList *body = $5->bodify().release();
				  $$ = new AstFixSize<2>(Ast::Type::WHILE, $3, body); }

do_while_stmt: "do" stmt "while" "(" expr ")" ";"
		 		{ AstList *body = $2->bodify().release();
				  $$ = new AstFixSize<2>(Ast::Type::DO_WHILE, body, $5); }

for_stmt: "for" "(" for_init expr ";" expr ")" stmt
		 		{ AstList *body = $8->bodify().release();
				  $$ = new AstFixSize<4>(Ast::Type::FOR, 
										 $3, $4, $6, body); }
		;

for_init: decl_var
				{ $$ = $1; }
		| expr ";"
				{ $$ = $1; }
		;

/*
guard_stmt: "synchronized" "(" expression ")" stmt
		  		{ $$ = nullptr; } //TODO
		  ;
*/

try_list: "try" "{" stmt_list "}" catch_list
				{ $$ = new AstList(Ast::Type::TRY_LIST); 
				  $$->append($3);
				  size_t ct = $5->size();
				  for (size_t i = 0; i < ct; i++)
				  	  $$->append($5->remove(0));
				  delete $5; }
		| "try" "{" stmt_list "}" catch_list "finally" "{" stmt_list "}"
				{ $$ = new AstList(Ast::Type::TRY_LIST); 
				  $$->append($3);
				  size_t ct = $5->size();
				  for (size_t i = 0; i < ct; i++)
				  	  $$->append($5->remove(0));
				  delete $5; 
				  $$->append($8); }
		;

catch_list: %empty
		  		// type is dummy, list is temp
				{ $$ = new AstList(Ast::Type::IF_LIST); }
		  | catch_list catch
				{ $1->append($2); 
				  $$ = $1; }
		  ;

catch: "catch" "(" type_spec ident ")" "{" stmt_list "}"
				{ Ast *decl = new AstFixSize<2>(Ast::Type::DECL_VAR, 
												$3, $4);
				  $$ = new AstFixSize<2>(Ast::Type::CATCH, decl, $7); }
	 | "catch" "(" type_spec ")" "{" stmt_list "}"
				{ Ast *e = new AstScalar(Ast::Type::IDENT, "e");
				  Ast *decl = new AstFixSize<2>(Ast::Type::DECL_VAR,
												$3, e);
				  $$ = new AstFixSize<2>(Ast::Type::CATCH, decl, $6); }
	 ;

%right	"then" "else";

if_list: "if" "(" expr ")" stmt %prec "then"
				{ Ast *cond = new AstFixSize<2>(Ast::Type::IF_CONDBODY,
												$3, $5->bodify().release()); 
				  $$ = new AstList(Ast::Type::IF_LIST);
				  $$->append(cond); }
	   | "if" "(" expr ")" stmt "else" stmt
				{ Ast *cond = new AstFixSize<2>(Ast::Type::IF_CONDBODY,
												$3, $5->bodify().release()); 
				  $$ = new AstList(Ast::Type::IF_LIST);
				  $$->append(cond); 
				  if ($7->getType() == Ast::Type::IF_LIST) {
					  AstList &tail = $7->asList();
					  size_t size = tail.size();
					  for (size_t i = 0; i < size; i++)
						  $$->append(tail.remove(0));
				  } else {
					  $$->append($7->bodify().release());
				  } }
	   ;

/* expression */ 
expr: expr_lv0
				{ $$ = $1; }
	;

expr_lv0: expr_lv1
				{ $$ = $1; }
		| expr_unary "=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASSIGN, $1, $3); } 
		| expr_unary "+=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_ADD, $1, $3); } 
		| expr_unary "-=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_SUB, $1, $3); } 
		| expr_unary "*=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_MUL, $1, $3); } 
		| expr_unary "/=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_DIV, $1, $3); } 
		| expr_unary "%=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_MOD, $1, $3); } 
		| expr_unary "&=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_AND, $1, $3); } 
		| expr_unary "^=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_XOR, $1, $3); } 
		| expr_unary "|=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_OR, $1, $3); } 
		| expr_unary "<<=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_SHL, $1, $3); } 
		| expr_unary ">>=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_SHR, $1, $3); } 
		| expr_unary ">>>=" expr_lv0
				{ $$ = new AstFixSize<2>(Ast::Type::ASS_SHRA, $1, $3); } 
		;

expr_lv1: expr_lv2
				{ $$ = $1; }
		| expr_lv2 "?" expr ":" expr_lv1
				{ $$ = new AstFixSize<3>(Ast::Type::QUESTION, $1, $3, $5); }
		;

expr_lv2: expr_lv3
				{ $$ = $1; }
		| expr_lv2 "||" expr_lv3
				{ $$ = new AstListBop(Ast::Type::LOGIC_OR_BOP_LIST, 
									  $1, $3, Bop::DEFAULT); } 
		;

expr_lv3: expr_lv4
				{ $$ = $1; }
		| expr_lv3 "&&" expr_lv4
				{ $$ = new AstListBop(Ast::Type::LOGIC_AND_BOP_LIST, 
									  $1, $3, Bop::DEFAULT); } 
		;

expr_lv4: expr_lv5
				{ $$ = $1; }
		| expr_lv4 "|" expr_lv5
				{ $$ = new AstFixSize<2>(Ast::Type::BIT_OR, $1, $3); } 
		;

expr_lv5: expr_lv6
				{ $$ = $1; }
		| expr_lv5 "^" expr_lv6
				{ $$ = new AstFixSize<2>(Ast::Type::BIT_XOR, $1, $3); } 
		;

expr_lv6: expr_lv7
				{ $$ = $1; }
		| expr_lv6 "&" expr_lv7
				{ $$ = new AstFixSize<2>(Ast::Type::BIT_AND, $1, $3); } 
		;

expr_lv7: expr_lv8
				{ $$ = $1; }
		| expr_lv7 "==" expr_lv8
				{ $$ = new AstFixSize<2>(Ast::Type::EQ, $1, $3); } 
		| expr_lv7 "!=" expr_lv8
				{ $$ = new AstFixSize<2>(Ast::Type::NEQ, $1, $3); } 
		;

expr_lv8: expr_lv9
				{ $$ = $1; }
		| expr_lv8 "<" expr_lv9
				{ $$ = new AstFixSize<2>(Ast::Type::LT, $1, $3); } 
		| expr_lv8 ">" expr_lv9
				{ $$ = new AstFixSize<2>(Ast::Type::GT, $1, $3); } 
		| expr_lv8 "<=" expr_lv9
				{ $$ = new AstFixSize<2>(Ast::Type::LEQ, $1, $3); } 
		| expr_lv8 ">=" expr_lv9
				{ $$ = new AstFixSize<2>(Ast::Type::GEQ, $1, $3); } 
		| expr_lv8 "instanceof" expr_lv9
				{ $$ = new AstFixSize<2>(Ast::Type::INSTANCEOF, $1, $3); } 
		;

expr_lv9: expr_lv10
				{ $$ = $1; }
		| expr_lv9 "<<" expr_lv10
				{ $$ = new AstFixSize<2>(Ast::Type::SHL, $1, $3); } 
		| expr_lv9 ">>" expr_lv10
				{ $$ = new AstFixSize<2>(Ast::Type::SHR, $1, $3); } 
		| expr_lv9 ">>>" expr_lv10
				{ $$ = new AstFixSize<2>(Ast::Type::SHRA, $1, $3); } 
		;


expr_lv10: expr_lv11
				{ $$ = $1; }
		 | expr_lv10 "+" expr_lv11
				{ $$ = new AstListBop(Ast::Type::ADD_BOP_LIST, $1, $3, 
									  Bop::ADD); }
		 | expr_lv10 "-" expr_lv11
				{ $$ = new AstListBop(Ast::Type::ADD_BOP_LIST, $1, $3, 
									  Bop::SUB); }
		 ;

expr_lv11: expr_lv12
				{ $$ = $1; }
		 | expr_lv11 "*" expr_lv12
				{ $$ = new AstListBop(Ast::Type::MUL_BOP_LIST, $1, $3, 
									  Bop::MUL); }
		 | expr_lv11 "/" expr_lv12
				{ $$ = new AstListBop(Ast::Type::MUL_BOP_LIST, $1, $3, 
									  Bop::DIV); }
		 | expr_lv11 "%" expr_lv12
				{ $$=$1; }
		 ;

expr_lv12: expr_unary
				{ $$ = $1; } 
		 | "(" expr_ptype ")" expr_lv12
				{ $$ = new AstFixSize<2>(Ast::Type::CAST, $2, $4); } 
		 | "(" name dims ")" expr_lv12
				{ Ast *d = new AstListBop($2, $3);
				  $$ = new AstFixSize<2>(Ast::Type::CAST, d, $5); } 
		 | "(" expr ")" expr_unary_logic
				{ $$ = new AstFixSize<2>(Ast::Type::CAST, $2, $4); } 
		 ;

expr_ptype: ptype
				{ $$ = $1; } 
		  | ptype dims
				{ $$ = new AstListBop($1, $2); }
		  ;

expr_unary: "++" expr_unary
				{ $$ = new AstFixSize<1>(Ast::Type::PRE_INC, $2); } 
		  | "--" expr_unary
				{ $$ = new AstFixSize<1>(Ast::Type::PRE_DEC, $2); } 
		  | "+" expr_lv12
				{ $$ = new AstFixSize<1>(Ast::Type::UNARY_PLUS, $2); } 
		  | "-" expr_lv12
				{ $$ = new AstFixSize<1>(Ast::Type::UNARY_MINUS, $2); } 
		  | expr_unary_logic
				{ $$ = $1; }
		  ;

expr_unary_logic: expr_pp
				{ $$ = $1; }
				| "~" expr_unary
				{ $$ = new AstFixSize<1>(Ast::Type::BIT_NOT, $2); } 
				| "!" expr_unary
				{ $$ = new AstFixSize<1>(Ast::Type::LOGIC_NOT, $2); } 
				;

expr_pp: expr_post
				{ $$ = $1; }
	   | expr_prime
				{ $$ = $1; }
	   ;

expr_post: expr_pp "++"
				{ $$ = new AstFixSize<1>(Ast::Type::POST_INC, $1); } 
		 | expr_pp "--"
				{ $$ = new AstFixSize<1>(Ast::Type::POST_DEC, $1); } 
		 ;

expr_prime: name
				{ $$ = $1; }
		  | expr_prime_noname
				{ $$ = $1; }
		  ;

// same as 'NotJustName' in the sample
expr_prime_noname: expr_prime_cx
				{ $$ = $1; }
				 | special_name
				{ $$ = $1; }
				 | expr_new
				{ $$ = $1; }
				 ;

expr_prime_cx: "(" expr ")"
				{ $$ = $2; } 
			 | expr_prime_cx_nude
				{ $$ = $1; }
			 ;

expr_prime_cx_nude: "number"
				{ $$ = new AstScalar(Ast::Type::NUMBER, $1); }
				  | "string"
				{ $$ = new AstScalar(Ast::Type::STRING, $1); }
				  | expr_call
				{ $$ = $1; }
				  | expr_field
				{ $$ = $1; }
				  | name "[" expr "]"
				{ $$ = new AstListBop(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  Bop::ARR); }
				  | expr_prime_cx "[" expr "]"
				{ $$ = new AstListBop(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  Bop::ARR); }
			;

expr_call: callee "(" arg_list ")"
				{ $$ = new AstListBop(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  Bop::CALL); }
	;

expr_field: expr_prime_noname "." ident
				{ $$=$1; }
		  | expr_post "." ident
				{ $$=$1; }
		  ;

expr_new: expr_new_plain
				{ $$ = $1; }
/*
		| name "." expr_new_plain
				{ $$=$1; }
*/
// TODO: what's this? haven't seen... (A.B.C.new D(a, b, c)...?)
		;

expr_new_plain: "new" type_name "(" arg_list ")"
				{ Ast* hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<3>(Ast::Type::NEW_CLASS, 
										 $2, $4, hidden); } 
			  | "new" type_name "(" arg_list ")" "{" member_list "}"
				{ $$ = new AstFixSize<3>(Ast::Type::NEW_CLASS, 
										 $2, $4, $7); } 
// TODO: causes shift-reduce. also not supporting array init
			  | "new" type_name dim_exprs dims
				{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $3->asBopList().addDims($4);
				  $$ = new AstFixSize<3>(Ast::Type::NEW_ARRAY,
										 $2, $3, hidden); }
			  | "new" type_name dim_exprs
				{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  $$ = new AstFixSize<3>(Ast::Type::NEW_ARRAY,
										 $2, $3, hidden); }
			  | "new" type_name dims
				{ Ast *hidden = new AstScalar(Ast::Type::HIDDEN, "");
				  auto dimsA = AstListBop::makeDims($3);
				  $$ = new AstFixSize<3>(Ast::Type::NEW_ARRAY,
										 $2, dimsA.release(), hidden); }
			  ;

dim_exprs: "[" expr "]"
		 		{ $$ = AstListBop::makeDims(std::unique_ptr<Ast>($2)).release(); }
		 | dim_exprs "[" expr "]"
		 		{ $1->asBopList().addDims(std::unique_ptr<Ast>($3));
				  $$ = $1; }
		 ;

callee: expr_prime_cx_nude
				{ $$ = $1; }
	  | special_name
				{ $$ = $1; }
	  | name
				{ $$ = $1; }
	  ;

name: ident
				{ $$ = $1; }
	| name "." ident
				{ $$ = new AstListBop(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  Bop::DOT); }
	;

name_list: name
		 		{ $$ = new AstList(Ast::Type::NAME_LIST);
				  $$->append($1); }
		 | name_list "," name
				{ $1->append($3); $$ = $1; }
		 ;

special_name: "this"
				{ $$= new AstScalar(Ast::Type::IDENT,"this"); }
			| "super"
				{ $$= new AstScalar(Ast::Type::IDENT,"super"); }
			| "null"
				{ $$= new AstScalar(Ast::Type::IDENT,"null"); }
			;

dims: "[]"
				{ $$ = 1; }
	| dims "[]"
				{ $$ = $1 + 1; }
	;

%%

void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

