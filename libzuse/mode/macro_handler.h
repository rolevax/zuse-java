#ifndef ZUSE_MACRO_HANDLER_H
#define ZUSE_MACRO_HANDLER_H

#include "../core/doc_editable.h"

class MacroHandler
{
public:
    explicit MacroHandler(DocEditable &mDoc);

    MacroHandler(const MacroHandler& copy) = delete;
    MacroHandler &operator=(const MacroHandler &assign) = delete;

    bool macro(Key key, std::unique_ptr<Mode> &nextPush);

private:
    bool macroLeftParen(std::unique_ptr<Mode> &nextPush);
    bool macroLeftBrace(std::unique_ptr<Mode> &nextPush);
    bool macroComma(std::unique_ptr<Mode> &nextPush);
    bool macroEnter(std::unique_ptr<Mode> &nextPush, bool shift);
    bool macroBop(Key key, std::unique_ptr<Mode> &nextPush);
    std::unique_ptr<Mode> createMode(int offset = 0);

private:
    DocEditable &mDoc;
};

#endif // ZUSE_MACRO_HANDLER_H
