#include "ast/ast.h"

#include <cassert>

Ast::Ast(Type t) :
    type(t),
    parent(t == Type::CLASS_LIST ? this : nullptr)
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

Ast::Type Ast::getType() const
{
    return type;
}

Ast &Ast::getParent() const
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



