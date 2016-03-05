#include "ast/ast.h"
#include "ast/scalarast.h"
#include "ast/fixsizeast.h"
#include "ast/internalast.h"
#include "ast/listast.h"
#include "ast/boplistast.h"

#include <cassert>

Ast::Ast(Type t) :
    type(t),
    parent(nullptr)
{

}

bool Ast::isList() const
{
    return Type::CLASS_LIST <= type && type <= Type::ADD_BOP_LIST;
}

bool Ast::isBopList() const
{
    return Type::DOT_BOP_LIST <= type && type <= Type::ADD_BOP_LIST;
}

bool Ast::isMap() const
{
    return Type::DECL_CLASS <= type && type <= Type::ASSIGN;
}

bool Ast::isScalar() const
{
    return !isList() && !isMap();
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

template<std::size_t N>
const FixSizeAst<N> &Ast::asFixSize() const
{
    // TODO
    // assert(isFixSize());
    return static_cast<const FixSizeAst<N>&>(*this);
}

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

BopListAst &Ast::asBopList()
{
    assert(isBopList());
    return static_cast<BopListAst&>(*this);
}

const BopListAst &Ast::asBopList() const
{
    assert(isBopList());
    return static_cast<const BopListAst&>(*this);
}

ListAst *Ast::bodify()
{
    if (getType() == Type::STMT_LIST)
        return &this->asList();

    ListAst *ret = new ListAst(Type::STMT_LIST);
    ret->append(this);
    return ret;
}

int Ast::precedence() const
{
    switch (type) {
    case Type::ASSIGN:
        return 5;
    case Type::ADD_BOP_LIST:
        return 10;
    case Type::MUL_BOP_LIST:
        return 11;
    default:
        return 0;
    }
}

Ast::Type Ast::getType() const
{
    return type;
}

InternalAst &Ast::getParent() const
{
    return *parent;
}

void Ast::setParent(InternalAst *next)
{
    parent = next;
}

int Ast::indentLevel() const
{
    int ret = 0;
    Type ptype = parent->type;
    bool hasIndent = ptype == Type::MEMBER_LIST
            || ptype == Type::STMT_LIST;
    if (hasIndent)
        for (const Ast *a = parent; a->parent != a; a = &a->getParent())
            if (a->getType() == Type::STMT_LIST
                    || a->getType() == Type::MEMBER_LIST)
                ++ret;
    return ret;
}

// instanciation
template FixSizeAst<1> &Ast::asFixSize();
template FixSizeAst<2> &Ast::asFixSize();
template FixSizeAst<3> &Ast::asFixSize();
template FixSizeAst<4> &Ast::asFixSize();

// instanciation
template const FixSizeAst<1> &Ast::asFixSize() const;
template const FixSizeAst<2> &Ast::asFixSize() const;
template const FixSizeAst<3> &Ast::asFixSize() const;
template const FixSizeAst<4> &Ast::asFixSize() const;


