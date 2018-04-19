#include "ast/boplistast.h"
#include "ast/scalarast.h"

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

BopListAst::BopListAst(Ast *name, int dims)
    : ListAst(Type::DOT_BOP_LIST)
{
    append(name);
    addDims(dims);
}

BopListAst *BopListAst::makeDims(Ast *a)
{
    BopListAst *res = new BopListAst(Type::DOT_BOP_LIST);
    res->addDims(a);
    return res;
}

BopListAst *BopListAst::makeDims(int dims)
{
    BopListAst *res = new BopListAst(Type::DOT_BOP_LIST);
    res->addDims(dims);
    return res;
}

void BopListAst::addDims(Ast *a)
{
    append(a);
    setOpAt(size() - 1, ARR);
}

void BopListAst::addDims(int dims)
{
    while (dims --> 0) {
        append(new ScalarAst(Type::HIDDEN, ""));
        setOpAt(size() - 1, ARR);
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
    mOps.erase(mOps.begin() + pos);
    return ret;
}

void BopListAst::clear()
{
    // TODO: this causes ill-zero. is this ok?
    ListAst::clear();
    mOps.clear();
}

int BopListAst::opAt(size_t pos) const
{
    assert(pos < size());
    return mOps[pos];
}

void BopListAst::setOpAt(size_t pos, int op)
{
    assert(pos < size());
    assert(0 <= op && op < numOp());
    mOps[pos] = op;
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
    case Type::LOGIC_AND_BOP_LIST:
    case Type::LOGIC_OR_BOP_LIST:
        return 1;
    default:
        throw "WTF";
    }
}

void BopListAst::doInsert(size_t pos, Ast *child)
{
    ListAst::doInsert(pos, child);
    mOps.insert(mOps.begin() + pos, 0);
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

