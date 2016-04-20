#include "ast/internalast.h"
#include "ast/listast.h"

#include <cassert>

InternalAst::InternalAst(Type t)
    : Ast(t)
{
    assert(!isScalar());
}

void InternalAst::change(size_t pos, Ast *next)
{
    if (next != nullptr)
        next->setParent(this);
    doChange(pos, next);
}

void InternalAst::nestAsLeft(size_t pos, InternalAst *nester)
{
    if (nester->size() >= 1) {
        nester->change(0, at(pos).clone());
    } else { // nester is an empty list
        nester->asList().append(at(pos).clone());
    }

    change(pos, nester);
}

void InternalAst::nestAsRight(size_t pos, InternalAst *nester)
{
    if (nester->size() >= 1) {
        nester->change(nester->size() - 1, at(pos).clone());
    } else { // nester is an empty list
        nester->asList().append(at(pos).clone());
    }

    change(pos, nester);
}

void InternalAst::expose(size_t vanisher, size_t exposee)
{
    change(vanisher, at(vanisher).asInternal().at(exposee).clone());
}

Ast::Type InternalAst::typeAt(Ast::Type type, size_t pos)
{
    // mainly for fix-size trees, don't reuse too much
    // for bop list, return LHS and RHS type when op is default
    switch (type) {
    // lists
    case Type::DECL_PARAM_LIST:
        return Type::DECL_PARAM;
    case Type::DOT_BOP_LIST:
        return Type::IDENT;
    case Type::IF_LIST:
        return Type::IF_CONDBODY;

    // fix-size's
    case Type::DECL_CLASS:
        return pos == 0 ? Type::IDENT : Type::MEMBER_LIST;
    case Type::DECL_VAR:
        return Type::IDENT;
    case Type::DECL_METHOD:
        return pos == 0 || pos == 1 ? Type::IDENT
                                    : pos == 2 ? Type::DECL_PARAM_LIST
                                               : Type::STMT_LIST;
    case Type::DECL_PARAM:
        return Type::IDENT;
    case Type::WHILE:
        return pos == 0 ? Type::META : Type::STMT_LIST;
    case Type::ASSIGN:
        return pos == 0 ? Type::IDENT : Type::META;
    case Type::IF_CONDBODY:
        return pos == 0 ? Type::META : Type::STMT_LIST;
    default:
        return Type::META;
    }
}

Ast::Type InternalAst::typeAt(size_t pos) const
{
    return typeAt(getType(), pos);
}

