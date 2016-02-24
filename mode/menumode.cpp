#include "core/editabledoc.h"
#include "mode/menumode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/pairinputmode.h"

#include <cctype>
#include <cassert>
#include <QDebug>

MenuMode::MenuMode(EditableDoc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

void MenuMode::keyboard(char key)
{
    if (key == ' ') {
        doc.pop();
        return;
    }

    Ast::Type keyType = keyToType(key);
    Mode *nextMode = nullptr;

    switch (context) {
    case Context::INSERT: {
        doc.insert(keyType);
        nextMode = modeFor(keyType);
        break;
    }
    case Context::APPEND: {
        doc.append(keyType);
        nextMode = modeFor(keyType);
        break;
    }
    case Context::ASSART: {
        doc.assart(keyType);
        nextMode = modeFor(keyType);
        break;
    }
    case Context::FALL_SEARCH:
        switch (key) {
        // TODO: pattern matching cases keyToPattern
        default:
            // TODO: do-nothing if no map
            std::function<bool(const Ast *)> match = [keyType](const Ast *a)
            {
                return a->getType() == keyType;
            };
            doc.flyIn(match);
        }

        break;
    case Context::NEST:
        switch (key) {
        // TODO: smart condition check (nester cannot be scalar)
        case '=':
            doc.nestAsLeft(Ast::Type::ASSIGN);
            break;
        case '+':
            doc.nestAsLeft(Ast::Type::ADDSUB_LIST);
            break;
        case '-':
            doc.nestAsLeft(Ast::Type::ADDSUB_LIST);
            //TODO set rasing
            break;
        case '*':
            doc.nestAsLeft(Ast::Type::MULDIV_LIST);
            break;
        case '/':
            doc.nestAsLeft(Ast::Type::MULDIV_LIST);
            break;
        case '(':
            doc.nestAsLeft(Ast::Type::CALL);
            break;
        }

        break;
    default:
        break;
    }

    doc.pop(nextMode);
}

void MenuMode::emptyKeyboard(char key)
{
    keyboard(key);
}

void MenuMode::onPushed()
{
    doc.toggleTension(true);
}

void MenuMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MenuMode::name()
{
    return "op pending";
}

Mode *MenuMode::modeFor(Ast::Type t)
{
    switch (t) {
    case Ast::Type::IDENT:
        return new IdentInputMode(doc, false);
    default:
        return nullptr;
    }
}

Ast::Type MenuMode::keyToType(char key)
{
    switch (key) {
    case '.':
        return Ast::Type::IDENT;
    case 'C':
        return Ast::Type::CLASS;
    case 'i':
        return Ast::Type::IF_LIST;
    case 'm':
        return Ast::Type::METHOD;
    case 'v':
        return Ast::Type::DECL_STMT;
    default: // TODO: a 'nothing' type?
        return Ast::Type::IDENT;
    }
}



