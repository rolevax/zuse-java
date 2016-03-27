#ifndef FIXSIZEINPUTMODE_H
#define FIXSIZEINPUTMODE_H

#include "mode/mode.h"
#include "ast/internalast.h"

class FixSizeInputMode : public Mode
{
public:
    explicit FixSizeInputMode(EditableDoc &doc, size_t offset = 0);

    Result onPushed() override;
    Result onResume() override;
    const char *name() override;

private:
    size_t stage;
    const InternalAst *ast;
};

#endif // FIXSIZEINPUTMODE_H
