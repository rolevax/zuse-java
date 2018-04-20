#ifndef ZUSE_MODE_INPUT_NUMBER_H
#define ZUSE_MODE_INPUT_NUMBER_H

#include "mode.h"

class ModeInputNumber : public Mode
{
public:
    ModeInputNumber(DocEditable &mDoc, bool mClear);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool mClear;
};

#endif // ZUSE_MODE_INPUT_NUMBER_H
