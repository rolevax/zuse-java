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
    case Type::COMMA_LIST:
        if (getParent().getType() == Type::DECL_METHOD)
            // TODO DECL_METHOD_TH and DECL_METHOD_NOTH
            return Type::DECL_PARAM;
        else
            return Type::META;
    default:
        return Type::META;
    }
}

