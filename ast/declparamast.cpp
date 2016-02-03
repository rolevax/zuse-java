#include "ast/declparamast.h"

#include <cassert>

DeclParamAst::DeclParamAst(Ast *type, const std::string &id)
    : Ast(Type::DECL_PARAM)
{
    change(0, type);
    change(1, new ScalarAst(Type::IDENT, id));
}

size_t DeclParamAst::size() const
{
    return 2;
}

Ast &DeclParamAst::at(size_t pos) const
{
    assert(pos < size());
    return pos == 0 ? *type : *id;
}

size_t DeclParamAst::indexOf(const Ast *child) const
{
    if (child == type.get())
        return 0;
    else if (child == id.get())
        return 1;
    else
        return assert(false && "child not found"), 0;
}

void DeclParamAst::doChange(size_t pos, Ast *next)
{
    assert(pos < size());
    if (pos == 0)
        type.reset(next);
    else
        id.reset(&ScalarAst::fromAst(*next));
}
