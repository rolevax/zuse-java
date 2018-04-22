#include "mode_tilex.h"
#include "mode_input_ident.h"
#include "mode_input_string.h"
#include "mode_input_number.h"

#include "../core/doc_editable.h"

#include <cassert>

ModeTilex::ModeTilex(DocEditable &doc, bool macroContext)
    : Mode(doc)
    , mMacroContext(macroContext)
{

}

Mode::Result ModeTilex::keyboard(Key key)
{
    char str[2] = { KeyCode::toChar(key), '\0' };
    Ast::Type ot = mDoc.getOuter().getType();

    if (KeyCode::isAlpha(key)) {
        mDoc.change(Ast::Type::IDENT);
        mDoc.scalarClear();
        mDoc.scalarAppend(str);
        return { ResultType::DONE_POP, new ModeInputIdent(mDoc, false) };
    } else if (KeyCode::isDigit(key)) {
        mDoc.change(Ast::Type::NUMBER);
        mDoc.scalarClear();
        mDoc.scalarAppend(str);
        return { ResultType::DONE_POP, new ModeInputNumber(mDoc, false) };
    } else if (key == Key::DOUBLE_QUOTE) {
        mDoc.change(Ast::Type::STRING);
        return { ResultType::DONE_POP, new ModeInputString(mDoc, true) };
    } else if (key == Key::TAB) { // paste
        mDoc.paste();
        mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
        return DONE_POP_NOPUSH;
    } else if (key == Key::SPACE) {
        return keyboardSpace();
    } else if (key == Key::EQUAL) {
        return keyboardEqual();
    } else if (key == Key::PLUS || key == Key::MINUS) {
        return ppmm(key == Key::PLUS);
    } else { // character coupling
        if (key == Key::AND && ot == Ast::Type::BIT_AND) // & -> &&
            castOuter(Ast::Type::LOGIC_AND_BOP_LIST);
        else if (key == Key::PIPE && ot == Ast::Type::BIT_OR) // | -> ||
            castOuter(Ast::Type::LOGIC_OR_BOP_LIST);
        else if (key == Key::LESS && ot == Ast::Type::LT) // < -> <<
            castOuter(Ast::Type::SHL);
        else if (key == Key::GREATER && ot == Ast::Type::GT) // > -> >>
            castOuter(Ast::Type::SHR);
        else if (key == Key::GREATER && ot == Ast::Type::SHR) // >> -> >>>
            castOuter(Ast::Type::SHRA);
        return DONE_STAY_NOPUSH;
    }
}

Mode::Result ModeTilex::onPushed()
{
    assert(mDoc.getInner().getType() == Ast::Type::META);
    mDoc.setHotLight(DocEditable::HotLightLevel::AREA);
    return DONE_STAY_NOPUSH;
}

const char *ModeTilex::name()
{
    return "Tilex";
}

Mode::Result ModeTilex::keyboardSpace()
{
    const AstInternal &outer = mDoc.getOuter();
    if (outer.getType() == Ast::Type::ADD_BOP_LIST
            && outer.size() == 2
            && mDoc.getInnerIndex() == 0) {
        Ast::Type t = outer.asBopList().opAt(1) == Bop::ADD ?
                    Ast::Type::UNARY_PLUS : Ast::Type::UNARY_MINUS;
        mDoc.sibling(1);
        mDoc.nestAsRight(t);
        mDoc.expose();
    } else if (outer.getType() == Ast::Type::DOT_BOP_LIST
               && outer.asBopList().opAt(mDoc.getInnerIndex()) == Bop::ARR) {
        mDoc.change(Ast::Type::HIDDEN);
    } else {
        mDoc.remove(); // remove the meta node
    }

    mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
    return DONE_POP_NOPUSH;
}

Mode::Result ModeTilex::keyboardEqual()
{
    if (mDoc.getOuter().size() != 2)
        return DONE_STAY_NOPUSH;

    switch (mDoc.getOuter().getType()) {
    case Ast::Type::ASSIGN:
        castOuter(Ast::Type::EQ);
        break;
    case Ast::Type::LT:
        castOuter(Ast::Type::LEQ);
        break;
    case Ast::Type::GT:
        castOuter(Ast::Type::GEQ);
        break;
    case Ast::Type::SHL:
        castOuter(Ast::Type::ASS_SHL);
        break;
    case Ast::Type::SHR:
        castOuter(Ast::Type::ASS_SHR);
        break;
    case Ast::Type::SHRA:
        castOuter(Ast::Type::ASS_SHRA);
        break;
    case Ast::Type::BIT_OR:
        castOuter(Ast::Type::ASS_OR);
        break;
    case Ast::Type::BIT_XOR:
        castOuter(Ast::Type::ASS_XOR);
        break;
    case Ast::Type::BIT_AND:
        castOuter(Ast::Type::ASS_AND);
        break;
    case Ast::Type::ADD_BOP_LIST:
        switch (mDoc.getOuter().asBopList().opAt(1)) {
        case Bop::ADD:
            castOuter(Ast::Type::ASS_ADD);
            break;
        case Bop::SUB:
            castOuter(Ast::Type::ASS_SUB);
            break;
        default:
            throw "WTF";
        }
        break;
    case Ast::Type::MUL_BOP_LIST:
        switch (mDoc.getOuter().asBopList().opAt(1)) {
        case Bop::MUL:
            castOuter(Ast::Type::ASS_MUL);
            break;
        case Bop::DIV:
            castOuter(Ast::Type::ASS_DIV);
            break;
        case Bop::MOD:
            castOuter(Ast::Type::ASS_MOD);
            break;
        default:
            throw "WTF";
        }
        break;
    default:
        break;
    }

    return DONE_STAY_NOPUSH;
}

Mode::Result ModeTilex::ppmm(bool inc)
{
    const AstInternal &outer = mDoc.getOuter();
    if (outer.getType() != Ast::Type::ADD_BOP_LIST || outer.size() != 2)
        return DONE_STAY_NOPUSH;

    bool prefix = mDoc.getInnerIndex() == 0;
    bool plus = outer.asBopList().opAt(1) == Bop::ADD;
    if (plus != inc) // "+-" or "-+"
        return DONE_STAY_NOPUSH;

    Ast::Type t = prefix ? (inc ? Ast::Type::PRE_INC : Ast::Type::PRE_DEC)
                         : (inc ? Ast::Type::POST_INC : Ast::Type::POST_DEC);
    mDoc.sibling(prefix ? +1 : -1);
    mDoc.nestAsLeft(t); // unary nest, both left and right is ok
    mDoc.expose();

    mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
    return DONE_POP_NOPUSH;
}

void ModeTilex::castOuter(Ast::Type ot)
{
    assert(!Ast::isScalar(ot));

    // remember cursor position of the meta and dolly-out
    size_t inner = mDoc.getInnerIndex();
    mDoc.digOut();

    mDoc.cast(ot);

    // set cursor back
    mDoc.fallIn();
    mDoc.sibling(inner);

    if (mMacroContext)
        relayMacro(inner);
}

void ModeTilex::relayMacro(int savedInner)
{
    Ast::Type ot = mDoc.getOuter().getType();
    Ast::Type oot = mDoc.getOuter().getParent().getType();

    if (Ast::isBopList(oot) && oot == ot) {
        mDoc.digOut();
        const AstListBop &flattenee = mDoc.getInner().asBopList();
        int flatteneeSize = flattenee.size();
        for (int i = 0; i < flatteneeSize; i++) {
            Bop infix = i == 0 ? Bop::UNUSED : flattenee.opAt(i);
            mDoc.append(Ast::Type::META, infix);
            mDoc.change(flattenee.at(i).clone());
        }

        mDoc.sibling(-flatteneeSize); // back to the meta-containing tree
        mDoc.remove();
        mDoc.sibling(savedInner); // back to the copied meta
        assert(mDoc.getInner().getType() == Ast::Type::META);
    } else if (mDoc.getOuter().size() == 2
               && Ast::precedence(ot) < Ast::precedence(oot)) {
        // a macro either nest-as-left or append.
        // (the only exception is type-cast, but that's not double-char op)
        // if the outer's size is two, it must be a nest.
        assert(mDoc.getInnerIndex() == 1);
        // revert the node created by the single-char macro
        mDoc.sibling(-1);
        mDoc.expose();

        // auto dolly-out as macros did
        while (Ast::precedence(ot) < mDoc.getOuter().precedence()
               && mDoc.getInnerIndex() == mDoc.getOuter().size() - 1)
            mDoc.digOut();

        // nest-or-append as macros did
        if (mDoc.getOuter().getType() == ot && mDoc.getOuter().isList()) {
            mDoc.append(Ast::Type::META);
        } else {
            mDoc.nestAsLeft(ot);
            mDoc.fallIn();
            mDoc.sibling(+1);
        }
    }
}
