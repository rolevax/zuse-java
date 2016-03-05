#include "ast/internalast.h"
#include "ast/boplistast.h"

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

Ast::Type InternalAst::typeAt(size_t pos)
{
    switch (getType()) {
    case Type::DECL_PARAM_LIST:
        return Type::DECL_PARAM;
    case Type::DECTOR_LIST:
        return Type::META;
    default:
        return Type::META;
    }
}

