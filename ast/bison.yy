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
	#include "ast/boplistast.h"
	#include "ast/rootast.h"
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

	CLASS		"class"
	RETURN		"return"
	WHILE		"while"
	DO			"do"
	IF			"if"
	ELSE		"else"
	INSTANCEOF	"instanceof"
	JNULL		"null"
	THIS		"this"
	SUPER		"super"
	NEW			"new"
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

%type	<ListAst*>		class_list
%type	<ListAst*>		member_list
%type	<ListAst*>		param_list
%type	<ListAst*>		param_list_noemp
%type	<ListAst*>		stmt_list
%type	<ListAst*>		if_list
%type	<ListAst*>		arg_list
%type	<ListAst*>		arg_list_noemp
%type	<ListAst*>		dector_list

%type	<Ast*>			class
%type	<Ast*>			decl_method
%type	<Ast*>			stmt
%type	<Ast*>			decl_var
%type	<Ast*>			decl_param
%type	<Ast*>			dector
%type	<Ast*>			dector_name
%type	<Ast*>			return_stmt
%type	<Ast*>			while_stmt
%type	<Ast*>			do_while_stmt
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
//TODO: dims
	 			{ $$ = $1; }
		 ;

type_name: ptype
	 			{ $$ = $1; }
		 | name
	 			{ $$ = $1; }
		 ;

ptype: "void"
	 			{ $$ = new ScalarAst(Ast::Type::IDENT, $1); }
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
	 			{ $$ = new FixSizeAst<2>(Ast::Type::DECL_CLASS, $2, $4); }
	 | modifiers "class" ident "{" member_list "}"	
	 			{ auto a = new FixSizeAst<2>(Ast::Type::DECL_CLASS, $3, $5);
				  a->setModifiers($1); $$ = a; }
	 ;

member_list: %empty					
		   		{ $$ = new ListAst(Ast::Type::MEMBER_LIST); }
		   | member_list decl_method
				{ $1->append($2); $$ = $1; }
		   | member_list decl_var
				{ $1->append($2); $$ = $1; }
		   ;

decl_method: type_spec dector_name "(" param_list ")" "{" stmt_list "}"
 				{ $$ = new FixSizeAst<4>(Ast::Type::DECL_METHOD, 
										 $1, $2, $4, $7); }
           | modifiers type_spec dector_name "(" param_list ")" 
				"{" stmt_list "}"
 				{ $$ = new FixSizeAst<4>(Ast::Type::DECL_METHOD, 
										 $2, $3, $5, $8); 
				  $$->asFixSize<4>().setModifiers($1); }
// TODO: dim in method decl (?)
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

decl_param: ident ident
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_PARAM, $1, $2); } 
		  | "final" ident ident
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_PARAM, $2, $3); 
				  $$->asFixSize<2>().getModifiers().final = true; } 
		  ;

stmt_list: %empty
		 		{ $$ = new ListAst(Ast::Type::STMT_LIST); }
		 | stmt_list stmt
				{ $1->append($2); $$ = $1; }
		 ;

stmt: expr ";"
				{ $$ = $1; }
	| decl_var
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

ident: "identifier"
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

decl_var: type_spec dector ";"
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_VAR, $1, $2); }
		| type_spec dector_list ";" 
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_VAR, $1, $2); }
		| modifiers type_spec dector ";" 
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_VAR, $2, $3); 
				  $$->asFixSize<2>().setModifiers($1); }
		| modifiers type_spec dector_list ";" 
		 		{ $$ = new FixSizeAst<2>(Ast::Type::DECL_VAR, $2, $3); 
				  $$->asFixSize<2>().setModifiers($1); }
		;

dector_list: dector "," dector
		 		{ $$ = new ListAst(Ast::Type::DECTOR_LIST); 
				  $$->append($1); $$->append($3); }
		   | dector_list "," dector
		 		{ $1->append($3); $$ = $1; }
		   ;

dector: dector_name
				{ $$ = $1; }
	  | dector_name "=" expr // TODO: array init
				// TODO: change to ASS_BOP_LIST
				{ $$ = new FixSizeAst<2>(Ast::Type::ASSIGN, $1, $3); } 
	  ;

dector_name: ident
				{ $$ = $1; }
		   | dector_name "[]"
				{ $$ = $1; } // TODO: scalar.dim++ (?)
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
					  ListAst &tail = $7->asList();
					  size_t size = tail.size();
					  for (size_t i = 0; i < size; i++)
						  $$->append(tail.remove(0));
				  } else {
					  $$->append($7->bodify());
				  } }
	   ;

/* expression */ 
expr: expr_lv0
				{ $$ = $1; }
	;

expr_lv0: expr_lv1
				{ $$ = $1; }
		| expr_unary "=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASSIGN, $1, $3); } 
		| expr_unary "+=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_ADD, $1, $3); } 
		| expr_unary "-=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_SUB, $1, $3); } 
		| expr_unary "*=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_MUL, $1, $3); } 
		| expr_unary "/=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_DIV, $1, $3); } 
		| expr_unary "%=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_MOD, $1, $3); } 
		| expr_unary "&=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_AND, $1, $3); } 
		| expr_unary "^=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_XOR, $1, $3); } 
		| expr_unary "|=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_OR, $1, $3); } 
		| expr_unary "<<=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_SHL, $1, $3); } 
		| expr_unary ">>=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_SHR, $1, $3); } 
		| expr_unary ">>>=" expr_lv0
				{ $$ = new FixSizeAst<2>(Ast::Type::ASS_SHRA, $1, $3); } 
		;

expr_lv1: expr_lv2
				{ $$ = $1; }
		| expr_lv2 "?" expr ":" expr_lv1
				{ $$ = new FixSizeAst<3>(Ast::Type::QUESTION, $1, $3, $5); }
		;

expr_lv2: expr_lv3
				{ $$ = $1; }
		| expr_lv2 "||" expr_lv3
				{ $$ = new BopListAst(Ast::Type::LOGIC_OR_BOP_LIST, 
									  $1, $3, BopListAst::DEFAULT); } 
		;

expr_lv3: expr_lv4
				{ $$ = $1; }
		| expr_lv3 "&&" expr_lv4
				{ $$ = new BopListAst(Ast::Type::LOGIC_AND_BOP_LIST, 
									  $1, $3, BopListAst::DEFAULT); } 
		;

expr_lv4: expr_lv5
				{ $$ = $1; }
		| expr_lv4 "|" expr_lv5
				{ $$ = new FixSizeAst<2>(Ast::Type::BIT_OR, $1, $3); } 
		;

expr_lv5: expr_lv6
				{ $$ = $1; }
		| expr_lv5 "^" expr_lv6
				{ $$ = new FixSizeAst<2>(Ast::Type::BIT_XOR, $1, $3); } 
		;

expr_lv6: expr_lv7
				{ $$ = $1; }
		| expr_lv6 "&" expr_lv7
				{ $$ = new FixSizeAst<2>(Ast::Type::BIT_AND, $1, $3); } 
		;

expr_lv7: expr_lv8
				{ $$ = $1; }
		| expr_lv7 "==" expr_lv8
				{ $$ = new FixSizeAst<2>(Ast::Type::EQ, $1, $3); } 
		| expr_lv7 "!=" expr_lv8
				{ $$ = new FixSizeAst<2>(Ast::Type::NEQ, $1, $3); } 
		;

expr_lv8: expr_lv9
				{ $$ = $1; }
		| expr_lv8 "<" expr_lv9
				{ $$ = new FixSizeAst<2>(Ast::Type::LT, $1, $3); } 
		| expr_lv8 ">" expr_lv9
				{ $$ = new FixSizeAst<2>(Ast::Type::GT, $1, $3); } 
		| expr_lv8 "<=" expr_lv9
				{ $$ = new FixSizeAst<2>(Ast::Type::LEQ, $1, $3); } 
		| expr_lv8 ">=" expr_lv9
				{ $$ = new FixSizeAst<2>(Ast::Type::GEQ, $1, $3); } 
		| expr_lv8 "instanceof" expr_lv9
				{ $$ = new FixSizeAst<2>(Ast::Type::INSTANCEOF, $1, $3); } 
		;

expr_lv9: expr_lv10
				{ $$ = $1; }
		| expr_lv9 "<<" expr_lv10
				{ $$ = new FixSizeAst<2>(Ast::Type::SHL, $1, $3); } 
		| expr_lv9 ">>" expr_lv10
				{ $$ = new FixSizeAst<2>(Ast::Type::SHR, $1, $3); } 
		| expr_lv9 ">>>" expr_lv10
				{ $$ = new FixSizeAst<2>(Ast::Type::SHRA, $1, $3); } 
		;


expr_lv10: expr_lv11
				{ $$ = $1; }
		 | expr_lv10 "+" expr_lv11
				{ $$ = new BopListAst(Ast::Type::ADD_BOP_LIST, $1, $3, 
									  BopListAst::ADD); }
		 | expr_lv10 "-" expr_lv11
				{ $$ = new BopListAst(Ast::Type::ADD_BOP_LIST, $1, $3, 
									  BopListAst::SUB); }
		 ;

expr_lv11: expr_lv12
				{ $$ = $1; }
		 | expr_lv11 "*" expr_lv12
				{ $$ = new BopListAst(Ast::Type::MUL_BOP_LIST, $1, $3, 
									  BopListAst::MUL); }
		 | expr_lv11 "/" expr_lv12
				{ $$ = new BopListAst(Ast::Type::MUL_BOP_LIST, $1, $3, 
									  BopListAst::DIV); }
		 | expr_lv11 "%" expr_lv12
				{ $$=$1; }
		 ;

expr_lv12: expr_unary
				{ $$ = $1; } 
		 | "(" expr_ptype ")" expr_lv12
				{ $$ = new FixSizeAst<2>(Ast::Type::CAST, $2, $4); } 
		 | "(" name dims ")" expr_lv12
//TODO: dims
				{ $$ = new FixSizeAst<2>(Ast::Type::CAST, $2, $5); } 
		 | "(" expr ")" expr_unary_logic
				{ $$ = new FixSizeAst<2>(Ast::Type::CAST, $2, $4); } 
		 ;

expr_ptype: ptype
				{ $$ = $1; } 
		  | ptype dims
				{ $$=$1; } 
		  ;

expr_unary: "++" expr_unary
				{ $$ = new FixSizeAst<1>(Ast::Type::PRE_INC, $2); } 
		  | "--" expr_unary
				{ $$ = new FixSizeAst<1>(Ast::Type::PRE_DEC, $2); } 
		  | "+" expr_lv12
				{ $$ = new FixSizeAst<1>(Ast::Type::UNARY_PLUS, $2); } 
		  | "-" expr_lv12
				{ $$ = new FixSizeAst<1>(Ast::Type::UNARY_MINUS, $2); } 
		  | expr_unary_logic
				{ $$ = $1; }
		  ;

expr_unary_logic: expr_pp
				{ $$ = $1; }
				| "~" expr_unary
				{ $$ = new FixSizeAst<1>(Ast::Type::BIT_NOT, $2); } 
				| "!" expr_unary
				{ $$ = new FixSizeAst<1>(Ast::Type::LOGIC_NOT, $2); } 
				;

expr_pp: expr_post
				{ $$ = $1; }
	   | expr_prime
				{ $$ = $1; }
	   ;

expr_post: expr_pp "++"
				{ $$ = new FixSizeAst<1>(Ast::Type::POST_INC, $1); } 
		 | expr_pp "--"
				{ $$ = new FixSizeAst<1>(Ast::Type::POST_DEC, $1); } 
		 ;

expr_prime: name
				{ $$ = $1; }
		  | expr_prime_noname
				{ $$ = $1; }
		  ;

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
				{ $$ = new ScalarAst(Ast::Type::NUMBER, $1); }
				  | "string"
				{ $$ = new ScalarAst(Ast::Type::STRING, $1); }
				  | expr_call
				{ $$ = $1; }
				  | expr_field
				{ $$ = $1; }
			;

expr_call: callee "(" arg_list ")"
				{ $$ = new BopListAst(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  BopListAst::CALL); }
	;

expr_field: expr_prime_noname "." ident
				{ $$=$1; }
		  | expr_post "." ident
				{ $$=$1; }
		  ;

expr_new: expr_new_plain
				{ $$ = $1; }
		| name "." expr_new_plain
				{ $$=$1; }
		;

expr_new_plain: "new" type_name "(" arg_list ")"
				{ $$=$2; }
			  // TODO various new
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
				{ $$ = new BopListAst(Ast::Type::DOT_BOP_LIST, $1, $3, 
									  BopListAst::DOT); }
	;

special_name: "this"
				{ $$= new ScalarAst(Ast::Type::IDENT,"xxx"); }
			| "super"
				{ $$= new ScalarAst(Ast::Type::IDENT,"xxx"); }
			| "null"
				{ $$= new ScalarAst(Ast::Type::IDENT,"xxx"); }
			;

dims: "[]"
	| dims "[]"
	;

%%

void yy::BisonParser::error(const location_type& l,
                              const std::string& m)
{
	throw ParseException(l, m);
}

