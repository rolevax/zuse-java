#include "ast/termlistast.h"

#include <cassert>

TermListAst &TermListAst::fromAst(Ast &ast)
{
    assert(ast.getType() == Type::ADDSUB_LIST
           || ast.getType() == Type::MULDIV_LIST);
    return static_cast<TermListAst&>(ast);
}

const TermListAst &TermListAst::fromAst(const Ast &ast)
{
    assert(ast.getType() == Type::ADDSUB_LIST
           || ast.getType() == Type::MULDIV_LIST);
    return static_cast<const TermListAst&>(ast);
}

TermListAst *TermListAst::makeBop(Ast *lhs, Ast *rhs, Op op)
{
    Ast::Type targetType = op == Op::ADD || op == Op::SUB ? Type::ADDSUB_LIST
                                                          : Type::MULDIV_LIST;
    bool midRaise = op == Op::ADD || op == Op::MUL;
    bool leftList = lhs->getType() == targetType;
    bool rightList = rhs->getType() == targetType;

    if (leftList && rightList) {
        TermListAst *tlhs = &TermListAst::fromAst(*lhs);
        TermListAst *trhs = &TermListAst::fromAst(*rhs);
        for (size_t i = 0; i < trhs->size(); i++) {
            bool raising = trhs->rasingAt(0);
            tlhs->append(trhs->remove(0).release());
            /* this is syntax level concat. only the first raise-sign
             * can be affected. actually that should always be rasing */
            bool nextRaise = i == 0 ? !(raising ^ midRaise) : raising;
            tlhs->setRasingAt(tlhs->size() - 1, nextRaise);
        }
        return tlhs;
    } else if (leftList) {
        TermListAst *tlhs = &TermListAst::fromAst(*lhs);
        tlhs->insert(lhs->size(), rhs);
        tlhs->setRasingAt(tlhs->size() - 1, midRaise);
        return tlhs;
    } else if (rightList) {
        TermListAst *trhs = &TermListAst::fromAst(*rhs);
        trhs->insert(0, lhs);
        // does not trigger semantic level raise-sign inverting
        trhs->setRasingAt(1, midRaise);
        return trhs;
    } else { // neither param matches the target list
        TermListAst *ret = new TermListAst(targetType);
        ret->append(lhs);
        ret->append(rhs);
        return ret;
    }
}

TermListAst::TermListAst(Type t)
    : ListAst(t)
{

}

bool TermListAst::rasingAt(size_t pos) const
{
    assert(pos < size());
    return rasing[pos];
}

void TermListAst::setRasingAt(size_t pos, bool b)
{
    assert(pos < size());
    rasing[pos] = b;
}

std::unique_ptr<Ast> TermListAst::remove(size_t pos)
{
    std::unique_ptr<Ast> ret = ListAst::remove(pos);
    rasing.erase(rasing.begin() + pos);
    return ret;
}

void TermListAst::doInsert(size_t pos, Ast *child)
{
    ListAst::doInsert(pos, child);
    rasing.insert(rasing.begin() + pos, true);
}
