#ifndef ZUSE_MODE_INPUT_STRING_H
#define ZUSE_MODE_INPUT_STRING_H

#include "mode.h"



namespace zuse
{



class ModeInputString : public Mode
{
public:
    ModeInputString(DocEditable &mDoc, bool mClear);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    bool mClear;
};



} // namespace zuse



#endif // ZUSE_MODE_INPUT_STRING_H
