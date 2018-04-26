#ifndef ZUSE_MODE_MOGGLE_H
#define ZUSE_MODE_MOGGLE_H

#include "mode.h"



namespace zuse
{



class ModeMoggle : public Mode
{
public:
    ModeMoggle(DocEditable &mDoc);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;
};



} // namespace zuse



#endif // ZUSE_MODE_MOGGLE_H
