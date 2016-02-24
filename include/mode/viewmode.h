#ifndef VIEWMODE_H
#define VIEWMODE_H

#include "mode/mode.h"

class ViewMode : public Mode
{
public:
    ViewMode(EditableDoc &doc);

    void keyboard(char key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    void menulessListOp(ListOp op);
};

#endif // VIEWMODE_H
