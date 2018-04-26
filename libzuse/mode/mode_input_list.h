#ifndef ZUSE_MODE_INPUT_LIST_H
#define ZUSE_MODE_INPUT_LIST_H

#include "mode.h"



namespace zuse
{



class ModeInputList : public Mode
{
public:
    ModeInputList(DocEditable &mDoc);
    Result onPushed() override;
    const char *name() override;
};



} // namespace zuse



#endif // ZUSE_MODE_INPUT_LIST_H
