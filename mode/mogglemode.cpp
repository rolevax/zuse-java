#include "mode/mogglemode.h"
#include "core/editabledoc.h"

#include <cassert>


MoggleMode::MoggleMode(EditableDoc &doc)
    : Mode(doc)
{

}

Mode::Result MoggleMode::keyboard(Key key)
{
    switch (key) {
    case Key::SPACE:
    case Key::M:
        return DONE_POP_NOPUSH;
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

    return DONE_STAY_NOPUSH;
}

Mode::Result MoggleMode::onPushed()
{
    assert(mDoc.getInner().isFixSize());

    mDoc.toggleTension(true);
    return DONE_STAY_NOPUSH;
}

void MoggleMode::onPopped()
{
    mDoc.toggleTension(false);
}

const char *MoggleMode::name()
{
    return "Moggle";
}
