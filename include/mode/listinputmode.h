#ifndef LISTINPUTMODE_H
#define LISTINPUTMODE_H

#include "mode/mode.h"

class ListInputMode : public Mode
{
public:
    ListInputMode(EditableDoc &mDoc);
    Result onPushed() override;
    const char *name() override;
};

#endif // LISTINPUTMODE_H
