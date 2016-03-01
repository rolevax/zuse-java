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

void TipaMode::keyboard(char key)
{
    char str[2] = { key, '\0' };

    if (isalpha(key)) {
        doc.change(Ast::Type::IDENT);
        doc.scalarClear();
        doc.scalarAppend(str);
        doc.pop(new IdentInputMode(doc, false));
    } else if (isdigit(key)) {
        // TODO Ast::Type::NUMBER
    } else if ('\"' == key) {
        doc.change(Ast::Type::STRING);
        doc.pop(new StringInputMode(doc, true));
    }
}

void TipaMode::onPushed()
{
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
}

const char *TipaMode::name()
{
    return "TIPA";
}
