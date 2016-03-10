#include "mode/listinputmode.h"
#include "core/editabledoc.h"

#include <cassert>

ListInputMode::ListInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

void ListInputMode::onPushed()
{
    assert(doc.getInner().isList());
    assert(!doc.getInner().isBopList());

    doc.listClear();
    doc.assart(doc.getInner().asList().typeAt(0));
    doc.pop(doc.createModifyMode(true));
}

const char *ListInputMode::name()
{
    return "";
}
