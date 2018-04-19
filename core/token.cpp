#include "core/token.h"

Token::Token(const Ast *ast, Role role) :
    mAst(ast),
    mRole(role)
{

}

const Ast *Token::getAst() const
{
    return mAst;
}

Token::Role Token::getRole() const
{
    return mRole;
}



