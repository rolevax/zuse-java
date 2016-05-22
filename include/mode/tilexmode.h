#ifndef TILEXMODE_H
#define TILEXMODE_H

#include "mode.h"

class TilexMode : public Mode
{
public:
    TilexMode(EditableDoc &doc, bool macroContext);

    Result keyboard(Key key) override;
    Result onPushed() override;
    const char *name() override;

private:
    Result keyboardSpace();
    Result keyboardEqual();
    Result ppmm(bool inc);
    void castOuter(Ast::Type to);
    void relayMacro(int savedInner);

private:
    bool macroContext;
};

#endif // TILEXMODE_H
