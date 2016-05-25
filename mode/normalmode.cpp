#include "mode/normalmode.h"
#include "mode/menumode.h"
#include "mode/tilexmode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "mode/mogglemode.h"
#include "core/editabledoc.h"
#include "ast/listast.h"

NormalMode::NormalMode(EditableDoc &doc)
    : Mode(doc)
    , macro(doc)
{

}

Mode::Result NormalMode::keyboard(Key key)
{
    if (doc.getOuter().getType() == Ast::Type::CLASS_LIST
            && doc.getOuter().size() == 0
            && key != Key::I && key != Key::S_I)
        return DONE_STAY_NOPUSH;

    Mode *nextPush = nullptr;

    if (macro.macro(key, nextPush))
        return { ResultType::DONE_STAY, nextPush };

    switch (key) {
    // logical cursor moving
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

    // HIJK cursor moving
    case Key::H:
        doc.hackLead(false);
        break;
    case Key::L:
        doc.hackLead(true);
        break;
    case Key::K:
        doc.jackKick(false);
        break;
    case Key::J:
        doc.jackKick(true);
        break;

    // modification
    case Key::I: // insert
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList() && doc.getOuter().asBopList().numOp() > 1)
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_INSERT);
            else
                nextPush = menulessListOp(ListOp::INSERT);
        }
        break;
    case Key::O: // oh, append
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList() && doc.getOuter().asBopList().numOp() > 1)
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
    case Key::C: // change
        // TODO
        break;
    case Key::N: // nest as left
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_LEFT);
        break;
    case Key::S_N: // nest as right
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_RIGHT);
        break;
    case Key::X: // expose
        doc.expose();
        break;
    case Key::M: // modify
    case Key::S_M: {
        bool clear = key == Key::S_M;
        if (doc.getInner().isScalar())
            nextPush = doc.createModifyMode(clear);
        else if (doc.getOuter().getType() == Ast::Type::MEMBER_LIST)
            nextPush = new MoggleMode(doc);
        else if (doc.getInner().getType() == Ast::Type::DECL_PARAM
                 || doc.getInner().getType() == Ast::Type::DECL_VAR) // local var
            doc.toggleFinal();
        break;
    }
    default:
        break;
    }

    return { ResultType::DONE_STAY, nextPush };
}

const char *NormalMode::name()
{
    return "View";
}

Mode *NormalMode::menulessListOp(ListOp op)
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
    case Ast::Type::IF_LIST:
        tar = Ast::Type::IF_CONDBODY;
        break;
    case Ast::Type::TRY_LIST:
        tar = Ast::Type::CATCH;
        break;
    case Ast::Type::STMT_LIST:
    case Ast::Type::MEMBER_LIST:
    case Ast::Type::ARG_LIST:
    case Ast::Type::LOGIC_AND_BOP_LIST:
    case Ast::Type::LOGIC_OR_BOP_LIST:
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



