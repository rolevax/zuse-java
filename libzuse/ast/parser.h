#ifndef CALCXX_DRIVER_HH
#define CALCXX_DRIVER_HH

#include "bison.hh"

#include <string>

class Ast;

namespace yaloe
{
RootAst *parse(const std::string& f, bool traceLex = false, 
									 bool traceParse = false);

};
#endif // ! CALCXX_DRIVER_HH

