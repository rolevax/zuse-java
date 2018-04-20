#ifndef STRINGINPUTMODE_H
#define STRINGINPUTMODE_H

#include "mode.h"

class StringInputMode : public Mode
{
public:
    StringInputMode(EditableDoc &mDoc, bool mClear);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool mClear;
};

#endif // STRINGINPUTMODE_H
