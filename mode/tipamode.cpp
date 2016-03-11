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

Mode::Result TipaMode::keyboard(char key)
{
    char str[2] = { key, '\0' };

    if (isalpha(key)) {
        doc.change(Ast::Type::IDENT);
        doc.scalarClear();
        doc.scalarAppend(str);
        return { ResultType::POP, new IdentInputMode(doc, false) };
    } else if (isdigit(key)) {
        return { ResultType::POP, new NumberInputMode(doc, true) };
    } else if ('\"' == key) {
        doc.change(Ast::Type::STRING);
        return { ResultType::POP, new StringInputMode(doc, true) };
    }

    return { ResultType::STAY, nullptr };
}

void TipaMode::onPushed()
{
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
}

const char *TipaMode::name()
{
    return "TIPA";
}
