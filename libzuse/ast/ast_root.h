#ifndef ZUSE_AST_ROOT_H
#define ZUSE_AST_ROOT_H

#include "ast_list.h"

namespace zuse
{



///
/// \brief Root node of the abstract syntax tree
///
class AstRoot : public AstList
{
public:
    AstRoot();
};



} // namespace zuse



#endif // ZUSE_AST_ROOT_H
