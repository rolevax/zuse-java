CXX=clang++ -std=c++11 -I. -I../include 
ASTSRC=ast.cpp rootast.cpp listast.cpp termlistast.cpp\
	   fixsizeast.cpp declbeanast.cpp scalarast.cpp internalast.cpp

all: parser
	@${CXX} mian.cpp parser.cpp bison.cc lex.cpp ${ASTSRC}
	@echo 'Test harness generated'

parser: bison lex
	@echo 'Bison/Flex parser generated'

bison: 
	@bison -o bison.cc bison.yy
	@cp *.hh ../include/ast
	@echo 'Headers copied to ../include/ast'

lex: 
	@flex -o lex.cpp lex.ll 

clean:
	rm -rf a.out
	rm -rf bison.hh location.hh position.hh stack.hh bison.cc
	rm -rf lex.cpp

