#include "token_flesh.h"

TokenFlesh::TokenFlesh(const AstScalar *ast) :
    Token(ast, Role::FLESH),
    mScalar(ast)
{

}

const std::string &TokenFlesh::getText() const
{
    return mScalar->getText();
}

