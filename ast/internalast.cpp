#include "ast/internalast.h"

#include <cassert>

InternalAst::InternalAst(Type t)
    : Ast(t)
{
    assert(!isScalar());
}

void InternalAst::change(size_t pos, Ast *next)
{
    if (next != nullptr)
        next->setParent(this);
    doChange(pos, next);
}

Ast::Type InternalAst::typeAt(size_t pos)
{
    // mainly for fix-size trees, don't reuse too much
    switch (getType()) {
    case Type::DECL_PARAM_LIST:
        return Type::DECL_PARAM;
    case Type::DECL_CLASS:
        return pos == 0 ? Type::IDENT : Type::STMT_LIST;
    default:
        return Type::META;
    }
}

