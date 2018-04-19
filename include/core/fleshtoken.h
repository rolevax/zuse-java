#ifndef FLESHTOKEN_H
#define FLESHTOKEN_H

#include "core/token.h"
#include "ast/scalarast.h"

class FleshToken : public Token
{
public:
    FleshToken(const ScalarAst *mAst);

    const std::string &getText() const override;

private:
    const ScalarAst *mScalar;
};

#endif // FLESHTOKEN_H
