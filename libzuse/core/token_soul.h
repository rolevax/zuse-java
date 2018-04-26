#ifndef ZUSE_TOKEN_SOUL_H
#define ZUSE_TOKEN_SOUL_H

#include "token.h"



namespace zuse
{



class TokenSoul : public Token
{
public:
    TokenSoul(const Ast *mAst, Role mRole);

    const std::string &getText() const;
};



} // namespace zuse



#endif // ZUSE_TOKEN_SOUL_H
