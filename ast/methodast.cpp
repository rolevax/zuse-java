#include "ast/methodast.h"

#include <cassert>

MethodAst &MethodAst::fromAst(Ast &a)
{
    assert(a.getType() == Type::METHOD);
    return static_cast<MethodAst&>(a);
}

const MethodAst &MethodAst::fromAst(const Ast &a)
{
    assert(a.getType() == Type::METHOD);
    return static_cast<const MethodAst&>(a);
}

MethodAst::MethodAst(const std::string &id,
                     ListAst *params, ListAst *stmts)
    : Ast(Type::METHOD)
{
    change(0, new ScalarAst(Type::IDENT, id));
    change(1, params);
    change(2, stmts);
}

size_t MethodAst::size() const
{
    return 3;
}

Ast &MethodAst::at(size_t pos) const
{
    assert(pos < size());
    if (pos == 0)
        return *id;
    else if (pos == 1)
        return *params;
    else
        return *stmts;
}

size_t MethodAst::indexOf(const Ast *child) const
{
    if (child == id.get())
        return 0;
    else if (child == params.get())
        return 1;
    else if (child == stmts.get())
        return 2;
    else
        return assert(false && "child not found"), 0;
}

void MethodAst::doChange(size_t pos, Ast *next)
{
    assert(pos < size());
    if (pos == 0)
        id.reset(&ScalarAst::fromAst(*next));
    else if (pos == 1)
        params.reset(&ListAst::fromAst(*next));
    else
        stmts.reset(&ListAst::fromAst(*next));
}
