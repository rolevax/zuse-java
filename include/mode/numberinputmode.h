#ifndef NUMBERINPUTMODE_H
#define NUMBERINPUTMODE_H

#include "mode/mode.h"

class NumberInputMode : public Mode
{
public:
    NumberInputMode(EditableDoc &doc, bool clear);

    Result keyboard(char key, bool top) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool clear;
};

#endif // NUMBERINPUTMODE_H
