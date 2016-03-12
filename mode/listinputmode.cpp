#include "mode/listinputmode.h"
#include "core/editabledoc.h"

#include <cassert>

ListInputMode::ListInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result ListInputMode::onPushed()
{
    assert(doc.getInner().isList());
    assert(!doc.getInner().isBopList());

    doc.listClear();
    doc.assart(doc.getInner().asList().typeAt(0));
    return { true, true, doc.createModifyMode(true) };
}

const char *ListInputMode::name()
{
    return "";
}
