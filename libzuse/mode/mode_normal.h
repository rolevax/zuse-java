#ifndef ZUSE_MODE_NORMAL_H
#define ZUSE_MODE_NORMAL_H

#include "mode.h"
#include "macro_handler.h"

class ModeNormal : public Mode
{
public:
    ModeNormal(DocEditable &mDoc);

    Result keyboard(Key key) override;
    const char *name() override;

private:
    enum class ListOp { INSERT, APPEND, ASSART };
    std::unique_ptr<Mode> menulessListOp(ListOp op);

private:
    MacroHandler mMacro;
};

#endif // ZUSE_MODE_NORMAL_H
