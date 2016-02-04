#include "ast/declbeanast.h"

#include <cassert>

DeclBeanAst &DeclBeanAst::fromAst(Ast &a)
{
    assert(a.getType() == Type::DECL_BEAN);
    return static_cast<DeclBeanAst&>(a);
}

const DeclBeanAst &DeclBeanAst::fromAst(const Ast &a)
{
    assert(a.getType() == Type::DECL_BEAN);
    return static_cast<const DeclBeanAst&>(a);
}

DeclBeanAst::DeclBeanAst(Ast *id, Ast *init)
    : InternalAst(Type::DECL_BEAN)
{
    change(0, id);
    change(1, init);
}

size_t DeclBeanAst::size() const
{
    return init ? 2 : 1;
}

Ast &DeclBeanAst::at(size_t pos) const
{
    assert(pos < size());
    return pos == 0 ? *id : *init;
}

size_t DeclBeanAst::indexOf(const Ast *child) const
{
    if (child == id.get())
        return 0;
    else if (child == init.get())
        return 1;
    else
        return assert(false && "child not found"), 0;
}

void DeclBeanAst::doChange(size_t pos, Ast *next)
{
    assert(pos == 0 || pos == 1);
    if (pos == 0)
        id.reset(&ScalarAst::fromAst(*next));
    else
        init.reset(next);
}
