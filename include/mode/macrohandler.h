#ifndef MACROHANDLER_H
#define MACROHANDLER_H

#include "core/editabledoc.h"

class MacroHandler
{
public:
    explicit MacroHandler(EditableDoc &doc);

    MacroHandler(const MacroHandler& copy) = delete;
    MacroHandler &operator=(const MacroHandler &assign) = delete;

    bool macro(Key key, Mode *&nextPush);

private:
    bool macroBop(Key key, Mode *&nextPush);

private:
    EditableDoc &doc;
};

#endif // MACROHANDLER_H
