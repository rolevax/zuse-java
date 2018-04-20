#ifndef ZUSE_TOKEN_SOUL_H
#define ZUSE_TOKEN_SOUL_H

#include "token.h"

class TokenSoul : public Token
{
public:
    TokenSoul(const Ast *mAst, Role mRole);

    const std::string &getText() const;
};

#endif // ZUSE_TOKEN_SOUL_H
