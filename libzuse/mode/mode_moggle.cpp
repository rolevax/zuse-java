#include "mode_moggle.h"

#include "../core/doc_editable.h"

#include <cassert>


ModeMoggle::ModeMoggle(DocEditable &doc)
    : Mode(doc)
{

}

Mode::Result ModeMoggle::keyboard(Key key)
{
    switch (key) {
    case Key::SPACE:
    case Key::M:
        return Result::donePopNoPush();
    case Key::A:
        mDoc.toggleAbstract();
        break;
    case Key::F:
        mDoc.toggleFinal();
        break;
    case Key::P:
        mDoc.toggleAccess(true);
        break;
    case Key::S_P:
        mDoc.toggleAccess(false);
        break;
    case Key::S:
        mDoc.toggleStatic();
        break;
    case Key::T:
        mDoc.toggleTransient();
        break;
    case Key::V:
        mDoc.toggleVolatile();
        break;
    case Key::N:
        mDoc.toggleNative();
        break;
    case Key::C:
        mDoc.toggleSynchronized();
        break;
    default:
        break;
    }

    return Result::doneStayNoPush();
}

Mode::Result ModeMoggle::onPushed()
{
    assert(mDoc.getInner().isFixSize());

    mDoc.toggleTension(true);
    return Result::doneStayNoPush();
}

void ModeMoggle::onPopped()
{
    mDoc.toggleTension(false);
}

const char *ModeMoggle::name()
{
    return "Moggle";
}
