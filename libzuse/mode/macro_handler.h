#ifndef ZUSE_MACRO_HANDLER_H
#define ZUSE_MACRO_HANDLER_H

#include "../core/doc_editable.h"

class MacroHandler
{
public:
    explicit MacroHandler(DocEditable &mDoc);

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
    DocEditable &mDoc;
};

#endif // ZUSE_MACRO_HANDLER_H
