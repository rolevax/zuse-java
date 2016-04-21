CXX=clang++ -std=c++11 -I. -I../include 
ASTSRC=ast.cpp rootast.cpp listast.cpp boplistast.cpp\
	   fixsizeast.cpp scalarast.cpp internalast.cpp

all: lex.cpp bison.cc
	@${CXX} mian.cpp parser.cpp bison.cc lex.cpp ${ASTSRC}
	@echo 'Test harness generated'

bison.cc: 
	@bison -o bison.cc bison.yy
	@cp *.hh ../include/ast
	@echo 'Bison parser generated, headers copied to ../include/ast'

lex.cpp: 
	@flex -o lex.cpp lex.ll 
	@echo 'Flex lexer generated'

clean:
	rm -rf a.out
	rm -rf bison.hh location.hh position.hh stack.hh bison.cc
	rm -rf lex.cpp

