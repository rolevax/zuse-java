#include "ast/rootast.h"

#include <cassert>

RootAst::RootAst() :
    ListAst(Type::CLASS_LIST)
{
    // TODO bad design
    parent = this;
}
