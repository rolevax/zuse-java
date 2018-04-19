#ifndef MACROHANDLER_H
#define MACROHANDLER_H

#include "../core/editabledoc.h"

class MacroHandler
{
public:
    explicit MacroHandler(EditableDoc &mDoc);

    MacroHandler(const MacroHandler& copy) = delete;
    MacroHandler &operator=(const MacroHandler &assign) = delete;

    bool macro(Key key, Mode *&nextPush);

private:
    bool macroLeftParen(Mode *&nextPush);
    bool macroLeftBrace(Mode *&nextPush);
    bool macroComma(Mode *&nextPush);
    bool macroEnter(Mode *&nextPush, bool shift);
    bool macroBop(Key key, Mode *&nextPush);
    Mode *createMode(int offset = 0);

private:
    EditableDoc &mDoc;
};

#endif // MACROHANDLER_H
