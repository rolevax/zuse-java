#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "mode/mode.h"

class ViewMode : public Mode
{
public:
    ViewMode(EditableDoc &doc);

    Result keyboard(char key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    Mode *menulessListOp(ListOp op);
    bool macro(char key, Mode *&nextPush);
    bool macroBop(char key, Mode *&nextPush);
};

#endif // VIEWMODE_H
