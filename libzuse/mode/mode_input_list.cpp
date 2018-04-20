#include "mode_input_list.h"

#include "../core/doc_editable.h"

#include <cassert>

ModeInputList::ModeInputList(DocEditable &doc)
    : Mode(doc)
{

}

Mode::Result ModeInputList::onPushed()
{
    assert(mDoc.getInner().isList());
    assert(!mDoc.getInner().isBopList());

    mDoc.listClear();
    mDoc.assart(mDoc.getInner().asList().typeAt(0));
    return { ResultType::DONE_POP, mDoc.createModifyMode(true) };
}

const char *ModeInputList::name()
{
    return "";
}
