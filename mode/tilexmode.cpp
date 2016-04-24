#include "mode/tilexmode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"

#include <cassert>

TilexMode::TilexMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result TilexMode::keyboard(Key key)
{
    char str[2] = { KeyCode::toChar(key), '\0' };
    Ast::Type ot = doc.getOuter().getType();

    if (KeyCode::isAlpha(key)) {
        doc.change(Ast::Type::IDENT);
        doc.scalarClear();
        doc.scalarAppend(str);
        return { ResultType::DONE_POP, new IdentInputMode(doc, false) };
    } else if (KeyCode::isDigit(key)) {
        doc.change(Ast::Type::NUMBER);
        doc.scalarClear();
        doc.scalarAppend(str);
        return { ResultType::DONE_POP, new NumberInputMode(doc, false) };
    } else if (Key::DOUBLE_QUOTE == key) {
        doc.change(Ast::Type::STRING);
        return { ResultType::DONE_POP, new StringInputMode(doc, true) };
    } else if (Key::SPACE == key) {
        doc.remove(); // remove the meta node
        doc.setHotLight(EditableDoc::HotLightLevel::OFF);
        return { ResultType::DONE_POP, nullptr };
    } else if (Key::EQUAL == key) {
        return keyboardEqual();
    } else { // keystroke merging
        if (key == Key::AND && ot == Ast::Type::BIT_AND) // & -> &&
            castOuter(Ast::Type::LOGIC_AND);
        else if (key == Key::PIPE && ot == Ast::Type::BIT_OR) // | -> ||
            castOuter(Ast::Type::LOGIC_OR);
        else if (key == Key::LESS && ot == Ast::Type::LT) // < -> <<
            castOuter(Ast::Type::SHL);
        else if (key == Key::GREATER && ot == Ast::Type::GT) // > -> >>
            castOuter(Ast::Type::SHR);
        else if (key == Key::GREATER && ot == Ast::Type::SHR) // >> -> >>>
            castOuter(Ast::Type::SHRA);
        return DONE_STAY_NOPUSH;
    }
}

Mode::Result TilexMode::onPushed()
{
    assert(doc.getInner().getType() == Ast::Type::META);
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
    return DONE_STAY_NOPUSH;
}

const char *TilexMode::name()
{
    return "Tilex";
}

Mode::Result TilexMode::keyboardEqual()
{
    if (doc.getOuter().size() != 2)
        return DONE_STAY_NOPUSH;

    switch (doc.getOuter().getType()) {
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
        switch (doc.getOuter().asBopList().opAt(1)) {
        case BopListAst::ADD:
            castOuter(Ast::Type::ASS_ADD);
            break;
        case BopListAst::SUB:
            castOuter(Ast::Type::ASS_SUB);
            break;
        default:
            throw "WTF";
        }
        break;
    case Ast::Type::MUL_BOP_LIST:
        switch (doc.getOuter().asBopList().opAt(1)) {
        case BopListAst::MUL:
            castOuter(Ast::Type::ASS_MUL);
            break;
        case BopListAst::DIV:
            castOuter(Ast::Type::ASS_DIV);
            break;
        case BopListAst::MOD:
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

void TilexMode::castOuter(Ast::Type to)
{
    assert(!Ast::isScalar(to));

    size_t inner = doc.getOuter().indexOf(&doc.getInner());
    doc.digOut();
    doc.cast(to);
    doc.fallIn();
    doc.sibling(inner);
}
