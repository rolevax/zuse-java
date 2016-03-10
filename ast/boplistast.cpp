#include "ast/boplistast.h"

#include <cassert>

BopListAst::BopListAst(Ast::Type t, Ast *lhs, Ast *rhs, int op)
    : ListAst(t)
{
    assert(isBopList());
    assert(0 <= op && op < numOp());

    if (lhs->getType() == t && isLeftAssoc()) {
        mergeIn(&lhs->asBopList());
        append(rhs);
        setOpAt(size() - 1, op);
    } else if (rhs->getType() == t && isRightAssoc()) {
        append(lhs);
        mergeIn(&rhs->asBopList(), op);
    } else { // neither operand should be flatten
        append(lhs);
        append(rhs);
        setOpAt(1, op);
    }
}

void BopListAst::dump() const
{
    std::cout << "TML:";
    ListAst::dump();
}

BopListAst *BopListAst::clone() const
{
    BopListAst *ret = new BopListAst(getType());
    for (size_t i = 0; i < size(); i++) {
        ret->append(at(i).clone());
        ret->setOpAt(i, opAt(i));
    }
    return ret;
}

Ast *BopListAst::remove(size_t pos)
{
    Ast *ret = ListAst::remove(pos);
    ops.erase(ops.begin() + pos);
    return ret;
}

void BopListAst::clear()
{
    // TODO: this causes ill-zero. is this ok?
    ListAst::clear();
    ops.clear();
}

int BopListAst::opAt(size_t pos) const
{
    assert(pos < size());
    return ops[pos];
}

void BopListAst::setOpAt(size_t pos, int op)
{
    assert(pos < size());
    assert(0 <= op && op < numOp());
    ops[pos] = op;
}

int BopListAst::numOp() const
{
    switch (getType()) {
    case Type::ADD_BOP_LIST:
        return 2;
    case Type::MUL_BOP_LIST:
        return 3;
    case Type::DOT_BOP_LIST:
        return 3;
    default:
        throw "WTF";
    }
}

void BopListAst::doInsert(size_t pos, Ast *child)
{
    ListAst::doInsert(pos, child);
    ops.insert(ops.begin() + pos, 0);
}

BopListAst::BopListAst(Ast::Type t)
    : ListAst(t)
{
}

void BopListAst::mergeIn(BopListAst *t, int lead)
{
    size_t size = t->size();
    for (size_t i = 0; i < size; i++) {
        // leading op of rhs is just placeholder, discard it
        int nextOp = i == 0 ? lead : t->opAt(0);
        append(t->remove(0));
        setOpAt(this->size() - 1, nextOp);
    }

    delete t;
}

bool BopListAst::isLeftAssoc() const
{
    return true; // TODO: dummy
}

bool BopListAst::isRightAssoc() const
{
    return !isLeftAssoc();
}

