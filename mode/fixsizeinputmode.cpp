#include "core/editabledoc.h"
#include "mode/fixsizeinputmode.h"
#include "mode/menumode.h"
#include "mode/stringinputmode.h"

#include <cassert>

FixSizeInputMode::FixSizeInputMode(EditableDoc &doc, size_t offset)
    : Mode(doc)
    , stage(offset)
    , macro(doc)
{

}

Mode::Result FixSizeInputMode::keyboard(Key key)
{
    Mode *mode = nullptr;
    macro.macro(key, mode);
    return { ResultType::DONE_STAY, mode };
}

Mode::Result FixSizeInputMode::onPushed()
{
    ast = &doc.getInner().asInternal();
    assert(ast->isFixSize());

    // assume all fix-size node has at least size one
    doc.fallIn();
    return { ResultType::DONE_STAY, doc.createModifyMode(true) };
}

Mode::Result FixSizeInputMode::onResume()
{
    if (++stage == ast->size()) {
        doc.digOut();
        return { ResultType::DONE_POP, nullptr };
    } else {
        doc.sibling(+1);
        return { ResultType::DONE_STAY, doc.createModifyMode(true) };
    }
}

const char *FixSizeInputMode::name()
{
    return "Fix";
}
