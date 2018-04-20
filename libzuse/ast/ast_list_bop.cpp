#include "ast_list_bop.h"
#include "ast_scalar.h"

#include <cassert>

AstListBop::AstListBop(Ast::Type t, Ast *lhs, Ast *rhs, int op)
    : AstList(t)
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

AstListBop::AstListBop(Ast *name, int dims)
    : AstList(Type::DOT_BOP_LIST)
{
    append(name);
    addDims(dims);
}

AstListBop *AstListBop::makeDims(Ast *a)
{
    AstListBop *res = new AstListBop(Type::DOT_BOP_LIST);
    res->addDims(a);
    return res;
}

AstListBop *AstListBop::makeDims(int dims)
{
    AstListBop *res = new AstListBop(Type::DOT_BOP_LIST);
    res->addDims(dims);
    return res;
}

void AstListBop::addDims(Ast *a)
{
    append(a);
    setOpAt(size() - 1, ARR);
}

void AstListBop::addDims(int dims)
{
    while (dims --> 0) {
        append(new AstScalar(Type::HIDDEN, ""));
        setOpAt(size() - 1, ARR);
    }
}

void AstListBop::dump() const
{
    std::cout << "TML:";
    AstList::dump();
}

AstListBop *AstListBop::clone() const
{
    AstListBop *ret = new AstListBop(getType());
    for (size_t i = 0; i < size(); i++) {
        ret->append(at(i).clone());
        ret->setOpAt(i, opAt(i));
    }
    return ret;
}

Ast *AstListBop::remove(size_t pos)
{
    Ast *ret = AstList::remove(pos);
    mOps.erase(mOps.begin() + pos);
    return ret;
}

void AstListBop::clear()
{
    // TODO: this causes ill-zero. is this ok?
    AstList::clear();
    mOps.clear();
}

int AstListBop::opAt(size_t pos) const
{
    assert(pos < size());
    return mOps[pos];
}

void AstListBop::setOpAt(size_t pos, int op)
{
    assert(pos < size());
    assert(0 <= op && op < numOp());
    mOps[pos] = op;
}

int AstListBop::numOp() const
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

void AstListBop::doInsert(size_t pos, Ast *child)
{
    AstList::doInsert(pos, child);
    mOps.insert(mOps.begin() + pos, 0);
}

AstListBop::AstListBop(Ast::Type t)
    : AstList(t)
{
}

void AstListBop::mergeIn(AstListBop *t, int lead)
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

bool AstListBop::isLeftAssoc() const
{
    return true; // TODO: dummy
}

bool AstListBop::isRightAssoc() const
{
    return !isLeftAssoc();
}

