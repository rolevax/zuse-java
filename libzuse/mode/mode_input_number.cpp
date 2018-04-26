#include "mode_input_number.h"

#include "../core/doc_editable.h"
#include "../ast/ast_scalar.h"

#include <cassert>



namespace zuse
{



ModeInputNumber::ModeInputNumber(DocEditable &doc, bool clear) :
    Mode(doc),
    mClear(clear)
{

}

Mode::Result ModeInputNumber::keyboard(Key key)
{
    assert(mDoc.getInner().getType() == Ast::Type::NUMBER);

    if (Key::SPACE == key)
        return Result::donePopNoPush();

    char input;
    if (KeyCode::isDigit(key) || key == Key::BACKSPACE)
        input = KeyCode::toChar(key);
    else if (Key::U == key)
        input = '4';
    else if (Key::I == key)
        input = '5';
    else if (Key::O == key)
        input = '6';
    else if (Key::J == key)
        input = '1';
    else if (Key::K == key)
        input = '2';
    else if (Key::L == key)
        input = '3';
    else if (Key::M == key)
        input = '0';
    else
        return Result::raisePopNoPush();

    mDoc.scalarAppend(input);
    return Result::doneStayNoPush();
}

Mode::Result ModeInputNumber::onPushed()
{
    if (mClear) {
        assert(mDoc.getInner().getType() == Ast::Type::NUMBER);
        mDoc.scalarClear();
    }

    mDoc.setHotLight(DocEditable::HotLightLevel::POINT);
    return Result::doneStayNoPush();
}

void ModeInputNumber::onPopped()
{
    mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
}

const char *ModeInputNumber::name()
{
    return "Number Input";
}



} // namespace zuse
