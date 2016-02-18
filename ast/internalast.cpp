#include "ast/internalast.h"

InternalAst::InternalAst(Type t)
    : Ast(t)
{

}

void InternalAst::change(size_t pos, Ast *next)
{
    if (next != nullptr)
        next->setParent(this);
    doChange(pos, next);
}

