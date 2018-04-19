#ifndef SOULTOKEN_H
#define SOULTOKEN_H

#include "token.h"

class SoulToken : public Token
{
public:
    SoulToken(const Ast *mAst, Role mRole);

    const std::string &getText() const;
};

#endif // SOULTOKEN_H
