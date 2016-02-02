#include "ast/declast.h"

#include <cassert>

DeclAst::DeclAst(Ast *type, Ast *bean)
    : Ast(Type::DECL)
{
    change(0, type);
    change(1, bean);
}

size_t DeclAst::size() const
{
    return 2;
}

Ast &DeclAst::at(size_t pos) const
{
    assert(pos < size());
    return pos == 0 ? *type : *bean;
}

size_t DeclAst::indexOf(const Ast *child) const
{
    if (child == type.get())
        return 0;
    else if (child == bean.get())
        return 1;
    else
        return assert(false && "child not found"), 0;
}

void DeclAst::doChange(size_t pos, Ast *next)
{
    assert(pos < size());
    if (pos == 0)
        type.reset(next);
    else
        bean.reset(next);
}
