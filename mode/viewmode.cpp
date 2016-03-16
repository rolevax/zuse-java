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

Mode::Result ViewMode::keyboard(char key, bool top)
{
    if (doc.getOuter().getType() == Ast::Type::CLASS_LIST
            && doc.getOuter().size() == 0
            && key != 'i' && key != 'I')
        return DONE_STAY_NOPUSH;

    Mode *nextPush = nullptr;

    switch (key) {
    case '(':
        if (doc.getOuter().getType() == Ast::Type::MEMBER_LIST
             && doc.getInner().getType() == Ast::Type::DECL_VAR) {
            doc.cast(Ast::Type::DECL_METHOD);
            // TODO push a fix-size mode with offset
        } else if (doc.getOuter().getType() == Ast::Type::DECL_VAR) {
            doc.digOut();
            doc.cast(Ast::Type::DECL_METHOD);
            // TODO push a fix-size mode with offset
        } else if (doc.getOuter().getType() == Ast::Type::DOT_BOP_LIST) {
            doc.append(Ast::Type::IDENT, BopListAst::CALL);
        } else if (doc.getOuter().getType() == Ast::Type::IF_LIST) {
            // TODO
        } else {
            doc.nestAsLeft(Ast::Type::DOT_BOP_LIST, BopListAst::CALL);
            doc.fallIn();
            doc.sibling(+1);
        }
        if (top)
            nextPush = doc.createModifyMode(true);
        break;
    case '.':
        if (doc.getOuter().getType() == Ast::Type::DOT_BOP_LIST) {
            doc.append(Ast::Type::IDENT, BopListAst::DOT);
        } else {
            doc.nestAsLeft(Ast::Type::DOT_BOP_LIST, BopListAst::DOT);
            doc.fallIn();
            doc.sibling(+1);
        }
        if (top)
            nextPush = doc.createModifyMode(true);
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
                nextPush = menulessListOp(ListOp::ASSART);
        }
        break;
    case 'F': // fall-search
        if (!doc.getInner().isScalar()
                && doc.getInner().asInternal().size() > 0)
            nextPush = new MenuMode(doc, MenuMode::Context::FALL_SEARCH);
        break;
    case 'd': // dig-out
        doc.digOut();
        break;
    case 'D': // dig-search
        nextPush = new MenuMode(doc, MenuMode::Context::DIG_SEARCH);
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
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_INSERT);
            else
                nextPush = menulessListOp(ListOp::INSERT);
        }
        break;
    case 'o': // oh, append
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList())
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_APPEND);
            else
                nextPush = menulessListOp(ListOp::APPEND);
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
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_LEFT);
        break;
    case 'm': // modify
    case 'M': {
        bool clear = key == 'M';
        nextPush = doc.createModifyMode(clear);
        break;
    }
    default:
        break;
    }

    return { ResultType::DONE_STAY, nextPush };
}

const char *ViewMode::name()
{
    return "View";
}

Mode *ViewMode::menulessListOp(ListOp op)
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
        return nullptr; // silently do nothing
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
        return new TipaMode(doc);
    default:
        return nullptr;
    }
}


