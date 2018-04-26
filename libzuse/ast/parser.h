#ifndef ZUSE_PARSER_H
#define ZUSE_PARSER_H

#include "bison.hh"

#include <string>



namespace zuse
{



class Ast;

AstRoot *parse(const std::string& f, bool traceLex = false, bool traceParse = false);



} // namespace zuse



#endif // ZUSE_PARSER_H

