#include "mode_menu.h"
#include "mode_input_ident.h"
#include "mode_input_string.h"
#include "mode_input_number.h"
#include "mode_tilex.h"
#include "mode_input_fix_size.h"

#include "../core/doc_editable.h"
#include "../ast/ast_list_bop.h"

#include <cctype>
#include <cassert>
#include <QDebug>

static bool matchStmt(const Ast *a)
{
    return a->getParent().getType() == Ast::Type::STMT_LIST;
}

ModeMenu::ModeMenu(DocEditable &doc, Context context) :
    Mode(doc),
    mContext(context)
{

}

Mode::Result ModeMenu::keyboard(Key key)
{
    if (key == Key::SPACE)
        return DONE_POP_NOPUSH;

    Ast::Type ktype = keyToType(key);
    Bop bop = keyToBop(key);

    switch (mContext) {
    case Context::BOP_INSERT:
    case Context::BOP_APPEND: {
        if (mContext == Context::BOP_INSERT) {
            mDoc.insert(ktype, bop);
        } else {
            mDoc.append(ktype, bop);
        }
        break;
    }
    case Context::NEST_AS_LEFT:
        // TODO: smart condition check (nester cannot be scalar)
        if (ktype != Ast::Type::META) {
            mDoc.nestAsLeft(ktype, bop);
            mDoc.fallIn();
            mDoc.sibling(+1);
        }
        break;
    case Context::NEST_AS_RIGHT:
        // TODO: smart condition check (nester cannot be scalar)
        if (ktype != Ast::Type::META) {
            mDoc.nestAsRight(ktype, bop);
            mDoc.fallIn();
        }
        break;
    case Context::FOCUS_IN_BIG:
        switch (key) {
        case Key::S:
            mDoc.focusInBig(matchStmt);
            break;
        default:
            if (ktype != Ast::Type::META)
                mDoc.focusInBig(ktype);
            break;
        }
        break;
    case Context::DOLLY_OUT_BIG:
        switch (key) {
        case Key::S:
            mDoc.dollyOutBig(matchStmt);
            break;
        default:
            if (ktype != Ast::Type::META)
                mDoc.dollyOutBig(ktype);
            break;
        }
        break;
    case Context::S_BIG:
        switch (key) {
        case Key::S:
            mDoc.siblingBig(matchStmt, false);
            break;
        default:
            if (ktype != Ast::Type::META)
                mDoc.siblingBig(ktype, false);
            break;
        }
        break;
    case Context::G_BIG:
        switch (key) {
        case Key::S:
            mDoc.siblingBig(matchStmt, true);
            break;
        default:
            if (ktype != Ast::Type::META)
                mDoc.siblingBig(ktype, true);
            break;
        }
        break;
    case Context::SWITCH_CLIP:
        do {
            char c = KeyCode::toChar(key);
            if ('a' <= c && c <= 'z')
                mDoc.switchClip(c);
        } while (false);
        break;
    }

    if (mContext == Context::BOP_INSERT
            || mContext == Context::BOP_APPEND
            || mContext == Context::NEST_AS_LEFT
            || mContext == Context::NEST_AS_RIGHT) {
        return { ResultType::DONE_POP, mDoc.createModifyMode(true) };
    } else { // big motions
        return DONE_POP_NOPUSH;
    }
}

Mode::Result ModeMenu::onPushed()
{
    if (mContext != Context::SWITCH_CLIP)
        mDoc.toggleTension(true);
    return DONE_STAY_NOPUSH;
}

void ModeMenu::onPopped()
{
    if (mContext != Context::SWITCH_CLIP)
        mDoc.toggleTension(false);
}

const char *ModeMenu::name()
{
    return "oprand?";
}

Ast::Type ModeMenu::keyToType(Key key)
{
    switch (mContext) {
    case Context::BOP_INSERT:
    case Context::BOP_APPEND:
        switch (key) {
        case Key::LEFT_PAREN:
            if (mDoc.getOuter().getType() == Ast::Type::DOT_BOP_LIST
                    && mContext == Context::BOP_APPEND)
                return Ast::Type::ARG_LIST;
            else
                return Ast::Type::META;
        default:
            return Ast::Type::META;
        }
    case Context::NEST_AS_LEFT: // nester type
    case Context::NEST_AS_RIGHT:
    case Context::FOCUS_IN_BIG:
    case Context::DOLLY_OUT_BIG:
    case Context::S_BIG:
    case Context::G_BIG:
        switch (key) {
        case Key::DOT:
        case Key::LEFT_PAREN:
        case Key::LEFT_SQUARE:
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
            return Ast::Type::IF_CONDBODY;
        case Key::S_M:
            return Ast::Type::DECL_METHOD;
        case Key::V:
            return Ast::Type::DECL_VAR;
        case Key::EQUAL:
            return Ast::Type::ASSIGN;
        default:
            return Ast::Type::META;
        }
    case Context::SWITCH_CLIP:
        return Ast::Type::META;
    default:
        throw "unreached";
    }
}

Bop ModeMenu::keyToBop(Key key)
{
    switch (key) {
    case Key::DOT:
        return Bop::DOT;
    case Key::LEFT_PAREN:
        return Bop::CALL;
    case Key::LEFT_SQUARE:
        return Bop::ARR;
    case Key::PLUS:
        return Bop::ADD;
    case Key::MINUS:
        return Bop::SUB;
    case Key::ASTERISK:
        return Bop::MUL;
    case Key::SLASH:
        return Bop::DIV;
    case Key::PERCENT:
        return Bop::MOD;
    default:
        return Bop::UNUSED;
    }
}



