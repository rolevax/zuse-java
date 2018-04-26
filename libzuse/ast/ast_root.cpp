#include "ast_root.h"

#include <cassert>



namespace zuse
{



AstRoot::AstRoot() :
    AstList(Type::CLASS_LIST)
{
    setParent(this);
}



} // namespace zuse
