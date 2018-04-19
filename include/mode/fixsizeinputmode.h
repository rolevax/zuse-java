#ifndef FIXSIZEINPUTMODE_H
#define FIXSIZEINPUTMODE_H

#include "mode/mode.h"
#include "mode/macrohandler.h"
#include "ast/internalast.h"

class FixSizeInputMode : public Mode
{
public:
    explicit FixSizeInputMode(EditableDoc &mDoc, const InternalAst &f,
                              size_t offset = 0);

    Result keyboard(Key key) override;
    Result onPushed() override;
    Result onResume() override;
    const char *name() override;

private:
    Result nextStage();
    Result pushOrWait();

private:
    const InternalAst &mAst;
    size_t mStage;
    MacroHandler mMacro;
};

#endif // FIXSIZEINPUTMODE_H
