#include "mode/viewmode.h"
#include "mode/menumode.h"
#include "mode/tilexmode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"
#include "ast/listast.h"

ViewMode::ViewMode(EditableDoc &doc)
    : Mode(doc)
    , macro(doc)
{

}

Mode::Result ViewMode::keyboard(Key key)
{
    if (doc.getOuter().getType() == Ast::Type::CLASS_LIST
            && doc.getOuter().size() == 0
            && key != Key::I && key != Key::S_I)
        return DONE_STAY_NOPUSH;

    Mode *nextPush = nullptr;

    if (macro.macro(key, nextPush))
        return { ResultType::DONE_STAY, nextPush };

    switch (key) {
    // tree-wise cursor moving
    case Key::G: // get next node
        doc.sibling(+1);
        break;
    case Key::S: // senior previous node
        doc.sibling(-1);
        break;
    case Key::F: // fall-in or assart
        if (!doc.getInner().isScalar()) {
            if (doc.getInner().asInternal().size() > 0)
                doc.fallIn();
            else
                nextPush = menulessListOp(ListOp::ASSART);
        }
        break;
    case Key::S_F: // fall-search
        if (!doc.getInner().isScalar()
                && doc.getInner().asInternal().size() > 0)
            nextPush = new MenuMode(doc, MenuMode::Context::FALL_SEARCH);
        break;
    case Key::D: // dig-out
        doc.digOut();
        break;
    case Key::S_D: // dig-search
        nextPush = new MenuMode(doc, MenuMode::Context::DIG_SEARCH);
        break;

    // token-wise cursor moving
    case Key::H: // hack left
        doc.hackLead(false);
        break;
    case Key::L: // lead right
        doc.hackLead(true);
        break;
    case Key::K: // kick up
        doc.jackKick(false);
        break;
    case Key::J: // jack down
        doc.jackKick(true);
        break;

    // outer modification
    case Key::I: // insert
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList())
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_INSERT);
            else
                nextPush = menulessListOp(ListOp::INSERT);
        }
        break;
    case Key::O: // oh, append
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList())
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_APPEND);
            else
                nextPush = menulessListOp(ListOp::APPEND);
        }
        break;
    case Key::R: // remove
        doc.remove();
        // TODO get to clipboard
        break;
    case Key::Y: // yank
        // TODO
        break;
    case Key::P: // paste
        break;

    // inner modification
    case Key::C: // change
        // TODO
        break;
    case Key::N: // nest
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_LEFT);
        break;
    case Key::M: // modify
    case Key::S_M: {
        bool clear = key == Key::S_M;
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
    return doc.createModifyMode(true);
}



