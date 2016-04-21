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
            && ast.precedence() <= ast.getParent().precedence();
    if (autoParen)
        bone(ast, buf, Sym::LPAREN);

    if (ast.isScalar()) {
        hitScalar(ast.asScalar(), buf);
    } else if (ast.isList()) {
        hitList(ast.asList(), buf);
    } else {
        switch (ast.getType()) {
        case Type::DECL_CLASS:
            hitClass(ast.asFixSize<2>(), buf);
            break;
        case Type::DECL_METHOD:
            hitMethod(ast.asFixSize<4>(), buf);
            break;
        case Type::DECL_VAR:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            bone(ast, buf, Sym::SPACE);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // decl bean list
            bone(ast, buf, Sym::SEMICOLON);
            break;
        case Type::DECL_PARAM:
            hitGeneral(ast.asFixSize<2>().at(0), buf); // type
            bone(ast, buf, Sym::SPACE);
            hitGeneral(ast.asFixSize<2>().at(1), buf); // identifier
            break;
        case Type::RETURN:
            bone(ast, buf, Sym::RETURN);
            hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
            bone(ast, buf, Sym::SEMICOLON);
            break;
        case Type::WHILE:
            hitWhile(ast.asFixSize<2>(), buf);
            break;
        case Type::DO_WHILE:
            hitDoWhile(ast.asFixSize<2>(), buf);
            break;
        case Type::IF_CONDBODY:
            hitIfCondBody(ast.asFixSize<2>(), buf);
            break;
        case Type::LT:
        case Type::LEQ:
        case Type::GT:
        case Type::GEQ:
        case Type::INSTANCEOF:
        case Type::EQ:
        case Type::NEQ:
        case Type::LOGIC_OR:
        case Type::LOGIC_AND:
        case Type::BIT_OR:
        case Type::BIT_XOR:
        case Type::BIT_AND:
        case Type::ASSIGN:
            hitInfixBop(ast.asFixSize<2>(), buf);
            break;
        default:
            assert("unhandled map type in hammer" && false);
            break;
        }
    }

    if (autoParen)
        bone(ast, buf, Sym::RPAREN);

    buf.push_back(new SoulToken(&ast, Token::Role::END));
}

void Hammer::hitScalar(const ScalarAst &ast, Hammer::Buf &buf)
{
    if (ast.getType() == Type::META) {
        bone(ast, buf, Sym::META);
    } else {
        bool isString = ast.getType() == Type::STRING;
        if (isString)
            bone(ast, buf, Sym::DQUOTE);
        buf.push_back(new FleshToken(&ast.asScalar()));
        if (isString)
            bone(ast, buf, Sym::DQUOTE);
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
    bone(ast, buf, Sym::CLASS);
    hitGeneral(ast.at(0), buf); // identifier
    hitGeneral(ast.at(1), buf); // member list
}

void Hammer::hitMethod(const FixSizeAst<4> &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // return type
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(1), buf); // id
    hitGeneral(ast.at(2), buf); // param list
    hitGeneral(ast.at(3), buf); // stmt list
}

void Hammer::hitIfCondBody(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, Sym::IF);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf); // condition
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(1), buf); // statement list
}

void Hammer::hitWhile(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, Sym::WHILE);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.asFixSize<2>().at(0), buf); // expr
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.asFixSize<2>().at(1), buf); // stmt
}

void Hammer::hitDoWhile(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, Sym::DO);
    hitGeneral(ast.asFixSize<2>().at(0), buf); // stmt
    bone(ast, buf, Sym::SPACE);
    bone(ast, buf, Sym::WHILE);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.asFixSize<2>().at(1), buf); // expr
    bone(ast, buf, Sym::RPAREN);
    bone(ast, buf, Sym::SEMICOLON);
}

void Hammer::hitInfixBop(const FixSizeAst<2> &ast, Hammer::Buf &buf)
{
    hitGeneral(ast.at(0), buf); // lhs
    buf.push_back(new BoneToken(&ast));
    hitGeneral(ast.at(1), buf); // rhs
}

void Hammer::hitListBegin(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Type::MEMBER_LIST:
        bone(ast, buf, Sym::LBRACE);
        buf.push_back(nullptr);
        break;
    case Type::STMT_LIST:
        if (needBrace(ast)) {
            // use space-less left brace for else-body
            Sym sym = ast.getParent().getType() == Type::IF_LIST ? Sym::LBRACE_NS
                                                                 : Sym::LBRACE;
            bone(ast, buf, sym);
        }
        buf.push_back(nullptr);
        break;
    case Type::DECL_PARAM_LIST:
        bone(ast, buf, Sym::LPAREN);
        break;
    case Type::ARG_LIST: {
        bone(ast, buf, Sym::LPAREN);
        break;
    }
    default:
        break;
    }
}

void Hammer::hitListEnd(const ListAst &ast, Hammer::Buf &buf)
{
    switch (ast.getType()) {
    case Type::MEMBER_LIST:
        bone(ast, buf, Sym::RBRACE);
        break;
    case Type::STMT_LIST:
        bone(ast, buf, needBrace(ast) ? Sym::RBRACE : Sym::RBRACE_VT);
        break;
    case Type::DECL_PARAM_LIST:
        bone(ast, buf, Sym::RPAREN);
        break;
    case Type::ARG_LIST: {
        bone(ast, buf, Sym::RPAREN);
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
    case Type::CLASS_LIST:
    case Type::MEMBER_LIST:
        buf.push_back(nullptr);
        if (!end)
            buf.push_back(nullptr);
        break;
    case Type::STMT_LIST: {
            // add semicolons for expression statements
            Type bt = ast.at(pos).getType();
            // TODO: more conditions
            if (bt != Type::IF_LIST
                    && bt != Type::IF_CONDBODY
                    && bt != Type::WHILE
                    && bt != Type::DO_WHILE
                    && bt != Type::RETURN
                    && bt != Type::DECL_VAR) {
                bone(ast, buf, Sym::SEMICOLON);
            }
        }
        buf.push_back(nullptr);
        break;
    case Type::DECL_PARAM_LIST:
    case Type::ARG_LIST:
    case Type::DECTOR_LIST:
        if (!end)
            bone(ast, buf, Sym::COMMA);
        break;
    case Type::IF_LIST:
        if (!end) {
            if (needBrace(ast.at(0).asFixSize<2>().at(1).asList()))
                bone(ast, buf, Sym::SPACE);
            bone(ast, buf, Sym::ELSE);
        }
        break;
    case Type::ADD_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            Sym sym = bast.opAt(pos + 1) == 0 ? Sym::ADD : Sym::SUB;
            bone(ast, buf, sym);
        }
        break;
    case Type::MUL_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            Sym sym = bast.opAt(pos + 1) == 0 ? Sym::MUL : Sym::DIV;
            bone(ast, buf, sym);
        }
        break;
    case Type::DOT_BOP_LIST:
        if (!end) {
            const BopListAst &bast = ast.asBopList();
            int bop = bast.opAt(pos + 1);
            if (bop == BopListAst::DOT)
                bone(ast, buf, Sym::DOT);
        }
        break;
    default:
        break;
    }
}

bool Hammer::needBrace(const ListAst &ast, bool norec)
{
    assert(ast.getType() == Type::STMT_LIST);

    Type pt = ast.getParent().getType();
    if (pt == Type::DECL_METHOD || ast.size() != 1)
        return true;

    if (norec)
        return false;

    // a body inside 'if' needs braces whenever one of its siblings does
    // the following lines are all for that

    const ListAst *ifList = nullptr;
    if (pt == Type::IF_CONDBODY) {
        const InternalAst &par = ast.getParent().getParent();
        if (par.getType() == Type::IF_LIST)
            ifList = &par.asList();
    } else if (pt == Type::IF_LIST) {
        ifList = &ast.getParent().asList();
    }

    if (ifList != nullptr) {
        for (size_t i = 0; i < ifList->size(); i++) {
            const Ast &chd = ifList->at(i);
            Type ct = chd.getType();
            if (ct == Type::STMT_LIST && needBrace(chd.asList(), true)) // else body
                return true;
            if (ct == Type::IF_CONDBODY
                    && needBrace(chd.asFixSize<2>().at(1).asList(), true))
                return true;
        }
    }

    return false;
}

void Hammer::bone(const Ast &ast, Buf &buf, Sym sym)
{
    buf.push_back(new BoneToken(&ast, sym));
}

