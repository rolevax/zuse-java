#ifndef HAMMER_H
#define HAMMER_H

#include "ast/scalarast.h"
#include "ast/listast.h"
#include "ast/classast.h"
#include "ast/methodast.h"
#include "ast/bopast.h"

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
    void hitList(const ListAst &ast, Buf &buf);
    void hitClass(const ClassAst &ast, Buf &buf);
    void hitMethod(const MethodAst &ast, Buf &buf);
    void hitBop(const BopAst &ast, Buf &buf);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
