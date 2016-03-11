#include "core/editabledoc.h"
#include "ast/boplistast.h"
#include "mode/menumode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/tipamode.h"
#include "mode/pairinputmode.h"

#include <cctype>
#include <cassert>
#include <QDebug>

MenuMode::MenuMode(EditableDoc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

Mode::Result MenuMode::keyboard(char key)
{
    if (key == ' ') {
        return { ResultType::POP, nullptr };
    }

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
    case Context::FALL_SEARCH:
        switch (key) {
        // TODO: pattern matching cases keyToPattern
        default:
            // TODO: do-nothing if no map
            std::function<bool(const Ast *)> match = [ktype](const Ast *a)
            {
                return a->getType() == ktype;
            };
            doc.flyIn(match);
        }

        break;
    default:
        break;
    }

    if (context == Context::BOP_INSERT
            || context == Context::BOP_APPEND
            || context == Context::NEST_AS_LEFT) {
        return { ResultType::POP, doc.createModifyMode(true) };
    } else {
        return { ResultType::STAY, nullptr };
    }
}

Mode::Result MenuMode::onPushed()
{
    doc.toggleTension(true);
    return { ResultType::STAY, nullptr };
}

void MenuMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MenuMode::name()
{
    return "oprand?";
}

Ast::Type MenuMode::keyToType(char key)
{
    switch (context) {
    case Context::BOP_INSERT:
    case Context::BOP_APPEND:
        switch (key) {
        case '(':
            if (doc.getOuter().getType() == Ast::Type::DOT_BOP_LIST
                    && context == Context::BOP_APPEND)
                return Ast::Type::ARG_LIST;
            else
                return Ast::Type::META;
        default:
            return Ast::Type::META;
        }
    case Context::NEST_AS_LEFT: // nester type
    case Context::FALL_SEARCH:
    case Context::DIG_SEARCH:
        switch (key) {
        case '.':
        case '(':
            return Ast::Type::DOT_BOP_LIST;
        case '+':
        case '-':
            return Ast::Type::ADD_BOP_LIST;
        case '*':
        case '/':
        case '%':
            return Ast::Type::MUL_BOP_LIST;
        case 'C':
            return Ast::Type::DECL_CLASS;
        case 'i':
            return Ast::Type::IF_LIST;
        case 'm':
            return Ast::Type::DECL_METHOD;
        case 'v':
            return Ast::Type::DECL_VAR;
        case '=':
            return Ast::Type::ASSIGN;
        default:
            return Ast::Type::META;
        }
    default: // useless, for supress 'return' warning
        return Ast::Type::IDENT;
    }
}

int MenuMode::keyToBop(char key)
{
    switch (key) {
    case '.': return BopListAst::DOT;
    case '(': return BopListAst::CALL;
    case '+': return BopListAst::ADD;
    case '-': return BopListAst::SUB;
    case '*': return BopListAst::MUL;
    case '/': return BopListAst::DIV;
    case '%': return BopListAst::MOD;
    default: return BopListAst::UNUSED;
    }
}



