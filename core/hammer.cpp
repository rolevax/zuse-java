#include "core/hammer.h"
#include "core/tokens.h"
#include "core/fleshtoken.h"
#include "core/soultoken.h"
#include "ast/boplistast.h"
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
    buf.push_back(new SoulToken(&ast, Token::Role::BEGIN));

    bool autoParen = ast.precedence() > 0
            && ast.precedence() < ast.getParent().precedence();
    if (autoParen)
        bone(ast, buf, BoneToken::Sym::LPAREN);

    if (ast.isScalar()) {
        hitScalar(ast.asScalar(), buf);
    } else if (ast.isList()) {
        hitList(ast.asList(), buf);
    } else {
        switch (ast.getType()) {
        case Ast::Type::DECL_CLASS:
            hitClass(ast.asFixSize<2>(), buf);
            break;
        case Ast::Type::DECL_METHOD:
            hitMethod(ast.asFixSize<4>(), buf);
            break;
        case Ast::Type::DECL_VAR:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            bone(ast, buf, BoneToken::Sym::SPACE);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // decl bean list
            bone(ast, buf, BoneToken::Sym::SEMICOLON);
            break;
        case Ast::Type::DECL_PARAM:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            bone(ast, buf, BoneToken::Sym::SPACE);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // identifier
            break;
        case Ast::Type::RETURN:
            bone(ast, buf, BoneToken::Sym::RETURN);
            hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
            bone(ast, buf, BoneToken::Sym::SEMICOLON);
            break;
        case Ast::Type::WHILE:
            bone(ast, buf, BoneToken::Sym::WHILE);
            bone(ast, buf, BoneToken::Sym::LPAREN);
            hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
            bone(ast, buf, BoneToken::Sym::RPAREN);
            bone(ast, buf, BoneToken::Sym::LBRACE);
            buf.push_back(nullptr);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // stmt
            bone(ast, buf, BoneToken::Sym::RBRACE);
            break;
        case Ast::Type::DO_WHILE:
            bone(ast, buf, BoneToken::Sym::DO);
            bone(ast, buf, BoneToken::Sym::LBRACE);
            buf.push_back(nullptr);
            hitGeneral(ast.asFixSize<2>().at(0), buf); // stmt
            bone(ast, buf, BoneToken::Sym::RBRACE);
            bone(ast, buf, BoneToken::Sym::SPACE);
            bone(ast, buf, BoneToken::Sym::WHILE);
            bone(ast, buf, BoneToken::Sym::LPAREN);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // expr
            bone(ast, buf, BoneToken::Sym::RPAREN);
            bone(ast, buf, BoneToken::Sym::SEMICOLON);
            break;
        case Ast::Type::IF_CONDBODY:
            hitIfCondBody(ast.asFixSize<2>(), buf);
            break;
        case Ast::Type::IF_ELSEBODY:
            hitIfElseBody(ast.asFixSize<1>(), buf);
            break;
        case Ast::Type::PAREN:
            hitParen(ast.asFixSize<1>(), buf);
            break;
        case Ast::Type::ASSIGN:
            hitInfixBop(ast.asFixSize<2>(), buf);
            break;
        default:
            assert("unhandled map type in hammer" && false);
            break;
        }
    }

    if (autoParen)
        bone(ast, buf, BoneToken::Sym::RPAREN);

    buf.push_back(new SoulToken(&ast, Token::Role::END));
}

void Hammer::hitScalar(const ScalarAst &ast, Hammer::Buf &buf)
{
    if (ast.getType() == Ast::Type::META) {
        bone(ast, buf, BoneToken::Sym::META);
    } else {
        bool isString = ast.getType() == Ast::Type::STRING;
        if (isString)
            bone(ast, buf, BoneToken::Sym::DQUOTE);
        buf.push_back(new FleshToken(&ast.asScalar()));
        if (isString)
            bone(ast, buf, BoneToken::Sym::DQUOTE);
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
    bone(ast, buf, BoneToken::Sym::CLASS);
    hitGeneral(ast.at(0), buf); // identifier
    bone(ast, buf, BoneToken::Sym::LBRACE);
    buf.push_back(nullptr);

    // member (TODO: it's still method) list
    hitGeneral(ast.at(1), buf);

    bone(ast, buf, BoneToken::Sym::RBRACE);
}

void Hammer::hitMethod(const FixSizeAst<4> &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // return type
    bone(ast, buf, BoneToken::Sym::SPACE);
    hitGeneral(ast.at(1), buf); // id
    hitGeneral(ast.at(2), buf); // param list

    // stmt list
    bone(ast, buf, BoneToken::Sym::LBRACE);
    buf.push_back(nullptr);
    hitGeneral(ast.at(3), buf);
    bone(ast, buf, BoneToken::Sym::RBRACE);
}

void Hammer::hitIfCondBody(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, BoneToken::Sym::IF);
    bone(ast, buf, BoneToken::Sym::LPAREN);
    hitGeneral(ast.at(0), buf);
    bone(ast, buf, BoneToken::Sym::RPAREN);
    bone(ast, buf, BoneToken::Sym::LBRACE);
    buf.push_back(nullptr);
    hitGeneral(ast.at(1), buf);
}

void Hammer::hitIfElseBody(const FixSizeAst<1> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, BoneToken::Sym::LBRACE_NS);
    buf.push_back(nullptr);
    hitGeneral(ast.at(0), buf);
}

void Hammer::hitParen(const FixSizeAst<1> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, BoneToken::Sym::LPAREN);
    hitGeneral(ast.at(0), buf);
    bone(ast, buf, BoneToken::Sym::RPAREN);
}

void Hammer::hitInfixBop(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // lhs
    bone(ast, buf, BoneToken::Sym::ASSIGN);
    hitGeneral(ast.at(1), buf); // rhs
}

void Hammer::hitListBegin(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Ast::Type::DECL_PARAM_LIST:
        bone(ast, buf, BoneToken::Sym::LPAREN);
        break;
    case Ast::Type::ARG_LIST: {
        bone(ast, buf, BoneToken::Sym::LPAREN);
        break;
    }
    default:
        break;
    }
}

void Hammer::hitListEnd(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Ast::Type::DECL_PARAM_LIST:
        bone(ast, buf, BoneToken::Sym::RPAREN);
        break;
    case Ast::Type::ARG_LIST: {
        bone(ast, buf, BoneToken::Sym::RPAREN);
        break;
    }
    default:
        break;
    }
}

void Hammer::hitListSep(const ListAst &ast, Hammer::Buf &buf, size_t pos)
{
    bool end = pos == ast.size() - 1;

    switch (ast.getType()) {
    case Ast::Type::CLASS_LIST:
    case Ast::Type::MEMBER_LIST:
        buf.push_back(nullptr);
        if (!end)
            buf.push_back(nullptr);
        break;
    case Ast::Type::STMT_LIST:
        buf.push_back(nullptr);
        break;
    case Ast::Type::DECL_PARAM_LIST:
    case Ast::Type::ARG_LIST:
    case Ast::Type::DECTOR_LIST:
        if (!end)
            bone(ast, buf, BoneToken::Sym::COMMA);
        break;
    case Ast::Type::IF_LIST:
        bone(ast, buf, BoneToken::Sym::RBRACE);
        if (!end)
            bone(ast, buf, BoneToken::Sym::ELSE);
        break;
    case Ast::Type::ADD_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            BoneToken::Sym sym = bast.opAt(pos + 1) == 0 ? BoneToken::Sym::ADD
                                                         : BoneToken::Sym::SUB;
            bone(ast, buf, sym);
        }
        break;
    case Ast::Type::MUL_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            BoneToken::Sym sym = bast.opAt(pos + 1) == 0 ? BoneToken::Sym::MUL
                                                         : BoneToken::Sym::DIV;
            bone(ast, buf, sym);
        }
        break;
    case Ast::Type::DOT_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            int bop = bast.opAt(pos + 1);
            if (bop == BopListAst::DOT)
                bone(ast, buf, BoneToken::Sym::DOT);
        }
        break;
    default:
        break;
    }
}

void Hammer::bone(const Ast &ast, Buf &buf, BoneToken::Sym sym)
{
    buf.push_back(new BoneToken(&ast, sym));
}

