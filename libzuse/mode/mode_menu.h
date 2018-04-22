#ifndef ZUSE_MODE_MENU_H
#define ZUSE_MODE_MENU_H

#include "mode.h"
#include "../ast/bop.h"



class ModeMenu : public Mode
{
public:
    enum class Context
    {
        BOP_INSERT, BOP_APPEND, NEST_AS_LEFT, NEST_AS_RIGHT,
        FOCUS_IN_BIG, DOLLY_OUT_BIG, S_BIG, G_BIG,
        SWITCH_CLIP
    };

    ModeMenu(DocEditable &mDoc, Context mContext);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    Ast::Type keyToType(Key key);
    static Bop keyToBop(Key key);

private:
    Context mContext;
};

#endif // ZUSE_MODE_MENU_H
