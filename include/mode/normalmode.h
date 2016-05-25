#ifndef NORMALMODE_H
#define NORMALMODE_H

#include "mode/mode.h"
#include "mode/macrohandler.h"

class NormalMode : public Mode
{
public:
    NormalMode(EditableDoc &doc);

    Result keyboard(Key key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    Mode *menulessListOp(ListOp op);

    MacroHandler macro;
};

#endif // NORMALMODE_H
