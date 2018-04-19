CXX=clang++ -std=c++11 -I.

ASTSRC=ast.cpp rootast.cpp listast.cpp boplistast.cpp\
	   fixsizeast.cpp scalarast.cpp internalast.cpp

all: lex.cpp bison.cc
	@${CXX} mian.cpp parser.cpp bison.cc lex.cpp ${ASTSRC}
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

