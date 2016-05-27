#ifndef HAMMER_H
#define HAMMER_H

#include "core/bonetoken.h"
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
    using Buf = std::vector<Token*>;
    using Type = Ast::Type;
    using Sym = BoneToken::Sym;

    void hitGeneral(const Ast &ast, Buf &buf);
    void hitScalar(const ScalarAst &ast, Buf &buf);
    void hitList(const ListAst &ast, Buf &buf);
    void hitModifiers(Modifiers m, const Ast &ast, Buf &buf);
    void hitClass(const FixSizeAst<2> &ast, Buf &buf);
    void hitMethod(const FixSizeAst<5> &ast, Buf &buf);
    void hitVarDecl(const FixSizeAst<2> &ast, Buf &buf);
    void hitParamDecl(const FixSizeAst<2> &ast, Buf &buf);
    void hitIfCondBody(const FixSizeAst<2> &ast, Buf &buf);
    void hitCatch(const FixSizeAst<2> &ast, Buf &buf);
    void hitWhile(const FixSizeAst<2> &ast, Buf &buf);
    void hitDoWhile(const FixSizeAst<2> &ast, Buf &buf);
    void hitFor(const FixSizeAst<4> &ast, Buf &buf);
    void hitJump(const FixSizeAst<1> &ast, Buf &buf);
    void hitInfixBop(const FixSizeAst<2> &ast, Buf &buf);
    void hitPrefixUop(const FixSizeAst<1> &ast, Buf &buf);
    void hitPostfixUop(const FixSizeAst<1> &ast, Buf &buf);
    void hitCast(const FixSizeAst<2> &ast, Buf &buf);
    void hitQuestion(const FixSizeAst<3> &ast, Buf &buf);

    void hitListBegin(const ListAst &ast, Buf &buf);
    void hitListEnd(const ListAst &ast, Buf &buf);
    void hitListSep(const ListAst &ast, Buf &buf, size_t pos);
    static bool needBrace(const ListAst &ast, bool norec = false);

    void bone(const Ast &ast, Buf &buf, BoneToken::Sym sym);

private:
    Tokens &tokens;
};

#endif // HAMMER_H
