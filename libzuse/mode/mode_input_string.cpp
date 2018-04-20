#include "mode_input_string.h"

#include "../core/doc_editable.h"
#include "../ast/ast_scalar.h"

#include <cassert>

ModeInputString::ModeInputString(DocEditable &doc, bool clear) :
    Mode(doc),
    mClear(clear)
{

}

Mode::Result ModeInputString::keyboard(Key key)
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

Mode::Result ModeInputString::onPushed()
{
    if (mClear) {
        assert(mDoc.getInner().isScalar());
        mDoc.scalarClear();
    }

    mDoc.setHotLight(DocEditable::HotLightLevel::POINT);
    return DONE_STAY_NOPUSH;
}

void ModeInputString::onPopped()
{
    mDoc.setHotLight(DocEditable::HotLightLevel::OFF);
}

const char *ModeInputString::name()
{
    return "String Input";
}

