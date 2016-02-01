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

void Ast::insert(size_t pos, Ast *child)
{
    child->parent = this;
    doInsert(pos, child);
}

void Ast::append(Ast *subtree)
{
    insert(size(), subtree);
}

std::unique_ptr<Ast> Ast::remove(size_t pos)
{
    (void) pos;
    throw 2048;
}

void Ast::change(size_t pos, Ast *next)
{
    next->parent = this;
    doChange(pos, next);
}

void Ast::nest(size_t pos, Ast *nester)
{
    //assert(nester->type == Type::ARRAY && nester->size() == 0);
    // TODO

    std::unique_ptr<Ast> nestee = remove(pos);
    insert(pos, nester);
    at(pos).insert(0, nestee.release());
}

size_t Ast::size() const
{
    return 0;
}

Ast &Ast::at(size_t pos) const
{
    (void) pos;
    throw 2048;
}

size_t Ast::indexOf(const Ast *child) const
{
    (void) child;
    return -1;
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

void Ast::doInsert(size_t pos, Ast *child)
{
    (void) pos;
    (void) child;
    throw 2048;
}

void Ast::doChange(size_t pos, Ast *next)
{
    (void) pos;
    (void) next;
    throw 2048;
}



