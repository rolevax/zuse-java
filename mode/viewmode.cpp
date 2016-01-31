#include "core/editabledoc.h"
#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"

ViewMode::ViewMode(EditableDoc &doc) :
    Mode(doc)
{

}

void ViewMode::keyboard(char key)
{
    switch (key) {
    // abstract cursor moving
    case 'g': // get next node
        doc.sibling(+1);
        break;
    case 's': // senior previous node
        doc.sibling(-1);
        break;
    case 'f': // fall in
        doc.fallIn();
        break;
    case 'd': // dig out
        doc.digOut();
        break;

    // concrete cursor moving
    case 'h': // hack left
        doc.hackLead(false);
        break;
    case 'l': // lead right
        doc.hackLead(true);
        break;
    case 'k': // kick up
        doc.jackKick(false);
        break;
    case 'j': // jack down
        doc.jackKick(true);
        break;

    // outer modification
    case 'o': // oh, append
    case 'i': // insert
        if (doc.getOuter().isList()) {
            MenuMode::Context context;
            context = 'o' == key ? MenuMode::Context::APPEND
                                 : MenuMode::Context::INSERT;
            doc.push(new MenuMode(doc, context));
        }
        break;
    case 'r': // remove
        if (doc.getOuter().isList())
            doc.remove();
        // TODO get to clipboard
        break;
    case 'y': // yank
        // TODO
        break;
    case 'p': // paste
        break;

    // inner modification
    case 'c': // change
        if (doc.getOuter().isChangeable()) {
            doc.push(new MenuMode(doc, MenuMode::Context::CHANGE));
        }
        break;
    case 'n': // nest
        if (doc.getOuter().isChangeable()) {
            doc.push(new MenuMode(doc, MenuMode::Context::NEST));
        }
        break;
    case 'm': // modify
    case 'M':
        switch (doc.getInner().getType()) {
        case Ast::Type::STRING:
        case Ast::Type::KEY:
            doc.push(new StringInputMode(doc, key == 'M'));
            break;
        case Ast::Type::NUMBER:
            doc.push(new NumberInputMode(doc, key == 'M'));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void ViewMode::emptyKeyboard(char key)
{
    // insert to empty document
    if (key == 'i')
        doc.push(new MenuMode(doc, MenuMode::Context::INSERT));
}

const char *ViewMode::name()
{
    return "View";
}


