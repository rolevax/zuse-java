#include "core/editabledoc.h"
#include "mode/numberinputmode.h"
#include "ast/scalarast.h"

#include <cassert>

NumberInputMode::NumberInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

Mode::Result NumberInputMode::keyboard(char key, bool top)
{
    assert(doc.getInner().getType() == Ast::Type::NUMBER);

    if (' ' == key)
        return DONE_POP_NOPUSH;

    char input;
    if (('0' <= key && key <= '9') || key == '\b')
        input = key;
    else if ('u' == key)
        input = '4';
    else if ('i' == key)
        input = '5';
    else if ('o' == key)
        input = '6';
    else if ('j' == key)
        input = '1';
    else if ('k' == key)
        input = '2';
    else if ('l' == key)
        input = '3';
    else if ('m' == key)
        input = '0';
    else
        return DONE_STAY_NOPUSH;

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

