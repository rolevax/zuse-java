#include "core/editabledoc.h"
#include "mode/stringinputmode.h"
#include "ast/scalarast.h"

#include <cassert>

StringInputMode::StringInputMode(EditableDoc &doc, bool clear) :
    Mode(doc),
    mClear(clear)
{

}

Mode::Result StringInputMode::keyboard(Key key)
{
    assert(mDoc.getInner().getType() == Ast::Type::STRING);

    if (Key::DOUBLE_QUOTE == key) {
        const std::string &text = mDoc.getInner().asScalar().getText();
        // count postfix '\' run
        int backSlashCount = 0;
        for (auto it = text.rbegin(); it != text.rend() && '\\' == *it; ++it)
            backSlashCount++;
        if (0 == backSlashCount % 2) { // no '\' waiting for a character
            return DONE_POP_NOPUSH;
        }
    }

    mDoc.scalarAppend(KeyCode::toChar(key));
    return DONE_STAY_NOPUSH;
}

Mode::Result StringInputMode::onPushed()
{
    if (mClear) {
        assert(mDoc.getInner().isScalar());
        mDoc.scalarClear();
    }

    mDoc.setHotLight(EditableDoc::HotLightLevel::POINT);
    return DONE_STAY_NOPUSH;
}

void StringInputMode::onPopped()
{
    mDoc.setHotLight(EditableDoc::HotLightLevel::OFF);
}

const char *StringInputMode::name()
{
    return "String Input";
}

