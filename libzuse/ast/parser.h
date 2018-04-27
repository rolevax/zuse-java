#ifndef ZUSE_PARSER_H
#define ZUSE_PARSER_H

#include "bison.hh"

#include <string>
#include <memory>



namespace zuse
{



class Ast;

std::unique_ptr<AstRoot> parse(const std::string& f, bool traceLex = false, bool traceParse = false);



} // namespace zuse



#endif // ZUSE_PARSER_H

