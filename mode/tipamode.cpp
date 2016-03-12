#include "mode/tipamode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"

#include <cstdlib>


TipaMode::TipaMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result TipaMode::keyboard(char key, bool top)
{
    char str[2] = { key, '\0' };

    if (isalpha(key)) {
        doc.change(Ast::Type::IDENT);
        doc.scalarClear();
        doc.scalarAppend(str);
        return { true, true, new IdentInputMode(doc, false) };
    } else if (isdigit(key)) {
        return { true, true, new NumberInputMode(doc, true) };
    } else if ('\"' == key) {
        doc.change(Ast::Type::STRING);
        return { true, true, new StringInputMode(doc, true) };
    }

    return { false, true, nullptr };
}

Mode::Result TipaMode::onPushed()
{
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
    return { false, true, nullptr };
}

const char *TipaMode::name()
{
    return "TIPA";
}
