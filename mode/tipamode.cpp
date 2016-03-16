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
        return { ResultType::DONE_POP, new IdentInputMode(doc, false) };
    } else if (isdigit(key)) {
        return { ResultType::DONE_POP, new NumberInputMode(doc, true) };
    } else if ('\"' == key) {
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
