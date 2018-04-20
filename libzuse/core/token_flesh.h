#ifndef ZUSE_TOKEN_FLESH_H
#define ZUSE_TOKEN_FLESH_H

#include "token.h"
#include "../ast/ast_scalar.h"



class TokenFlesh : public Token
{
public:
    TokenFlesh(const AstScalar *mAst);

    const std::string &getText() const override;

private:
    const AstScalar *mScalar;
};

#endif // ZUSE_TOKEN_FLESH_H
