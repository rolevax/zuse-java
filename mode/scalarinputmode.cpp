#include "mode/scalarinputmode.h"
#include "mode/identinputmode.h"
#include "mode/stringinputmode.h"
#include "mode/numberinputmode.h"
#include "core/editabledoc.h"

#include <cstdlib>


ScalarInputMode::ScalarInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

void ScalarInputMode::keyboard(char key)
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

void ScalarInputMode::onPushed()
{
    doc.setHotLight(EditableDoc::HotLightLevel::AREA);
}

const char *ScalarInputMode::name()
{
    return "Scalar Input";
}
