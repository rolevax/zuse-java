#include "core/hammer.h"
#include "core/tokens.h"
#include "core/fleshtoken.h"
#include "core/bonetoken.h"
#include "core/soultoken.h"
#include "ast/termlistast.h"

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
        buf.push_back(new FleshToken(&ScalarAst::fromAst(ast)));
    } else if (ast.isList()) {
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
        case Ast::Type::PAREN:
            hitParen(ParenAst::fromAst(ast), buf);
            break;
        case Ast::Type::ASSIGN:
            hitInfixBop(BopAst::fromAst(ast), buf);
            break;
        case Ast::Type::CALL:
            hitGeneral(ast.at(0), buf); // method name
            hitGeneral(ast.at(1), buf); // arg list
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

void Hammer::hitParen(const ParenAst &ast, Hammer::Buf &buf)
{
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::LPAREN));
    hitGeneral(ast.at(0), buf);
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::RPAREN));
}

void Hammer::hitInfixBop(const BopAst &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // lhs
    buf.push_back(new BoneToken(&ast, BoneToken::Sym::ASSIGN));
    hitGeneral(ast.at(1), buf); // rhs
}

void Hammer::hitListBegin(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Ast::Type::DECL_LIST:
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
    case Ast::Type::DECL_LIST:
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
    case Ast::Type::DECL_LIST:
        // TODO comma
        break;
    case Ast::Type::STMT_LIST:
        buf.push_back(new BoneToken(&ast, BoneToken::Sym::SEMICOLON));
        buf.push_back(nullptr);
        break;
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

