#ifndef MOGGLEMODE_H
#define MOGGLEMODE_H

#include "mode.h"

class MoggleMode : public Mode
{
public:
    MoggleMode(EditableDoc &mDoc);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;
};

#endif // MOGGLEMODE_H
