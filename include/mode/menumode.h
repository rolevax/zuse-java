#ifndef MENUMODE_H
#define MENUMODE_H

#include "mode/mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        BOP_INSERT, BOP_APPEND, NEST_AS_LEFT,
        FALL_SEARCH, DIG_SEARCH
    };

    MenuMode(EditableDoc &doc, Context context);

    Result keyboard(Key key) override;
    Result onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    Ast::Type keyToType(Key key);
    static int keyToBop(Key key);

private:
    Context context;
};

#endif // MENUMODE_H
