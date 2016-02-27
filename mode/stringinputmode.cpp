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
    assert(doc.getInner().getType() == Ast::Type::STRING);

    if ('"' == key) {
        const std::string &text = doc.getInner().asScalar().getText();
        // count postfix '\' run
        int backSlashCount = 0;
        for (auto it = text.rbegin(); it != text.rend() && '\\' == *it; ++it)
            backSlashCount++;
        if (0 == backSlashCount % 2) { // no '\' waiting for a character
            doc.pop();
            return;
        }
    }

    doc.scalarAppend(key);
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

