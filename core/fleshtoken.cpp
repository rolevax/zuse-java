#include "core/fleshtoken.h"

FleshToken::FleshToken(const ScalarAst *ast) :
    Token(ast, Role::FLESH),
    mScalar(ast)
{

}

const std::string &FleshToken::getText() const
{
    return mScalar->getText();
}

