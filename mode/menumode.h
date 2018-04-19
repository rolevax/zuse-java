#ifndef MENUMODE_H
#define MENUMODE_H

#include "mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        BOP_INSERT, BOP_APPEND, NEST_AS_LEFT, NEST_AS_RIGHT,
        FOCUS_IN_BIG, DOLLY_OUT_BIG, S_BIG, G_BIG,
        SWITCH_CLIP
    };

    MenuMode(EditableDoc &mDoc, Context mContext);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    Ast::Type keyToType(Key key);
    static int keyToBop(Key key);

private:
    Context mContext;
};

#endif // MENUMODE_H
