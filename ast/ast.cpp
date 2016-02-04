#include "ast/ast.h"
#include "ast/scalarast.h"
#include "ast/fixsizeast.h"
#include "ast/internalast.h"
#include "ast/listast.h"

#include <cassert>

Ast::Ast(Type t) :
    type(t),
    parent(nullptr)
{

}

bool Ast::isList() const
{
    return Type::CLASS_LIST <= type && type <= Type::MULDIV_LIST;
}

bool Ast::isMap() const
{
    return Type::CLASS <= type && type <= Type::CALL;
}

bool Ast::isScalar() const
{
    return !isList() && !isMap();
}

bool Ast::isChangeable() const
{
    //return type != Type::KEY && type != Type::PAIR && type != Type::ROOT;
    return false; // TODO
}

ScalarAst &Ast::asScalar()
{
    assert(isScalar());
    return static_cast<ScalarAst&>(*this);
}

const ScalarAst &Ast::asScalar() const
{
    assert(isScalar());
    return static_cast<const ScalarAst&>(*this);
}

InternalAst &Ast::asInternal()
{
    // TODO assert
    return static_cast<InternalAst&>(*this);
}

const InternalAst &Ast::asInternal() const
{
    // TODO assert
    return static_cast<const InternalAst&>(*this);
}

template<std::size_t N>
FixSizeAst<N> &Ast::asFixSize()
{
    // TODO
    // assert(isFixSize());
    return static_cast<FixSizeAst<N>&>(*this);
}

template FixSizeAst<1> &Ast::asFixSize();
template FixSizeAst<2> &Ast::asFixSize();
template FixSizeAst<3> &Ast::asFixSize();

template<std::size_t N>
const FixSizeAst<N> &Ast::asFixSize() const
{
    // TODO
    // assert(isFixSize());
    return static_cast<const FixSizeAst<N>&>(*this);
}

template const FixSizeAst<1> &Ast::asFixSize() const;
template const FixSizeAst<2> &Ast::asFixSize() const;
template const FixSizeAst<3> &Ast::asFixSize() const;

ListAst &Ast::asList()
{
    assert(isList());
    return static_cast<ListAst&>(*this);
}

const ListAst &Ast::asList() const
{
    assert(isList());
    return static_cast<const ListAst&>(*this);
}

Ast::Type Ast::getType() const
{
    return type;
}

InternalAst &Ast::getParent() const
{
    return *parent;
}

int Ast::indentLevel() const
{
    int ret = 0;
    Type ptype = parent->type;
    bool hasIndent = ptype == Type::METHOD_LIST
            || ptype == Type::STMT_LIST;
    if (hasIndent)
        for (const Ast *a = this; a->parent != a; a = &a->getParent())
            ret += a->isList();
    return ret;
}




