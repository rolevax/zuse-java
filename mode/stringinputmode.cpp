#include "core/editabledoc.h"
#include "mode/stringinputmode.h"
#include "ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    clear(clear)
{

}

Mode::Result StringInputMode::keyboard(char key, bool top)
{
    assert(doc.getInner().getType() == Ast::Type::STRING);

    if ('"' == key) {
        const std::string &text = doc.getInner().asScalar().getText();
        // count postfix '\' run
        int backSlashCount = 0;
        for (auto it = text.rbegin(); it != text.rend() && '\\' == *it; ++it)
            backSlashCount++;
        if (0 == backSlashCount % 2) { // no '\' waiting for a character
            return { true, true, nullptr };
        }
    }

    doc.scalarAppend(key);
    return { false, true, nullptr };
}

Mode::Result StringInputMode::onPushed()
{
    if (clear) {
        assert(doc.getInner().isScalar());
        doc.scalarClear();
    }

    doc.setHotLight(EditableDoc::HotLightLevel::POINT);
    return { false, true, nullptr };
}

void StringInputMode::onPopped()
{
    doc.setHotLight(EditableDoc::HotLightLevel::OFF);
}

const char *StringInputMode::name()
{
    return "String Input";
}

