#ifndef MENUMODE_H
#define MENUMODE_H

#include "mode/mode.h"

class MenuMode : public Mode
{
public:
    enum class Context
    {
        INSERT, APPEND, ASSART, CHANGE, NEST,
        FALL_SEARCH, DIG_SEARCH
    };

    MenuMode(EditableDoc &doc, Context context);

    void keyboard(char key) override;
    void emptyKeyboard(char key) override;
    void onPushed() override;
    void onPopped() override;
    const char *name() override;

private:
    Mode *modeFor(Ast::Type t);
    static Ast::Type keyToType(char key);

private:
    Context context;
};

#endif // MENUMODE_H
