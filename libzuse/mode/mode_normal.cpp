#include "mode_normal.h"
#include "mode_menu.h"
#include "mode_tilex.h"
#include "mode_input_ident.h"
#include "mode_input_string.h"
#include "mode_input_number.h"
#include "mode_moggle.h"

#include "../core/doc_editable.h"
#include "../ast/ast_list.h"

ModeNormal::ModeNormal(DocEditable &doc)
    : Mode(doc)
    , mMacro(doc)
{

}

Mode::Result ModeNormal::keyboard(Key key)
{
    if (mDoc.getOuter().getType() == Ast::Type::CLASS_LIST
            && mDoc.getOuter().size() == 0
            && key != Key::I && key != Key::S_I)
        return DONE_STAY_NOPUSH;

    Mode *nextPush = nullptr;

    if (mMacro.macro(key, nextPush))
        return { ResultType::DONE_STAY, nextPush };

    switch (key) {
    // logical cursor moving
    case Key::G: // next node
        mDoc.sibling(+1, true);
        break;
    case Key::S: // previous node
        mDoc.sibling(-1, true);
        break;
    case Key::F: // fall-in, assart, or punch
        if (!mDoc.getInner().isScalar()) {
            if (mDoc.getInner().asInternal().size() > 0) {
                mDoc.fallIn();
                if (mDoc.getInner().getType() == Ast::Type::HIDDEN) {
                    // punch
                    mDoc.change(Ast::Type::META);
                    nextPush = mDoc.createModifyMode(true);
                }
            } else { // assart
                nextPush = menulessListOp(ListOp::ASSART);
            }
        }
        break;
    case Key::D: // dig-out
        mDoc.digOut();
        break;
    case Key::S_S:
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::S_BIG);
        break;
    case Key::S_G:
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::G_BIG);
        break;
    case Key::S_F: // fall-search
        if (!mDoc.getInner().isScalar()
                && mDoc.getInner().asInternal().size() > 0)
            nextPush = new ModeMenu(mDoc, ModeMenu::Context::FOCUS_IN_BIG);
        break;
    case Key::S_D: // dig-search
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::DOLLY_OUT_BIG);
        break;

    // HIJK cursor moving
    case Key::H:
        mDoc.hackLead(false);
        break;
    case Key::L:
        mDoc.hackLead(true);
        break;
    case Key::K:
        mDoc.jackKick(false);
        break;
    case Key::J:
        mDoc.jackKick(true);
        break;

    // modification
    case Key::I: // insert
        if (mDoc.getOuter().isList()) {
            if (mDoc.getOuter().isBopList() && mDoc.getOuter().asBopList().numOp() > 1)
                nextPush = new ModeMenu(mDoc, ModeMenu::Context::BOP_INSERT);
            else
                nextPush = menulessListOp(ListOp::INSERT);
        } else if (mDoc.getInnerIndex() > 0
                   && mDoc.getOuter().at(mDoc.getInnerIndex() - 1).getType()
                   == Ast::Type::HIDDEN) {
            mDoc.sibling(-1, false);
            mDoc.change(mDoc.getOuter().typeAt(mDoc.getInnerIndex(), false));
            nextPush = mDoc.createModifyMode(true);
        }
        break;
    case Key::O: // oh, append
        if (mDoc.getOuter().isList()) {
            if (mDoc.getOuter().isBopList() && mDoc.getOuter().asBopList().numOp() > 1)
                nextPush = new ModeMenu(mDoc, ModeMenu::Context::BOP_APPEND);
            else
                nextPush = menulessListOp(ListOp::APPEND);
        } else if (mDoc.getInnerIndex() + 1 < mDoc.getOuter().size()
                   && mDoc.getOuter().at(mDoc.getInnerIndex() + 1).getType()
                   == Ast::Type::HIDDEN) {
            mDoc.sibling(+1, false);
            mDoc.change(mDoc.getOuter().typeAt(mDoc.getInnerIndex(), false));
            nextPush = mDoc.createModifyMode(true);
        }
        break;
    case Key::R: // remove
    case Key::S_R: // remove-clip
        if (key == Key::S_R)
            mDoc.yank(mDoc.getInner());
        mDoc.remove();
        if (mDoc.getOuter().getType() != Ast::Type::CLASS_LIST
                && mDoc.getInner().getType() == Ast::Type::HIDDEN) {
            if (mDoc.getOuter().size() == 1)
                mDoc.digOut();
            else
                mDoc.sibling(-1, true);
        }
        break;
    case Key::Y: // yank
        mDoc.yank(mDoc.getInner());
        break;
    case Key::P: // switch clipslot
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::SWITCH_CLIP);
        break;
    case Key::C: // change
    case Key::S_C: // change-clip
        if (key == Key::S_C)
            mDoc.yank(mDoc.getInner());
        mDoc.change(mDoc.getOuter().typeAt(mDoc.getInnerIndex()));
        nextPush = mDoc.createModifyMode(true);
        break;
    case Key::N: // nest as left
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::NEST_AS_LEFT);
        break;
    case Key::S_N: // nest as right
        nextPush = new ModeMenu(mDoc, ModeMenu::Context::NEST_AS_RIGHT);
        break;
    case Key::X: // expose
    case Key::S_X: // expose-clip
        if (key == Key::S_X) {
            if (mDoc.getOuter().size() == 2) {
                // if inner is 1, clip 0; vice versa
                size_t toClip = 1 - mDoc.getInnerIndex();
                mDoc.yank(mDoc.getOuter().at(toClip));
            }
        }
        mDoc.expose();
        break;
    case Key::M: // modify
    case Key::S_M: {
        bool clear = key == Key::S_M;
        if (mDoc.getInner().isScalar())
            nextPush = mDoc.createModifyMode(clear);
        else if (mDoc.getOuter().getType() == Ast::Type::MEMBER_LIST
                 || mDoc.getOuter().getType() == Ast::Type::CLASS_LIST)
            nextPush = new ModeMoggle(mDoc);
        else if (mDoc.getInner().getType() == Ast::Type::DECL_PARAM
                 || mDoc.getInner().getType() == Ast::Type::DECL_VAR) // local var
            mDoc.toggleFinal();
        break;
    }
    default:
        break;
    }

    return { ResultType::DONE_STAY, nextPush };
}

const char *ModeNormal::name()
{
    return "View";
}

Mode *ModeNormal::menulessListOp(ListOp op)
{
    const AstList &l = (op == ListOp::ASSART ? mDoc.getInner()
                                             : mDoc.getOuter()).asList();
    Ast::Type tar;
    switch (op) {
    case ListOp::INSERT:
        tar = l.typeAt(mDoc.getInnerIndex());
        break;
    case ListOp::APPEND:
        tar = l.typeAt(mDoc.getInnerIndex() + 1);
        break;
    case ListOp::ASSART:
        tar = l.typeAt(0);
        break;
    }

    switch (op) {
    case ListOp::INSERT:
        mDoc.insert(tar);
        break;
    case ListOp::APPEND:
        mDoc.append(tar);
        break;
    case ListOp::ASSART:
        mDoc.assart(tar);
        break;
    }

    // after insertion jobs
    return mDoc.createModifyMode(true);
}



