#ifndef HAMMER_H
#define HAMMER_H

#include "ast/scalarast.h"
#include "ast/fixsizeast.h"
#include "ast/listast.h"

class Token;
class Tokens;

class Hammer
{
public:
    Hammer(Tokens &tokens);

    Hammer(const Hammer&) = delete;
    Hammer &operator=(const Hammer&) = delete;

    void hit(const Ast &ast, size_t r, size_t c);

private:
    typedef std::vector<Token*> Buf;
    void hitGeneral(const Ast &ast, Buf &buf);
    void hitScalar(const ScalarAst &ast, Buf &buf);
    void hitList(const ListAst &ast, Buf &buf);
    void hitClass(const FixSizeAst<2> &ast, Buf &buf);
    void hitMethod(const FixSizeAst<4> &ast, Buf &buf);
    void hitIfCondBody(const FixSizeAst<2> &ast, Buf &buf);
    void hitIfElseBody(const FixSizeAst<1> &ast, Buf &buf);
    void hitParen(const FixSizeAst<1> &ast, Buf &buf);
    void hitInfixBop(const FixSizeAst<2> &ast, Buf &buf);

    void hitListBegin(const ListAst &ast, Buf &buf);
    void hitListEnd(const ListAst &ast, Buf &buf);
    void hitListSep(const ListAst &ast, Buf &buf, size_t pos);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
