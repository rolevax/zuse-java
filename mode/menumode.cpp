#include "core/editabledoc.h"
#include "ast/boplistast.h"
#include "mode/menumode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/tilexmode.h"
#include "mode/fixsizeinputmode.h"

#include <cctype>
#include <cassert>
#include <QDebug>

static bool matchStmt(const Ast *a)
{
    return a->getParent().getType() == Ast::Type::STMT_LIST;
}

MenuMode::MenuMode(EditableDoc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

Mode::Result MenuMode::keyboard(Key key)
{
    if (key == Key::SPACE)
        return DONE_POP_NOPUSH;

    Ast::Type ktype = keyToType(key);
    int bop = keyToBop(key);

    switch (context) {
    case Context::BOP_INSERT:
    case Context::BOP_APPEND: {
        if (context == Context::BOP_INSERT) {
            doc.insert(ktype, bop);
        } else {
            doc.append(ktype, bop);
        }
        break;
    }
    case Context::NEST_AS_LEFT:
        // TODO: smart condition check (nester cannot be scalar)
        if (ktype != Ast::Type::META) {
            doc.nestAsLeft(ktype, bop);
            doc.fallIn();
            doc.sibling(+1);
        }
        break;
    case Context::NEST_AS_RIGHT:
        // TODO: smart condition check (nester cannot be scalar)
        if (ktype != Ast::Type::META) {
            doc.nestAsRight(ktype, bop);
            doc.fallIn();
        }
        break;
    case Context::FALL_SEARCH:
        switch (key) {
        case Key::S:
            doc.focusInBig(matchStmt);
            break;
        // TODO: pattern matching cases keyToPattern
        default:
            doc.focusInBig(ktype);
            break;
        }

        break;
    default:
        break;
    }

    if (context == Context::BOP_INSERT
            || context == Context::BOP_APPEND
            || context == Context::NEST_AS_LEFT
            || context == Context::NEST_AS_RIGHT) {
        return { ResultType::DONE_POP, doc.createModifyMode(true) };
    } else { // big motions
        return DONE_POP_NOPUSH;
    }
}

Mode::Result MenuMode::onPushed()
{
    doc.toggleTension(true);
    return DONE_STAY_NOPUSH;
}

void MenuMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MenuMode::name()
{
    return "oprand?";
}

Ast::Type MenuMode::keyToType(Key key)
{
    switch (context) {
    case Context::BOP_INSERT:
    case Context::BOP_APPEND:
        switch (key) {
        case Key::LEFT_PAREN:
            if (doc.getOuter().getType() == Ast::Type::DOT_BOP_LIST
                    && context == Context::BOP_APPEND)
                return Ast::Type::ARG_LIST;
            else
                return Ast::Type::META;
        default:
            return Ast::Type::META;
        }
    case Context::NEST_AS_LEFT: // nester type
    case Context::NEST_AS_RIGHT:
    case Context::FALL_SEARCH:
    case Context::DIG_SEARCH:
        switch (key) {
        case Key::DOT:
        case Key::LEFT_PAREN:
            return Ast::Type::DOT_BOP_LIST;
        case Key::RIGHT_PAREN:
            return Ast::Type::CAST;
        case Key::PLUS:
        case Key::MINUS:
            return Ast::Type::ADD_BOP_LIST;
        case Key::ASTERISK:
        case Key::SLASH:
        case Key::PERCENT:
            return Ast::Type::MUL_BOP_LIST;
        case Key::AND:
            return Ast::Type::BIT_AND;
        case Key::PIPE:
            return Ast::Type::BIT_OR;
        case Key::CARET:
            return Ast::Type::BIT_XOR;
        case Key::EXCLAM:
            return Ast::Type::LOGIC_NOT;
        case Key::TIDE:
            return Ast::Type::BIT_NOT;
        case Key::LESS:
            return Ast::Type::LT;
        case Key::GREATER:
            return Ast::Type::GT;
        case Key::S_C:
            return Ast::Type::DECL_CLASS;
        case Key::I:
            return Ast::Type::IF_LIST;
        case Key::S_M:
            return Ast::Type::DECL_METHOD;
        case Key::V:
            return Ast::Type::DECL_VAR;
        case Key::EQUAL:
            return Ast::Type::ASSIGN;
        default:
            return Ast::Type::META;
        }
    default: // useless, for supress 'return' warning
        return Ast::Type::IDENT;
    }
}

int MenuMode::keyToBop(Key key)
{
    switch (key) {
    case Key::DOT:
        return BopListAst::DOT;
    case Key::LEFT_PAREN:
        return BopListAst::CALL;
    case Key::PLUS:
        return BopListAst::ADD;
    case Key::MINUS:
        return BopListAst::SUB;
    case Key::ASTERISK:
        return BopListAst::MUL;
    case Key::SLASH:
        return BopListAst::DIV;
    case Key::PERCENT:
        return BopListAst::MOD;
    default:
        return BopListAst::UNUSED;
    }
}



