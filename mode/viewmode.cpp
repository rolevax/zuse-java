#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/tipamode.h"
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
    if (doc.getOuter().getType() == Ast::Type::CLASS_LIST
            && doc.getOuter().size() == 0
            && key != 'i' && key != 'I')
        return;

    switch (key) {
    case '(':
        if (doc.getOuter().getType() == Ast::Type::MEMBER_LIST
                && doc.getInner().getType() == Ast::Type::DECL_VAR)
            doc.cast(Ast::Type::DECL_METHOD);
        break;
    // tree-wise cursor moving
    case 'g': // get next node
        doc.sibling(+1);
        break;
    case 's': // senior previous node
        doc.sibling(-1);
        break;
    case 'f': // fall-in or assart
        if (!doc.getInner().isScalar()) {
            if (doc.getInner().asInternal().size() > 0)
                doc.fallIn();
            else
                menulessListOp(ListOp::ASSART);
        }
        break;
    case 'F': // fall-search
        if (!doc.getInner().isScalar()
                && doc.getInner().asInternal().size() > 0)
            doc.push(new MenuMode(doc, MenuMode::Context::FALL_SEARCH));
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
    case 'i': // insert
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList())
                doc.push(new MenuMode(doc, MenuMode::Context::BOP_INSERT));
            else
                menulessListOp(ListOp::INSERT);
        }
        break;
    case 'o': // oh, append
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList())
                doc.push(new MenuMode(doc, MenuMode::Context::BOP_APPEND));
            else
                menulessListOp(ListOp::APPEND);
        }
        break;
    case 'r': // remove
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
        // TODO
        break;
    case 'n': // nest
        doc.push(new MenuMode(doc, MenuMode::Context::NEST_AS_LEFT));
        break;
    case 'm': // modify
    case 'M': {
        bool clear = key == 'M';
        switch (doc.getInner().getType()) {
        case Ast::Type::IDENT:
            doc.push(new IdentInputMode(doc, clear));
            break;
        case Ast::Type::STRING:
            doc.push(new StringInputMode(doc, clear));
            break;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
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
        tar = Ast::Type::DECL_CLASS;
        break;
    case Ast::Type::DECL_PARAM_LIST:
        tar = Ast::Type::DECL_PARAM;
        break;
    case Ast::Type::STMT_LIST:
    case Ast::Type::MEMBER_LIST:
    case Ast::Type::ARG_LIST:
        tar = Ast::Type::META;
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
    case Ast::Type::MEMBER_LIST:
    case Ast::Type::ARG_LIST:
        doc.push(new TipaMode(doc));
        break;
    default:
        break;
    }
}


