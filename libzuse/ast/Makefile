CXX=clang++ -std=c++1z -I.

ASTSRC=ast.cpp ast_root.cpp ast_list.cpp ast_list_bop.cpp\
	   ast_fix_size.cpp ast_scalar.cpp ast_internal.cpp

all: lex.cpp bison.cc
	@${CXX} main.cpp parser.cpp bison.cc lex.cpp ${ASTSRC}
	@echo 'Test harness generated'

bison.cc: bison.yy
	@bison -o bison.cc bison.yy
	@echo 'Bison parser generated'

lex.cpp: lex.ll
	@flex -o lex.cpp lex.ll 
	@echo 'Flex lexer generated'

clean:
	rm -rf a.out
	rm -rf bison.hh location.hh position.hh stack.hh bison.cc
	rm -rf lex.cpp

