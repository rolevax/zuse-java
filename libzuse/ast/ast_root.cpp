#include "ast_root.h"

#include <cassert>

AstRoot::AstRoot() :
    AstList(Type::CLASS_LIST)
{
    setParent(this);
}
