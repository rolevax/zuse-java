#include "core/editabledoc.h"
#include "mode/stringinputmode.h"
#include "ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

void StringInputMode::keyboard(char key)
{
    switch (key) {
    case '\t':
    case '\r':
        doc.pop();
        break;
    default:
        assert(doc.getInner().isScalar());
        doc.scalarAppend(key);
        break;
    }
}

void StringInputMode::onPushed()
{
    if (clear) {
        assert(doc.getInner().isScalar());
        doc.scalarClear();
    }

    doc.setHotLight(EditableDoc::HotLightLevel::POINT);
}

void StringInputMode::onPopped()
{
    doc.setHotLight(EditableDoc::HotLightLevel::OFF);
}

const char *StringInputMode::name()
{
    return "String Input";
}

