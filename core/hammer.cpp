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
        case Type::DECL_INTERFACE:
            hitClass(ast.asFixSize<4>(), buf);
            break;
        case Type::DECL_METHOD:
            hitMethod(ast.asFixSize<5>(), buf);
            break;
        case Type::DECL_VAR:
            hitVarDecl(ast.asFixSize<2>(), buf);
            break;
        case Type::DECL_PARAM:
            hitParamDecl(ast.asFixSize<2>(), buf);
            break;
        case Type::RETURN:
        case Type::BREAK:
        case Type::CONTINUE:
        case Type::THROW:
            hitJump(ast.asFixSize<1>(), buf);
            break;
        case Type::WHILE:
            hitWhile(ast.asFixSize<2>(), buf);
            break;
        case Type::DO_WHILE:
            hitDoWhile(ast.asFixSize<2>(), buf);
            break;
        case Type::FOR:
            hitFor(ast.asFixSize<4>(), buf);
            break;
        case Type::IF_CONDBODY:
            hitIfCondBody(ast.asFixSize<2>(), buf);
            break;
        case Type::CATCH:
            hitCatch(ast.asFixSize<2>(), buf);
            break;
        case Type::CAST:
            hitCast(ast.asFixSize<2>(), buf);
            break;
        case Type::NEW_CLASS:
        case Type::NEW_ARRAY:
            hitNew(ast.asFixSize<3>(), buf);
            break;
        case Type::SHL:
        case Type::SHR:
        case Type::SHRA:
        case Type::LT:
        case Type::LEQ:
        case Type::GT:
        case Type::GEQ:
        case Type::INSTANCEOF:
        case Type::EQ:
        case Type::NEQ:
        case Type::BIT_OR:
        case Type::BIT_XOR:
        case Type::BIT_AND:
        case Type::ASSIGN:
        case Type::ASS_ADD:
        case Type::ASS_SUB:
        case Type::ASS_MUL:
        case Type::ASS_DIV:
        case Type::ASS_MOD:
        case Type::ASS_AND:
        case Type::ASS_XOR:
        case Type::ASS_OR:
        case Type::ASS_SHL:
        case Type::ASS_SHR:
        case Type::ASS_SHRA:
            hitInfixBop(ast.asFixSize<2>(), buf);
            break;
        case Type::POST_INC:
        case Type::POST_DEC:
            hitPostfixUop(ast.asFixSize<1>(), buf);
            break;
        case Type::PRE_INC:
        case Type::PRE_DEC:
        case Type::UNARY_PLUS:
        case Type::UNARY_MINUS:
        case Type::LOGIC_NOT:
        case Type::BIT_NOT:
            hitPrefixUop(ast.asFixSize<1>(), buf);
            break;
        case Type::QUESTION:
            hitQuestion(ast.asFixSize<3>(), buf);
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

void Hammer::hitScalar(const ScalarAst &ast, Buf &buf)
{
    if (ast.getType() == Type::META) {
        bone(ast, buf, Sym::META);
    } else if (ast.getType() == Type::HIDDEN) {
        // do nothing, reserve for future feature
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

void Hammer::hitModifiers(Modifiers m, const Ast &ast, Buf &buf)
{
    if (m.access == Modifiers::PUBLIC)
        bone(ast, buf, Sym::PUBLIC);
    if (m.access == Modifiers::PROTECTED)
        bone(ast, buf, Sym::PROTECTED);
    if (m.access == Modifiers::PRIVATE)
        bone(ast, buf, Sym::PRIVATE);
    if (m.abstract)
        bone(ast, buf, Sym::ABSTRACT);
    if (m.final)
        bone(ast, buf, Sym::FINAL);
    if (m.statik)
        bone(ast, buf, Sym::STATIC);
    if (m.transient)
        bone(ast, buf, Sym::TRANSIENT);
    if (m.voladile)
        bone(ast, buf, Sym::VOLATILE);
    if (m.native)
        bone(ast, buf, Sym::NATIVE);
    if (m.synchronized)
        bone(ast, buf, Sym::SYNCHRONIZED);
}

void Hammer::hitClass(const FixSizeAst<4> &ast, Buf &buf)
{
    hitModifiers(ast.getModifiers(), ast, buf);
    bone(ast, buf, ast.getType() == Type::DECL_CLASS ? Sym::CLASS : Sym::INTERFACE);
    hitGeneral(ast.at(0), buf); // identifier

    if (ast.at(1).getType() != Ast::Type::HIDDEN) // extends
        bone(ast, buf, Sym::EXTENDS);
    hitGeneral(ast.at(1), buf);

    if (ast.at(2).getType() != Ast::Type::HIDDEN) // implements
        bone(ast, buf, Sym::IMPLEMENTS);
    hitGeneral(ast.at(2), buf);

    hitGeneral(ast.at(3), buf); // member list
}

void Hammer::hitMethod(const FixSizeAst<5> &ast, Buf &buf)
{
    hitModifiers(ast.getModifiers(), ast, buf);
    hitGeneral(ast.at(0), buf); // return type
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(1), buf); // id
    hitGeneral(ast.at(2), buf); // param list

    if (ast.at(3).getType() != Ast::Type::HIDDEN) // throws
        bone(ast, buf, Sym::THROWS);

    hitGeneral(ast.at(3), buf); // throws name list
    hitGeneral(ast.at(4), buf); // stmt list
}

void Hammer::hitVarDecl(const FixSizeAst<2> &ast, Buf &buf)
{
    hitModifiers(ast.getModifiers(), ast, buf);
    hitGeneral(ast.at(0), buf); // type
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(1), buf); // decl bean list

    // semicolon of 'for' is for's
    // catch does not have semicolon
    if (ast.getParent().getType() != Type::FOR
            && ast.getParent().getType() != Type::CATCH)
        bone(ast, buf, Sym::SEMICOLON);
}

void Hammer::hitParamDecl(const FixSizeAst<2> &ast, Buf &buf)
{
    hitModifiers(ast.getModifiers(), ast, buf);
    hitGeneral(ast.at(0), buf); // type
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(1), buf); // identifier
}

void Hammer::hitIfCondBody(const FixSizeAst<2> &ast, Buf &buf)
{
    bone(ast, buf, Sym::IF);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf); // condition
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(1), buf); // statement list
}

void Hammer::hitCatch(const FixSizeAst<2> &ast, Buf &buf)
{
    bone(ast, buf, Sym::CATCH);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf); // exception decl
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(1), buf); // statement list
}

void Hammer::hitWhile(const FixSizeAst<2> &ast, Buf &buf)
{
    bone(ast, buf, Sym::WHILE);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf); // expr
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(1), buf); // stmt
}

void Hammer::hitDoWhile(const FixSizeAst<2> &ast, Buf &buf)
{
    bone(ast, buf, Sym::DO);
    hitGeneral(ast.at(0), buf); // stmt
    bone(ast, buf, Sym::SPACE);
    bone(ast, buf, Sym::WHILE);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(1), buf); // expr
    bone(ast, buf, Sym::RPAREN);
    bone(ast, buf, Sym::SEMICOLON);
}

void Hammer::hitFor(const FixSizeAst<4> &ast, Buf &buf)
{
    bone(ast, buf, Sym::FOR);
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf);
    bone(ast, buf, Sym::SEMICOLON);
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(1), buf);
    bone(ast, buf, Sym::SEMICOLON);
    bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(2), buf);
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(3), buf);
}

void Hammer::hitJump(const FixSizeAst<1> &ast, Buf &buf)
{
    switch (ast.getType()) {
    case Type::RETURN:
        bone(ast, buf, Sym::RETURN);
        break;
    case Type::BREAK:
        bone(ast, buf, Sym::BREAK);
        break;
    case Type::CONTINUE:
        bone(ast, buf, Sym::CONTINUE);
        break;
    case Type::THROW:
        bone(ast, buf, Sym::THROW);
        break;
    default:
        throw "WTF: unexcected jump type";
    }

    if (ast.getType() != Type::THROW
            && ast.at(0).getType() != Type::HIDDEN)
        bone(ast, buf, Sym::SPACE);
    hitGeneral(ast.at(0), buf); // expr
    bone(ast, buf, Sym::SEMICOLON);
}

void Hammer::hitInfixBop(const FixSizeAst<2> &ast, Buf &buf)
{
    hitGeneral(ast.at(0), buf); // lhs
    buf.push_back(new BoneToken(&ast));
    hitGeneral(ast.at(1), buf); // rhs
}

void Hammer::hitPrefixUop(const FixSizeAst<1> &ast, Buf &buf)
{
    buf.push_back(new BoneToken(&ast)); // operator
    hitGeneral(ast.at(0), buf); // operand
}

void Hammer::hitPostfixUop(const FixSizeAst<1> &ast, Buf &buf)
{
    hitGeneral(ast.at(0), buf); // operand
    buf.push_back(new BoneToken(&ast)); // operator
}

void Hammer::hitCast(const FixSizeAst<2> &ast, Buf &buf)
{
    bone(ast, buf, Sym::LPAREN);
    hitGeneral(ast.at(0), buf); // type
    bone(ast, buf, Sym::RPAREN);
    hitGeneral(ast.at(1), buf); // val
}

void Hammer::hitNew(const FixSizeAst<3> &ast, Hammer::Buf &buf)
{
    bone(ast, buf, Sym::NEW);
    hitGeneral(ast.at(0), buf); // type
    hitGeneral(ast.at(1), buf); // arg list or array dims
    hitGeneral(ast.at(2), buf); // member list or hidden
}

void Hammer::hitQuestion(const FixSizeAst<3> &ast, Buf &buf)
{
    hitGeneral(ast.at(0), buf); // condition
    bone(ast, buf, Sym::QUESTION);
    hitGeneral(ast.at(1), buf); // val1
    bone(ast, buf, Sym::COLON);
    hitGeneral(ast.at(2), buf); // val2
}

void Hammer::hitListBegin(const ListAst &ast, Buf &buf)
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
    case Type::TRY_LIST:
        bone(ast, buf, Sym::TRY);
        break;
    case Type::DECL_PARAM_LIST:
        bone(ast, buf, Sym::LPAREN);
        break;
    case Type::ARG_LIST:
        bone(ast, buf, Sym::LPAREN);
        break;
    case Type::DOT_BOP_LIST:
        if (ast.size() > 0 && ast.asBopList().opAt(0) == BopListAst::ARR)
            bone(ast, buf, Sym::LSQUARE);
        break;
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

// pos: position of just already hit child
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
                    && bt != Type::DECL_VAR
                    && bt != Type::FOR
                    && bt != Type::TRY_LIST
                    && bt != Type::BREAK
                    && bt != Type::CONTINUE
                    && bt != Type::THROW) {
                bone(ast, buf, Sym::SEMICOLON);
            }
        }
        buf.push_back(nullptr);
        break;
    case Type::DECL_PARAM_LIST:
    case Type::ARG_LIST:
    case Type::DECTOR_LIST:
    case Type::NAME_LIST:
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
    case Type::TRY_LIST:
        if (!end && ast.at(pos + 1).getType() == Type::STMT_LIST)
            bone(ast, buf, Sym::FINALLY);
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
            int op = bast.opAt(pos + 1);
            Sym sym = op == 0 ? Sym::MUL : op == 1 ? Sym::DIV : Sym::MOD;
            bone(ast, buf, sym);
        }
        break;
    case Type::DOT_BOP_LIST: {
        const BopListAst &bast = ast.asBopList();
        if (bast.opAt(pos)) // previous bop
            bone(ast, buf, Sym::RSQUARE);

        if (!end) {
            int bop = bast.opAt(pos + 1);
            if (bop == BopListAst::DOT)
                bone(ast, buf, Sym::DOT);
            else if (bop == BopListAst::ARR)
                bone(ast, buf, Sym::LSQUARE);
        }
        break;
    }
    case Type::LOGIC_AND_BOP_LIST:
        if (!end)
            bone(ast, buf, Sym::LOGIC_AND);
        break;
    case Type::LOGIC_OR_BOP_LIST:
        if (!end)
            bone(ast, buf, Sym::LOGIC_OR);
        break;
    default:
        break;
    }
}

bool Hammer::needBrace(const ListAst &ast, bool norec)
{
    assert(ast.getType() == Type::STMT_LIST);

    Type pt = ast.getParent().getType();
    if (pt == Type::DECL_METHOD
            || pt == Type::TRY_LIST
            || pt == Type::CATCH
            || pt == Type::NEW_CLASS
            || ast.size() != 1)
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

