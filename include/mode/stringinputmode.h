#ifndef STRINGINPUTMODE_H
#define STRINGINPUTMODE_H

#include "mode/mode.h"

class StringInputMode : public Mode
{
public:
    StringInputMode(EditableDoc &doc, bool clear);

    Result keyboard(char key, bool top) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool clear;
};

#endif // STRINGINPUTMODE_H
