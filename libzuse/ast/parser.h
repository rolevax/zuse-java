#ifndef ZUSE_PARSER_H
#define ZUSE_PARSER_H

#include "bison.hh"

#include <string>

class Ast;

namespace yaloe
{
AstRoot *parse(const std::string& f, bool traceLex = false, 
									 bool traceParse = false);

}

#endif // ZUSE_PARSER_H

