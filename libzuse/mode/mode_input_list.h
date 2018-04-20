#ifndef ZUSE_MODE_INPUT_LIST_H
#define ZUSE_MODE_INPUT_LIST_H

#include "mode.h"

class ModeInputList : public Mode
{
public:
    ModeInputList(DocEditable &mDoc);
    Result onPushed() override;
    const char *name() override;
};

#endif // ZUSE_MODE_INPUT_LIST_H
