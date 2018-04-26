#include "ast_list_bop.h"
#include "ast_scalar.h"

#include <cassert>



namespace zuse
{



AstListBop::AstListBop(Ast::Type t, std::unique_ptr<Ast> lhs, std::unique_ptr<Ast> rhs, Bop op)
    : AstList(t)
{
    init(t, std::move(lhs), std::move(rhs), op);
}

AstListBop::AstListBop(std::unique_ptr<Ast> name, int dims)
    : AstList(Type::DOT_BOP_LIST)
{
    initDotList(std::move(name), dims);
}

AstListBop::AstListBop(Ast::Type t, Ast *lhs, Ast *rhs, Bop op)
    : AstList(t)
{
    init(t, std::unique_ptr<Ast>(lhs), std::unique_ptr<Ast>(rhs), op);
}

AstListBop::AstListBop(Ast *name, int dims)
    : AstList(Type::DOT_BOP_LIST)
{
    initDotList(std::unique_ptr<Ast>(name), dims);
}

std::unique_ptr<AstListBop> AstListBop::makeDims(std::unique_ptr<Ast> a)
{
    AstListBop res(Type::DOT_BOP_LIST);
    res.addDims(std::move(a));
    return std::make_unique<AstListBop>(std::move(res));
}

std::unique_ptr<AstListBop> AstListBop::makeDims(int dims)
{
    AstListBop res(Type::DOT_BOP_LIST);
    res.addDims(dims);
    return std::make_unique<AstListBop>(std::move(res));
}

void AstListBop::addDims(std::unique_ptr<Ast> a)
{
    append(std::move(a));
    setOpAt(size() - 1, Bop::ARR);
}

void AstListBop::addDims(int dims)
{
    while (dims --> 0) {
        append(std::make_unique<AstScalar>(Type::HIDDEN, ""));
        setOpAt(size() - 1, Bop::ARR);
    }
}

void AstListBop::dump() const
{
    std::cout << "TML:";
    AstList::dump();
}

std::unique_ptr<Ast> AstListBop::clone() const
{
    AstListBop res(getType());
    for (size_t i = 0; i < size(); i++) {
        res.append(at(i).clone());
        res.setOpAt(i, opAt(i));
    }

    return std::make_unique<AstListBop>(std::move(res));
}

std::unique_ptr<Ast> AstListBop::remove(size_t pos)
{
    auto res = AstList::remove(pos);
    mOps.erase(mOps.begin() + pos);
    return res;
}

void AstListBop::clear()
{
    // TODO: this causes ill-zero. is this ok?
    AstList::clear();
    mOps.clear();
}

Bop AstListBop::opAt(size_t pos) const
{
    assert(pos < size());
    return mOps[pos];
}

void AstListBop::setOpAt(size_t pos, Bop op)
{
    assert(pos < size());
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

void AstListBop::doInsert(size_t pos, std::unique_ptr<Ast> child)
{
    AstList::doInsert(pos, std::move(child));
    mOps.insert(mOps.begin() + pos, Bop::DEFAULT);
}

AstListBop::AstListBop(Ast::Type t)
    : AstList(t)
{
}

void AstListBop::init(Ast::Type t, std::unique_ptr<Ast> lhs, std::unique_ptr<Ast> rhs, Bop op)
{
    assert(isBopList());

    if (lhs->getType() == t && isLeftAssoc()) {
        mergeIn(lhs->asBopList());
        append(std::move(rhs));
        setOpAt(size() - 1, op);
    } else if (rhs->getType() == t && isRightAssoc()) {
        append(std::move(lhs));
        mergeIn(rhs->asBopList(), op);
    } else { // neither operand should be flatten
        append(std::move(lhs));
        append(std::move(rhs));
        setOpAt(1, op);
    }
}

void AstListBop::initDotList(std::unique_ptr<Ast> name, int dims)
{
    append(std::move(name));
    addDims(dims);
}

void AstListBop::mergeIn(AstListBop &t, Bop lead)
{
    size_t size = t.size();
    for (size_t i = 0; i < size; i++) {
        // leading op of rhs is just placeholder, discard it
        Bop nextOp = i == 0 ? lead : t.opAt(0);
        append(t.remove(0));
        setOpAt(this->size() - 1, nextOp);
    }
}

bool AstListBop::isLeftAssoc() const
{
    return true; // TODO: dummy
}

bool AstListBop::isRightAssoc() const
{
    return !isLeftAssoc();
}



} // namespace zuse
