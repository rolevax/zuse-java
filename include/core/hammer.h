#ifndef HAMMER_H
#define HAMMER_H

#include "ast/scalarast.h"
#include "ast/listast.h"
#include "ast/classast.h"
#include "ast/methodast.h"
#include "ast/parenast.h"
#include "ast/bopast.h"
#include "ast/declbeanast.h"

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
    void hitDeclBean(const DeclBeanAst &ast, Buf &buf);
    void hitParen(const ParenAst &ast, Buf &buf);
    void hitInfixBop(const BopAst &ast, Buf &buf);

    void hitListBegin(const ListAst &ast, Buf &buf);
    void hitListEnd(const ListAst &ast, Buf &buf);
    void hitListSep(const ListAst &ast, Buf &buf, size_t pos);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
