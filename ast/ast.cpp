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

bool Ast::isList(Ast::Type type)
{
    return Type::CLASS_LIST <= type && type <= Type::ADD_BOP_LIST;
}

bool Ast::isBopList(Ast::Type type)
{
    return Type::DOT_BOP_LIST <= type && type <= Type::ADD_BOP_LIST;
}

bool Ast::isFixSize(Ast::Type type, size_t s)
{
    switch (s) {
    case 0:
        return Type::RETURN <= type && type <= Type::DECL_METHOD;
    case 1:
        return Type::RETURN <= type && type <= Type::RETURN;
    case 2:
        return Type::DECL_CLASS <= type && type <= Type::ASSIGN;
    case 4:
        return Type::DECL_METHOD <= type && type <= Type::DECL_METHOD;
    default:
        return false;
    }
}

bool Ast::isScalar(Ast::Type type)
{
    return !isList(type) && !isFixSize(type);
}

bool Ast::isList() const
{
    return isList(type);
}

bool Ast::isBopList() const
{
    return isBopList(type);
}

bool Ast::isFixSize(size_t s) const
{
    return isFixSize(type, s);
}

bool Ast::isScalar() const
{
    return isScalar(type);
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
    assert(!isScalar());
    return static_cast<InternalAst&>(*this);
}

const InternalAst &Ast::asInternal() const
{
    assert(!isScalar());
    return static_cast<const InternalAst&>(*this);
}

template<std::size_t N>
FixSizeAst<N> &Ast::asFixSize()
{
    assert(isFixSize(N));
    return static_cast<FixSizeAst<N>&>(*this);
}

template<std::size_t N>
const FixSizeAst<N> &Ast::asFixSize() const
{
    assert(isFixSize(N));
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
    case Type::DOT_BOP_LIST:
        return 15;
    case Type::MUL_BOP_LIST:
        return 12;
    case Type::ADD_BOP_LIST:
        return 11;
    case Type::LT:
    case Type::LEQ:
    case Type::GT:
    case Type::GEQ:
    case Type::INSTANCEOF:
        return 9;
    case Type::EQ:
    case Type::NEQ:
        return 8;
    case Type::BIT_AND:
        return 7;
    case Type::BIT_XOR:
        return 6;
    case Type::BIT_OR:
        return 5;
    case Type::LOGIC_AND:
        return 4;
    case Type::LOGIC_OR:
        return 3;
    case Type::ASSIGN:
        return 1;
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

bool Ast::hasIndent() const
{
    Type ptype = parent->type;
    return ptype == Type::MEMBER_LIST || ptype == Type::STMT_LIST;
}

int Ast::indentLevel() const
{
    int ret = 0;
    for (const Ast *a = parent;
         a->parent != a && a->parent != nullptr;
         a = &a->getParent())
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


