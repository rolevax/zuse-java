#include "core/editabledoc.h"
#include "mode/menumode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/pairinputmode.h"

#include <cassert>
#include <QDebug>

MenuMode::MenuMode(EditableDoc &doc, Context context) :
    Mode(doc),
    context(context)
{

}

void MenuMode::keyboard(char key)
{
    Ast::Type otype = doc.getOuter().getType();
    switch (key) {
    case ' ':
        doc.pop();
        break;
    case 'f':
        if (otype == Ast::Type::MEMBER_LIST)
            work(Ast::Type::DECL_STMT);
        break;
    case 'm':
        if (otype == Ast::Type::MEMBER_LIST)
            work(Ast::Type::METHOD);
        break;
        /*
    case 'x':
        work(Ast::Type::KEYTAL, "null");
        break;
    case 't':
        work(Ast::Type::KEYTAL, "true");
        break;
    case 'f':
        work(Ast::Type::KEYTAL, "false");
        break;
        */
    default:
        break;
    }
}

void MenuMode::emptyKeyboard(char key)
{
    keyboard(key);
}

void MenuMode::onPushed()
{
    // early leave if only one choice
    bool underTyrant = context != Context::ASSART
            && doc.getOuter().getType() == Ast::Type::CLASS_LIST;
    bool intoTyrant = context == Context::ASSART
            && doc.getInner().getType() == Ast::Type::CLASS_LIST;
    if (underTyrant || intoTyrant) {
        work(Ast::Type::CLASS);
        return;
    } else if (context == Context::NEST) {
        // TODO
        //work(Ast::Type::ARRAY);
        return;
    }

    doc.toggleTension(true);
}

void MenuMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MenuMode::name()
{
    return "Select Type";
}

/**
 * @brief MenuMode::work
 * @param type The type of the new node
 * Insert into or change inside 'outer', with
 * position specified by 'inner'.
 */
void MenuMode::work(Ast::Type type, const char *keytal)
{
    (void) keytal; // TODO
    if (context == Context::CHANGE) {
        doc.change(type);
    } else if (context == Context::NEST) {
        doc.nest(type);
    } else {
        // prepare cursor
        if (context == Context::APPEND) {
            doc.cursorForward();
        } else if (context == Context::ASSART) {
            doc.cursorIn();
        }

        doc.insert(type);
    }

    switch (type) {
    /*
    case Ast::Type::STRING:
        doc.pop(new StringInputMode(doc, false));
        break;
    case Ast::Type::NUMBER:
        doc.pop(new NumberInputMode(doc, false));
        break;
    case Ast::Type::PAIR:
        doc.pop(new PairInputMode(doc));
        break;
    case Ast::Type::KEYTAL:
        assert(nullptr != keytal);
        doc.scalarAppend(keytal);
        doc.pop();
        break;
        */
    // no more input mode to push
    case Ast::Type::CLASS:
    case Ast::Type::DECL_STMT:
    case Ast::Type::METHOD:
        doc.pop();
        break;
    default:
        throw "MenuMode: work(): unhandled ast type";
        break;
    }
}


