#ifndef ZUSE_HAMMER_H
#define ZUSE_HAMMER_H

#include "token_bone.h"
#include "../ast/ast_scalar.h"
#include "../ast/ast_fix_size.h"
#include "../ast/ast_list.h"



class Token;
class Tokens;

class Hammer
{
public:
    Hammer(Tokens &mTokens);

    Hammer(const Hammer&) = delete;
    Hammer &operator=(const Hammer&) = delete;

    void hit(const Ast &ast, size_t r, size_t c);

private:
    using Buf = std::vector<Token*>;
    using Type = Ast::Type;
    using Sym = TokenBone::Sym;

    void hitGeneral(const Ast &ast, Buf &buf);
    void hitScalar(const AstScalar &ast, Buf &buf);
    void hitList(const AstList &ast, Buf &buf);
    void hitModifiers(Modifiers m, const Ast &ast, Buf &buf);
    void hitClass(const AstFixSize<4> &ast, Buf &buf);
    void hitMethod(const AstFixSize<5> &ast, Buf &buf);
    void hitVarDecl(const AstFixSize<2> &ast, Buf &buf);
    void hitParamDecl(const AstFixSize<2> &ast, Buf &buf);
    void hitIfCondBody(const AstFixSize<2> &ast, Buf &buf);
    void hitCatch(const AstFixSize<2> &ast, Buf &buf);
    void hitWhile(const AstFixSize<2> &ast, Buf &buf);
    void hitDoWhile(const AstFixSize<2> &ast, Buf &buf);
    void hitFor(const AstFixSize<4> &ast, Buf &buf);
    void hitJump(const AstFixSize<1> &ast, Buf &buf);
    void hitInfixBop(const AstFixSize<2> &ast, Buf &buf);
    void hitPrefixUop(const AstFixSize<1> &ast, Buf &buf);
    void hitPostfixUop(const AstFixSize<1> &ast, Buf &buf);
    void hitCast(const AstFixSize<2> &ast, Buf &buf);
    void hitNew(const AstFixSize<3> &ast, Buf &buf);
    void hitQuestion(const AstFixSize<3> &ast, Buf &buf);

    void hitListBegin(const AstList &ast, Buf &buf);
    void hitListEnd(const AstList &ast, Buf &buf);
    void hitListSep(const AstList &ast, Buf &buf, size_t pos);
    static bool needBrace(const AstList &ast, bool norec = false);

    void bone(const Ast &ast, Buf &buf, TokenBone::Sym sym);

private:
    Tokens &mTokens;
};

#endif // ZUSE_HAMMER_H
