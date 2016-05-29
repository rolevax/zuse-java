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

Ast::Type InternalAst::typeAt(Ast::Type type, size_t pos, bool allowHidden)
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
    case Type::TRY_LIST:
        return pos == 0 ? Type::STMT_LIST : Type::CATCH;

    // fix-size's
    case Type::DECL_CLASS:
    case Type::DECL_INTERFACE:
        switch (pos) {
        case 1: // extends
            return allowHidden ? Type::HIDDEN : Type::NAME_LIST;
        case 2: // implements
            return allowHidden ? Type::HIDDEN : Type::NAME_LIST;
        case 3: // member
            return Type::MEMBER_LIST;
        default: // ident
            return Type::META;
        }
    case Type::DECL_VAR:
        return Type::IDENT;
    case Type::DECL_METHOD:
        switch (pos) {
        case 2:
            return Type::DECL_PARAM_LIST;
        case 3:
            return allowHidden ? Type::HIDDEN : Type::NAME_LIST;
        case 4:
            return Type::STMT_LIST;
        default:
            return Type::META;
        }
    case Type::DECL_PARAM:
        return Type::IDENT;
    case Type::WHILE:
        return pos == 0 ? Type::META : Type::STMT_LIST;
    case Type::ASSIGN:
        return pos == 0 ? Type::IDENT : Type::META;
    case Type::IF_CONDBODY:
        return pos == 0 ? Type::META : Type::STMT_LIST;
    case Type::CATCH:
        return pos == 0 ? Type::DECL_VAR : Type::STMT_LIST;
    case Type::FOR:
        return pos == 3 ? Type::STMT_LIST : Type::META;
    case Type::RETURN:
    case Type::BREAK:
    case Type::CONTINUE:
        return allowHidden ? Type::HIDDEN : Type::META;
    default:
        return Type::META;
    }
}

Ast::Type InternalAst::typeAt(size_t pos, bool allowHidden) const
{
    return typeAt(getType(), pos, allowHidden);
}

