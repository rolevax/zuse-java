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
    case Key::G: // next node
        doc.sibling(+1, true);
        break;
    case Key::S: // previous node
        doc.sibling(-1, true);
        break;
    case Key::F: // fall-in, assart, or punch
        if (!doc.getInner().isScalar()) {
            if (doc.getInner().asInternal().size() > 0) {
                doc.fallIn();
                if (doc.getInner().getType() == Ast::Type::HIDDEN) {
                    // punch
                    doc.change(Ast::Type::META);
                    nextPush = doc.createModifyMode(true);
                }
            } else { // assart
                nextPush = menulessListOp(ListOp::ASSART);
            }
        }
        break;
    case Key::D: // dig-out
        doc.digOut();
        break;
    case Key::S_S:
        nextPush = new MenuMode(doc, MenuMode::Context::S_BIG);
        break;
    case Key::S_G:
        nextPush = new MenuMode(doc, MenuMode::Context::G_BIG);
        break;
    case Key::S_F: // fall-search
        if (!doc.getInner().isScalar()
                && doc.getInner().asInternal().size() > 0)
            nextPush = new MenuMode(doc, MenuMode::Context::FOCUS_IN_BIG);
        break;
    case Key::S_D: // dig-search
        nextPush = new MenuMode(doc, MenuMode::Context::DOLLY_OUT_BIG);
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
        } else if (doc.getInnerIndex() > 0
                   && doc.getOuter().at(doc.getInnerIndex() - 1).getType()
                   == Ast::Type::HIDDEN) {
            doc.sibling(-1, false);
            doc.change(doc.getOuter().typeAt(doc.getInnerIndex(), false));
            nextPush = doc.createModifyMode(true);
        }
        break;
    case Key::O: // oh, append
        if (doc.getOuter().isList()) {
            if (doc.getOuter().isBopList() && doc.getOuter().asBopList().numOp() > 1)
                nextPush = new MenuMode(doc, MenuMode::Context::BOP_APPEND);
            else
                nextPush = menulessListOp(ListOp::APPEND);
        } else if (doc.getInnerIndex() + 1 < doc.getOuter().size()
                   && doc.getOuter().at(doc.getInnerIndex() + 1).getType()
                   == Ast::Type::HIDDEN) {
            doc.sibling(+1, false);
            doc.change(doc.getOuter().typeAt(doc.getInnerIndex(), false));
            nextPush = doc.createModifyMode(true);
        }
        break;
    case Key::R: // remove
    case Key::S_R: // remove-clip
        if (key == Key::S_R)
            doc.yank(doc.getInner());
        doc.remove();
        if (doc.getInner().getType() == Ast::Type::HIDDEN) {
            if (doc.getOuter().size() == 1)
                doc.digOut();
            else
                doc.sibling(-1, true);
        }
        break;
    case Key::Y: // yank
        doc.yank(doc.getInner());
        break;
    case Key::P: // switch clipslot
        nextPush = new MenuMode(doc, MenuMode::Context::SWITCH_CLIP);
        break;
    case Key::C: // change
    case Key::S_C: // change-clip
        if (key == Key::S_C)
            doc.yank(doc.getInner());
        doc.change(doc.getOuter().typeAt(doc.getInnerIndex()));
        nextPush = doc.createModifyMode(true);
        break;
    case Key::N: // nest as left
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_LEFT);
        break;
    case Key::S_N: // nest as right
        nextPush = new MenuMode(doc, MenuMode::Context::NEST_AS_RIGHT);
        break;
    case Key::X: // expose
    case Key::S_X: // expose-clip
        if (key == Key::S_X) {
            if (doc.getOuter().size() == 2) {
                // if inner is 1, clip 0; vice versa
                size_t toClip = 1 - doc.getInnerIndex();
                doc.yank(doc.getOuter().at(toClip));
            }
        }
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
    switch (op) {
    case ListOp::INSERT:
        tar = l.typeAt(doc.getInnerIndex());
        break;
    case ListOp::APPEND:
        tar = l.typeAt(doc.getInnerIndex() + 1);
        break;
    case ListOp::ASSART:
        tar = l.typeAt(0);
        break;
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



