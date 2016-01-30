#include "ast/rootast.h"

#include <cassert>

RootAst::RootAst() :
    ListAst(Type::CLASS_LIST)
{
    parent = this;
}
