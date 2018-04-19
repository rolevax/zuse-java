#include "rootast.h"

#include <cassert>

RootAst::RootAst() :
    ListAst(Type::CLASS_LIST)
{
    setParent(this);
}
