#include "ast/classast.h"

#include <cassert>

ClassAst &ClassAst::fromAst(Ast &a)
{
    assert(a.getType() == Ast::Type::CLASS);
    return static_cast<ClassAst&>(a);
}

const ClassAst &ClassAst::fromAst(const Ast &a)
{
    assert(a.getType() == Ast::Type::CLASS);
    return static_cast<const ClassAst&>(a);
}

ClassAst::ClassAst(const std::string &id, ListAst *methods)
    : Ast(Ast::Type::CLASS)
{
    change(0, new ScalarAst(Type::IDENT, id));
    change(1, methods);
}

size_t ClassAst::size() const
{
    return 2;
}

Ast &ClassAst::at(size_t pos) const
{
    assert(pos == 0 || pos == 1);
    return pos == 0 ? (Ast&) *id : (Ast&) *methods;
}

size_t ClassAst::indexOf(const Ast *child) const
{
    if (child == id.get())
        return 0;
    else if (child == methods.get())
        return 1;
    else
        return assert(false && "child not found"), 0;
}

void ClassAst::doChange(size_t pos, Ast *next)
{
    assert(pos == 0 || pos == 1);
    if (pos == 0)
        id.reset(&ScalarAst::fromAst(*next));
    else
        methods.reset(&ListAst::fromAst(*next));
}
