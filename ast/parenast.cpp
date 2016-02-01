#include "ast/parenast.h"

#include <cassert>

ParenAst &ParenAst::fromAst(Ast &a)
{
    assert(a.getType() == Type::PAREN);
    return static_cast<ParenAst&>(a);
}

const ParenAst &ParenAst::fromAst(const Ast &a)
{
    assert(a.getType() == Type::PAREN);
    return static_cast<const ParenAst&>(a);
}

ParenAst::ParenAst(Ast *sub)
    : Ast(Type::PAREN)
{
    change(0, sub);
}

size_t ParenAst::size() const
{
    return 1;
}

Ast &ParenAst::at(size_t pos) const
{
    assert(pos < size());
    return *subtree;
}

size_t ParenAst::indexOf(const Ast *child) const
{
    if (child == subtree.get())
        return 0;
    else
        return assert(false && "child not found"), 0;
}

void ParenAst::doChange(size_t pos, Ast *next)
{
    assert(pos < size());
    subtree.reset(next);
}
