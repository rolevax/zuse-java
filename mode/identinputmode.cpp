#include "mode/identinputmode.h"
#include "core/editabledoc.h"

#include <cassert>

IdentInputMode::IdentInputMode(EditableDoc &doc, bool clear)
    : Mode(doc),
      clear(clear)
{

}

void IdentInputMode::keyboard(char key)
{
    if (key == ' ') {
        doc.pop();
    } else {
        // check legal char
        doc.scalarAppend(key);
    }
}

void IdentInputMode::onPushed()
{
    if (clear) {
        assert(doc.getInner().getType() == Ast::Type::IDENT);
        doc.scalarClear();
    }

    doc.setHotLight(true);
}

void IdentInputMode::onPopped()
{
    doc.setHotLight(false);
}

const char *IdentInputMode::name()
{
    return "Ident Input";
}

