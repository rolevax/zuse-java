#ifndef FIXSIZEINPUTMODE_H
#define FIXSIZEINPUTMODE_H

#include "mode/mode.h"
#include "mode/macrohandler.h"
#include "ast/internalast.h"

class FixSizeInputMode : public Mode
{
public:
    explicit FixSizeInputMode(EditableDoc &doc, size_t offset = 0);

    Result keyboard(Key key) override;
    Result onPushed() override;
    Result onResume() override;
    const char *name() override;

private:
    size_t stage;
    const InternalAst *ast;
    MacroHandler macro;
};

#endif // FIXSIZEINPUTMODE_H
