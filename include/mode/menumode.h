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

    void keyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    Ast::Type keyToType(char key);
    static int keyToBop(char key);

private:
    Context context;
};

#endif // MENUMODE_H
