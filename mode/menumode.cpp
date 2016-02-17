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

    if (context == Context::FLY_IN) {
        switch (key) {
        // TODO: non-(node-type) cases
        default:
            Ast::Type t = keyToType(key);
            // TODO: do-nothing if no map
            std::function<bool(const Ast *)> match = [t](const Ast *a)
            {
                return a->getType() == t;
            };
            doc.flyIn(match);
        }

        doc.pop();
        return;
    }

    // TODO: data-ize the fucking design
    Ast::Type type = context == Context::ASSART ? doc.getInner().getType()
                                                : doc.getOuter().getType();
    switch(type) {
    case Ast::Type::MEMBER_LIST:
        switch (key) {
        case 'f':
            work(Ast::Type::DECL_STMT);
            break;
        case 'm':
            work(Ast::Type::METHOD);
            break;
        default:
            break;
        }
        break;
    case Ast::Type::STMT_LIST:
        if (std::isalpha(key)) {
            char tmp[2] = { key, '\0' };
            work(Ast::Type::IDENT, tmp);
        }
        break;
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
    if (context == Context::INSERT || context == Context::APPEND
            || context == Context::ASSART || context == Context::CHANGE
            || context == Context::NEST) {
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
    }

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

Ast::Type MenuMode::keyToType(char key)
{
    switch (key) {
    case '.':
        return Ast::Type::IDENT;
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

    if (nullptr != keytal)
        doc.scalarAppend(keytal);

    switch (type) {
    /*
    case Ast::Type::STRING:
        doc.pop(new StringInputMode(doc, false));
        break;
    case Ast::Type::PAIR:
        doc.pop(new PairInputMode(doc));
        break;
        */
    case Ast::Type::IDENT:
        doc.pop(new IdentInputMode(doc, false));
        break;
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


