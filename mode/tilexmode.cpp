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
    }

    return DONE_STAY_NOPUSH;
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
