#include "ast/bopast.h"

#include <cassert>

BopAst &BopAst::fromAst(Ast &a)
{
    // TODO: add assert
    return static_cast<BopAst&>(a);
}

const BopAst &BopAst::fromAst(const Ast &a)
{
    // TODO: add assert
    return static_cast<const BopAst&>(a);
}

BopAst::BopAst(Type t, Ast *l, Ast *r)
    : Ast(t)
{
    change(0, l);
    change(1, r);
}

size_t BopAst::size() const
{
    return 2;
}

Ast &BopAst::at(size_t pos) const
{
    assert(pos < size());
    return pos == 0 ? *lhs : *rhs;
}

size_t BopAst::indexOf(const Ast *child) const
{
    if (child == lhs.get())
        return 0;
    else if (child == rhs.get())
        return 1;
    else
        return assert(false && "child not found"), 0;
}

void BopAst::doChange(size_t pos, Ast *next)
{
    assert(pos < size());
    if (pos == 0)
        lhs.reset(next);
    else
        rhs.reset(next);
}
