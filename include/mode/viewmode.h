#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "mode/mode.h"

class ViewMode : public Mode
{
public:
    ViewMode(EditableDoc &doc);

    Result keyboard(char key, bool top) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    Mode *menulessListOp(ListOp op);
};

#endif // VIEWMODE_H
