#include "mode/tipamode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"

TipaMode::TipaMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result TipaMode::keyboard(Key key)
{
    char str[2] = { KeyCode::toChar(key), '\0' };

    if (KeyCode::isAlpha(key)) {
        doc.change(Ast::Type::IDENT);
        doc.scalarClear();
        doc.scalarAppend(str);
        return { ResultType::DONE_POP, new IdentInputMode(doc, false) };
    } else if (KeyCode::isDigit(key)) {
        return { ResultType::DONE_POP, new NumberInputMode(doc, true) };
    } else if (Key::DOUBLE_QUOTE == key) {
        doc.change(Ast::Type::STRING);
        return { ResultType::DONE_POP, new StringInputMode(doc, true) };
    }

    return DONE_STAY_NOPUSH;
}

Mode::Result TipaMode::onPushed()
{
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
    return DONE_STAY_NOPUSH;
}

const char *TipaMode::name()
{
    return "TIPA";
}
