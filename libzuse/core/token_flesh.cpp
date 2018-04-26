#include "token_flesh.h"



namespace zuse
{



TokenFlesh::TokenFlesh(const AstScalar *ast) :
    Token(ast, Role::FLESH),
    mScalar(ast)
{

}

const std::string &TokenFlesh::getText() const
{
    return mScalar->getText();
}



} // namespace zuse
