#ifndef NORMALMODE_H
#define NORMALMODE_H

#include "mode.h"
#include "macrohandler.h"

class NormalMode : public Mode
{
public:
    NormalMode(EditableDoc &mDoc);

    Result keyboard(Key key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    Mode *menulessListOp(ListOp op);

private:
    MacroHandler mMacro;
};

#endif // NORMALMODE_H
