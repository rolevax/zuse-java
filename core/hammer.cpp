#include "core/hammer.h"
#include "core/tokens.h"
#include "core/fleshtoken.h"
#include "core/bonetoken.h"
#include "core/soultoken.h"
#include "ast/termlistast.h"
#include "ast/fixsizeast.h"

#include <cassert>

Hammer::Hammer(Tokens &tokens) :
    tokens(tokens)
{

}

void Hammer::hit(const Ast &ast, size_t r, size_t c)
{
    Buf ts;
    hitGeneral(ast, ts);
    tokens.put(r, c, ts);
}

void Hammer::hitGeneral(const Ast &ast, Buf &buf)
{
    if (ast.isScalar()) {
        buf.push_back(new FleshToken(&ast.asScalar()));
    } else if (ast.isList()) {
        buf.push_back(new SoulToken(&ast, Token::Role::BEGIN));
        hitList(ast.asList(), buf);
        buf.push_back(new SoulToken(&ast, Token::Role::END));
    } else {
        buf.push_back(new SoulToken(&ast, Token::Role::BEGIN));

        switch (ast.getType()) {
        case Ast::Type::CLASS:
            hitClass(ast.asFixSize<2>(), buf);
            break;
        case Ast::Type::METHOD:
            hitMethod(ast.asFixSize<3>(), buf);
            break;
        case Ast::Type::DECL:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SPACE));
            hitGeneral(ast.asFixSize<2>().at(1), buf); // decl bean list
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SEMICOLON));
            break;
        case Ast::Type::DECL_BEAN:
            hitDeclBean(DeclBeanAst::fromAst(ast), buf);
            break;
        case Ast::Type::DECL_PARAM:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SPACE));
            hitGeneral(ast.asFixSize<2>().at(1), buf); // identifier
            break;
        case Ast::Type::RETURN:
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::RETURN));
            hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SEMICOLON));
            break;
        case Ast::Type::WHILE:
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::WHILE));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
            hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
            buf.push_back(nullptr);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // stmt
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
            break;
        case Ast::Type::DO_WHILE:
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::DO));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
            buf.push_back(nullptr);
            hitGeneral(ast.asFixSize<2>().at(0), buf); // stmt
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SPACE));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::WHILE));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
            hitGeneral(ast.asFixSize<2>().at(1), buf); // expr
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SEMICOLON));
            break;
        case Ast::Type::PAREN:
            hitParen(ast.asFixSize<1>(), buf);
            break;
        case Ast::Type::ASSIGN:
            hitInfixBop(ast.asFixSize<2>(), buf);
            break;
        case Ast::Type::CALL:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // method name
            hitGeneral(ast.asFixSize<2>().at(1), buf); // arg list
            break;
        default:
            assert("unhandled map type in hammer" && false);
            break;
        }

        buf.push_back(new SoulToken(&ast, Token::Role::END));
    }
}

void Hammer::hitList(const ListAst &ast, Buf &buf)
{
    hitListBegin(ast, buf);

    for (size_t i = 0; i < ast.size(); i++) {
        hitGeneral(ast.at(i), buf);
        hitListSep(ast, buf, i);
    }

    hitListEnd(ast, buf);
}

void Hammer::hitClass(const FixSizeAst<2> &ast, Buf &buf)
{
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::CLASS));
    hitGeneral(ast.at(0), buf); // identifier
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
    buf.push_back(nullptr);

    // member (TODO: it's still method) list
    hitGeneral(ast.at(1), buf);

    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
}

void Hammer::hitMethod(const FixSizeAst<3> &ast, Hammer::Buf &buf)
{
    // return type TODO
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::VOID));

    // id
    hitGeneral(ast.at(0), buf);

    // param list
    hitGeneral(ast.at(1), buf);

    // stmt list
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
    buf.push_back(nullptr);
    hitGeneral(ast.at(2), buf);
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
}

void Hammer::hitDeclBean(const DeclBeanAst &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // identifier
    if (ast.size() == 2) {
        // TODO: which ast for map sep token?
        buf.push_back(new BoneToken(&ast.at(1), BoneToken::Sym::ASSIGN));
        hitGeneral(ast.at(1), buf); // initialization
    }
}

void Hammer::hitParen(const FixSizeAst<1> &ast, Hammer::Buf &buf)
{
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
    hitGeneral(ast.at(0), buf);
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
}

void Hammer::hitInfixBop(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // lhs
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::ASSIGN));
    hitGeneral(ast.at(1), buf); // rhs
}

void Hammer::hitListBegin(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Ast::Type::DECL_PARAM_LIST:
    case Ast::Type::ARG_LIST:
        buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
        break;
    default:
        break;
    }
}

void Hammer::hitListEnd(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Ast::Type::DECL_PARAM_LIST:
    case Ast::Type::ARG_LIST:
        buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
        break;
    default:
        break;
    }
}

void Hammer::hitListSep(const ListAst &ast, Hammer::Buf &buf, size_t pos)
{
    bool end = pos == ast.size() - 1;

    switch (ast.getType()) {
    case Ast::Type::CLASS_LIST:
    case Ast::Type::METHOD_LIST:
        buf.push_back(nullptr);
        if (!end)
            buf.push_back(nullptr);
        break;
    case Ast::Type::STMT_LIST:
        buf.push_back(nullptr);
        break;
    case Ast::Type::DECL_BEAN_LIST:
    case Ast::Type::DECL_PARAM_LIST:
    case Ast::Type::ARG_LIST:
        if (!end) {
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::COMMA));
        }
        break;
    case Ast::Type::ADDSUB_LIST:
        if (!end) {
            const TermListAst &tast = TermListAst::fromAst(ast);
            BoneToken::Sym sym = tast.rasingAt(pos + 1) ? BoneToken::Sym::ADD
                                                        : BoneToken::Sym::SUB;
            buf.push_back(new BoneToken(&ast, sym));
        }
        break;
    case Ast::Type::MULDIV_LIST:
        if (!end) {
            const TermListAst &tast = TermListAst::fromAst(ast);
            BoneToken::Sym sym = tast.rasingAt(pos + 1) ? BoneToken::Sym::MUL
                                                        : BoneToken::Sym::DIV;
            buf.push_back(new BoneToken(&ast, sym));
        }
        break;
    default:
        break;
    }
}

