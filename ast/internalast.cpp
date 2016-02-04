#include "ast/internalast.h"

InternalAst::InternalAst(Type t)
    : Ast(t)
{

}

void InternalAst::change(size_t pos, Ast *next)
{
    if (next != nullptr)
        next->parent = this;
    doChange(pos, next);
}

void InternalAst::nest(size_t pos, Ast *nester)
{
    //assert(nester->type == Type::ARRAY && nester->size() == 0);
    // TODO

    /*
    std::unique_ptr<Ast> nestee = remove(pos);
    insert(pos, nester);
    at(pos).insert(0, nestee.release());
    */
}

