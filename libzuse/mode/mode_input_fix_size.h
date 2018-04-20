#ifndef ZUSE_MODE_INPUT_FIX_SIZE_H
#define ZUSE_MODE_INPUT_FIX_SIZE_H

#include "mode.h"
#include "macro_handler.h"

#include "../ast/ast_internal.h"



class ModeInputFixSize : public Mode
{
public:
    explicit ModeInputFixSize(DocEditable &mDoc, const AstInternal &f,
                              size_t offset = 0);

    Result keyboard(Key key) override;
    Result onPushed() override;
    Result onResume() override;
    const char *name() override;

private:
    Result nextStage();
    Result pushOrWait();

private:
    const AstInternal &mAst;
    size_t mStage;
    MacroHandler mMacro;
};

#endif // ZUSE_MODE_INPUT_FIX_SIZE_H
