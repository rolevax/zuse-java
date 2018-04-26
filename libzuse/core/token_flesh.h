#ifndef ZUSE_TOKEN_FLESH_H
#define ZUSE_TOKEN_FLESH_H

#include "token.h"
#include "../ast/ast_scalar.h"



namespace zuse
{



class TokenFlesh : public Token
{
public:
    TokenFlesh(const AstScalar *mAst);

    const std::string &getText() const override;

private:
    const AstScalar *mScalar;
};



} // namespace zuse



#endif // ZUSE_TOKEN_FLESH_H
