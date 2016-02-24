#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/scalarinputmode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"
#include "ast/listast.h"

ViewMode::ViewMode(EditableDoc &doc) :
    Mode(doc)
{

}

void ViewMode::keyboard(char key)
{
    switch (key) {
    // tree-wise cursor moving
    case 'g': // get next node
        doc.sibling(+1);
        break;
    case 's': // senior previous node
        doc.sibling(-1);
        break;
    case 'f': // fall-in or quick-assart
        if (!doc.getInner().isScalar()) {
            if (doc.getInner().asInternal().size() > 0)
                doc.fallIn();
            else
                menulessListOp(ListOp::ASSART);
        }
        break;
    case 'F': // fall-search or quick-assart
        if (!doc.getInner().isScalar()) {
            if (doc.getInner().asInternal().size() > 0)
                doc.push(new MenuMode(doc, MenuMode::Context::FALL_SEARCH));
            else
                doc.push(new MenuMode(doc, MenuMode::Context::ASSART));
        }
        break;
    case 'd': // dig-out
        doc.digOut();
        break;
    case 'D': // dig-search
        doc.push(new MenuMode(doc, MenuMode::Context::DIG_SEARCH));
        break;

    // token-wise cursor moving
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
    case 'o': // oh, quick-append
        if (doc.getOuter().isList())
            menulessListOp(ListOp::APPEND);
        break;
    case 'i': // quick-insert
        if (doc.getOuter().isList())
            menulessListOp(ListOp::INSERT);
        break;
    case 'O': // oh, append
        if (doc.getOuter().isList())
            doc.push(new MenuMode(doc, MenuMode::Context::APPEND));
        break;
    case 'I': // insert
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

void ViewMode::menulessListOp(ListOp op)
{
    const ListAst &l = (op == ListOp::ASSART ? doc.getInner()
                                             : doc.getOuter()).asList();
    Ast::Type tar;

    switch (l.getType()) {
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

    switch (op) {
    case ListOp::INSERT:
        doc.insert(tar);
        break;
    case ListOp::APPEND:
        doc.append(tar);
        break;
    case ListOp::ASSART:
        doc.assart(tar);
        break;
    }

    // after insertion jobs
    switch (doc.getOuter().getType()) {
    case Ast::Type::STMT_LIST:
        doc.push(new ScalarInputMode(doc));
        break;
    default:
        break;
    }
}


