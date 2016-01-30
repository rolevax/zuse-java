#include "core/hammer.h"
#include "core/tokens.h"
#include "core/fleshtoken.h"
#include "core/bonetoken.h"
#include "core/soultoken.h"

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
    if (Ast::isScalar(ast)) {
        buf.push_back(new FleshToken(&ScalarAst::fromAst(ast)));
    } else if (Ast::isList(ast)) {
        buf.push_back(new SoulToken(&ast, Token::Role::BEGIN));
        hitList(ListAst::fromAst(ast), buf);
        buf.push_back(new SoulToken(&ast, Token::Role::END));
    } else {
        buf.push_back(new SoulToken(&ast, Token::Role::BEGIN));

        switch (ast.getType()) {
        case Ast::Type::CLASS:
            hitClass(ClassAst::fromAst(ast), buf);
            break;
        case Ast::Type::METHOD:
            hitMethod(MethodAst::fromAst(ast), buf);
            break;
        case Ast::Type::ASSIGN:
            hitBop(BopAst::fromAst(ast), buf);
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
    switch (ast.getType()) {
    case Ast::Type::DECL_LIST:
        buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
        break;
    default:
        break;
    }

    for (size_t i = 0; i < ast.size(); i++) {
        hitGeneral(ast.at(i), buf);
        switch (ast.getType()) {
        case Ast::Type::CLASS_LIST:
        case Ast::Type::METHOD_LIST:
            buf.push_back(nullptr);
            buf.push_back(nullptr);
            break;
        case Ast::Type::DECL_LIST:
            // TODO comma
            break;
        case Ast::Type::STMT_LIST:
            buf.push_back(new BoneToken(&ast, BoneToken::Sym::SEMICOLON));
            buf.push_back(nullptr);
            break;
        default:
            break;
        }
    }

    switch (ast.getType()) {
    case Ast::Type::DECL_LIST:
        buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
        break;
    default:
        break;
    }
}

void Hammer::hitClass(const ClassAst &ast, Buf &buf)
{
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::CLASS));
    hitGeneral(ast.at(0), buf); // identifier
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
    buf.push_back(nullptr);

    // member (TODO: it's still method) list
    hitGeneral(ast.at(1), buf);

    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
}

void Hammer::hitMethod(const MethodAst &ast, Hammer::Buf &buf)
{
    // return type TODO
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::VOID));

    // id
    buf.push_back(new FleshToken(&ScalarAst::fromAst(ast.at(0))));

    // param list
    hitGeneral(ast.at(1), buf);

    // stmt list
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LBRACE));
    buf.push_back(nullptr);
    hitGeneral(ast.at(2), buf);
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RBRACE));
}

void Hammer::hitBop(const BopAst &ast, Hammer::Buf &buf)
{
    // lhs
    hitGeneral(ast.at(0), buf);

    buf.push_back(new BoneToken(&ast, BoneToken::Sym::ASSIGN));

    // rhs
    hitGeneral(ast.at(1), buf);
}

