#include "core/editabledoc.h"
#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/scalarinputmode.h"
#include "mode/identinputmode.h"
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
    case 'f': // raw fall-in
        doc.fallIn();
        break;
    case 'F':
        doc.push(new MenuMode(doc, MenuMode::Context::FLY_IN));
        break;
    case 'd': // raw dig out
        doc.digOut();
        break;
    case 'D':
        doc.push(new MenuMode(doc, MenuMode::Context::DOG_OUT));
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
    case 'o': // oh, append - shortcut
        menulessListOp(true);
        break;
    case 'i': // insert - shortcut
        menulessListOp(false);
        break;
    case 'O': // oh, append - menu
        if (doc.getOuter().isList())
            doc.push(new MenuMode(doc, MenuMode::Context::APPEND));
        break;
    case 'I': // insert - menu
        if (doc.getOuter().isList())
            doc.push(new MenuMode(doc, MenuMode::Context::INSERT));
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
        doc.push(new MenuMode(doc, MenuMode::Context::NEST));
        break;
    case 'm': // modify
    case 'M': {
        bool clear = key == 'M';
        switch (doc.getInner().getType()) {
        case Ast::Type::IDENT:
            doc.push(new IdentInputMode(doc, clear));
        default:
            break;
        }
        break;
    }
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

void ViewMode::menulessListOp(bool append)
{
    if (!doc.getOuter().isList())
        return;

    Ast::Type tar;
    switch (doc.getOuter().getType()) {
    case Ast::Type::CLASS_LIST:
        tar = Ast::Type::CLASS;
        break;
    case Ast::Type::STMT_LIST:
        tar = Ast::Type::IDENT;
        break;
    case Ast::Type::DECL_PARAM_LIST:
        tar = Ast::Type::DECL_PARAM;
        break;
    default:
        return; // silently do nothing
    }

    if (append)
        doc.append(tar);
    else
        doc.insert(tar);

    // after insertion jobs
    switch (doc.getOuter().getType()) {
    case Ast::Type::STMT_LIST:
        doc.push(new ScalarInputMode(doc));
        break;
    default:
        break;
    }
}


