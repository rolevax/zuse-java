#include "core/editabledoc.h"
#include "mode/numberinputmode.h"
#include "ast/scalarast.h"

#include <cassert>

NumberInputMode::NumberInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

Mode::Result NumberInputMode::keyboard(Key key)
{
    assert(doc.getInner().getType() == Ast::Type::NUMBER);

    if (Key::SPACE == key)
        return DONE_POP_NOPUSH;

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
        return RAISE_POP_NOPUSH;

    doc.scalarAppend(input);
    return DONE_STAY_NOPUSH;
}

Mode::Result NumberInputMode::onPushed()
{
    if (clear) {
        assert(doc.getInner().getType() == Ast::Type::NUMBER);
        doc.scalarClear();
    }

    doc.setHotLight(EditableDoc::HotLightLevel::POINT);
    return DONE_STAY_NOPUSH;
}

void NumberInputMode::onPopped()
{
    doc.setHotLight(EditableDoc::HotLightLevel::OFF);
}

const char *NumberInputMode::name()
{
    return "Number Input";
}

