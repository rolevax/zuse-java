#ifndef ZUSE_MODE_MOGGLE_H
#define ZUSE_MODE_MOGGLE_H

#include "mode.h"

class ModeMoggle : public Mode
{
public:
    ModeMoggle(DocEditable &mDoc);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;
};

#endif // ZUSE_MODE_MOGGLE_H
