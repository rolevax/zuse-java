#include "ast_internal.h"
#include "ast_list.h"

#include <cassert>

AstInternal::AstInternal(Type t)
    : Ast(t)
{
    assert(!isScalar());
}

void AstInternal::change(size_t pos, std::unique_ptr<Ast> next)
{
    if (next != nullptr)
        next->setParent(this);
    doChange(pos, std::move(next));
}

void AstInternal::nestAsLeft(size_t pos, std::unique_ptr<AstInternal> nester)
{
    if (nester->size() >= 1) {
        nester->change(0, at(pos).clone());
    } else { // nester is an empty list
        nester->asList().append(at(pos).clone());
    }

    change(pos, std::move(nester));
}

void AstInternal::nestAsRight(size_t pos, std::unique_ptr<AstInternal> nester)
{
    if (nester->size() >= 1) {
        nester->change(nester->size() - 1, at(pos).clone());
    } else { // nester is an empty list
        nester->asList().append(at(pos).clone());
    }

    change(pos, std::move(nester));
}

void AstInternal::expose(size_t vanisher, size_t exposee)
{
    change(vanisher, at(vanisher).asInternal().at(exposee).clone());
}

Ast::Type AstInternal::typeAt(Ast::Type type, size_t pos, bool allowHidden)
{
    // mainly for fix-size trees, don't reuse too much
    // for bop list, return LHS and RHS type when op is default
    switch (type) {
    // lists
    case Type::CLASS_LIST:
        return Type::DECL_CLASS;
    case Type::DECL_PARAM_LIST:
        return Type::DECL_PARAM;
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
    case Type::NEW_CLASS:
        switch (pos) {
        case 1: // arg
            return Type::ARG_LIST;
        case 2: // anoy
            return allowHidden ? Type::HIDDEN : Type::MEMBER_LIST;
        default: // type name
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

Ast::Type AstInternal::typeAt(size_t pos, bool allowHidden) const
{
    return typeAt(getType(), pos, allowHidden);
}

