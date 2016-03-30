#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "mode/mode.h"
#include "mode/macrohandler.h"

class ViewMode : public Mode
{
public:
    ViewMode(EditableDoc &doc);

    Result keyboard(Key key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    Mode *menulessListOp(ListOp op);

    MacroHandler macro;
};

#endif // VIEWMODE_H
