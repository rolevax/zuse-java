#include "listinputmode.h"

#include "../core/editabledoc.h"

#include <cassert>

ListInputMode::ListInputMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result ListInputMode::onPushed()
{
    assert(mDoc.getInner().isList());
    assert(!mDoc.getInner().isBopList());

    mDoc.listClear();
    mDoc.assart(mDoc.getInner().asList().typeAt(0));
    return { ResultType::DONE_POP, mDoc.createModifyMode(true) };
}

const char *ListInputMode::name()
{
    return "";
}
