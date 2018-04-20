#ifndef NUMBERINPUTMODE_H
#define NUMBERINPUTMODE_H

#include "mode.h"

class NumberInputMode : public Mode
{
public:
    NumberInputMode(EditableDoc &mDoc, bool mClear);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool mClear;
};

#endif // NUMBERINPUTMODE_H
