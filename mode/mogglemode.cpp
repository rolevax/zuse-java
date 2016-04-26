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
        doc.toggleAbstract();
        break;
    case Key::F:
        doc.toggleFinal();
        break;
    case Key::P:
        doc.toggleAccess();
        break;
    case Key::S:
        doc.toggleStatic();
        break;
    case Key::T:
        doc.toggleTransient();
        break;
    case Key::V:
        doc.toggleVolatile();
        break;
    case Key::N:
        doc.toggleNative();
        break;
    case Key::C:
        doc.toggleSynchronized();
        break;
    default:
        break;
    }

    return DONE_STAY_NOPUSH;
}

Mode::Result MoggleMode::onPushed()
{
    assert(doc.getInner().isFixSize());

    doc.toggleTension(true);
    return DONE_STAY_NOPUSH;
}

void MoggleMode::onPopped()
{
    doc.toggleTension(false);
}

const char *MoggleMode::name()
{
    return "Moggle";
}
